<?php
	require 'cfg.php';
	
	if (isset ($_GET ['dump'])) {
		switch ($_GET ['dump']) {
			case 'v': $what = 'verinfo'; break;
			case 'c': $what = 'characters'; break;
			case 'q': $what = 'questions'; break;
			default: http_response_code (400); exit ();
		}
		header ('Content-Type: application/json');
		echo shell_exec ("sqlite3 -json -readonly '$DB_PATH' 'SELECT * FROM $what;'");
		exit ();
	}
	
	
	
	function rset () {
		global $question_total, $key, $db;
		
		if (! isset ($key)) { $key = mk_key (); }
		echo '<meta http-equiv="refresh" content="0;url=';
		echo '?cookie=' . str_repeat ('q', $question_total);
		echo "&key=$key";
		echo '" />';
		
		
		$db -> exec ('COMMIT;');
		exit ();
	}
	
	$db = new SQLite3 ($DB_PATH, SQLITE3_OPEN_READWRITE);
	
	$db -> exec ('BEGIN TRANSACTION;');
	
	
	# load questions
	$r = $db -> query ('SELECT id, text FROM questions;');
	$questions = array ();
	while (true) {
		$q = $r -> fetchArray (SQLITE3_NUM);
		if ($q === false) { break; }
		$questions [$q [0]] = $q [1];
	}
	$r -> finalize ();
	$question_total = count ($questions);
	unset ($r, $q);
	
	
	# check for reqd params
	if (! isset ($_GET ['cookie'])) { rset (); }
	elseif ($_GET ['cookie'] === "") { rset (); }
	
	if (! isset ($_GET ['key'])) { rset (); }
	elseif ($_GET ['key'] === "") { rset (); }
	
	$key = get_key ($_GET ['key']);
	
	
	# parse arguments (1)
	if (isset ($_GET ['do'])) {
		switch ($_GET ['do']) {
			case 'restart': rset ();
			break;
		}
	}
	
	$cookie = $_GET ['cookie'];


	# get user answers
	$s = str_split ($cookie);
	$userans = array ();
	foreach ($s as $i => $c) { $userans [$i + 1] = strtoans ($c); }
	unset ($s, $i, $c);
	
	# check answers
	$userans_ = array_count_values ($userans);
	if (! isset ($userans_ [0])) { $userans_ [0] = 0; }
	if (! isset ($userans_ [1])) { $userans_ [1] = 0; }
	if (! isset ($userans_ [-1])) { $userans_ [-1] = 0; }
	if ($userans_ [0] == 0) { rset (); } # ran out of questions to ask? retry (this shouldn't happen though)
	
	
	# load characters
	$r = $db -> query ('SELECT * FROM characters;');
	$keys = array_keys ($questions);
	$character = array ();
	while (true) {
		$q = $r -> fetchArray (SQLITE3_ASSOC);
		if ($q === false) { break; }
		
		$c = $q ['name'];
		foreach ($keys as $i) {
			$character [$c] [$i] = $q ["q_$i"];
		}
	}
	$r -> finalize ();
	unset ($c, $i, $q, $r, $keys);
	
	
	# calculate character probabilities
	foreach ($character as $ci => $c) {
		$d = 0; $n = 0.0;
		foreach ($c as $i => $q) {
			if ($userans [$i] != 0) {
				$n += $q * $userans [$i];
				$d ++;
			}
		}
		if ($d != 0) { $chance [$ci] = $n / $d; }
		else { $chance [$ci] = 0; }
	}
	unset ($ci, $c, $d, $n, $i, $q);
	
	
	# find highest probablility character
	$max = -10.0;
	foreach ($chance as $i => $c) {
		if ($c >= $max) {
			$max = $c;
			$target_character = $i;
		}
	}
	$confidence = $max + 1;
	unset ($i, $c, $max);
	
	# check if end of game
	$question_no = $userans_ [1] + $userans_ [-1];
	if ($question_no >= 20) {
		$redir = true;
		$question_text = "Are you $target_character?";
		goto restofdoc;
	} else { $redir = false; }
	
	
	# get question
	$q = 0;
	while (true) {
		while ($userans [$q = rand (1, $question_total)] != 0); # make sure its not one we already asked!
		
		if ($character [$target_character] [$q] == 0) { break; }
		elseif (rand (0, 2) === 0) { break; }
	}
	$question_text = $questions [$q];
	$question_id = $q;
	unset ($q);
	
	
	$cookie_f = $cookie_t = $cookie;
	
	$cookie_t [$question_id - 1] = 't';
	$cookie_f [$question_id - 1] = 'f';
	
	
restofdoc:
	#update cookie
	$a = array ();
	foreach ($userans as $i => $c) {
		$a [$i] = anstostr ($c);
	}
	$cookie = implode ($a);
	unset ($a, $i, $c);
	
	
	$db -> exec ('COMMIT;');
	
?>
<!DOCTYPE html>
<html>
	<head>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<style>
			<?php include_once $CSS_PATH; ?>
		</style>
		<title>QGame 3.2</title>
	</head>
	<body>
		<div class="main">
			<p>
				<small><a href="?key=<?php echo $key; ?>">Restart game</a></small>
			</p>
			
			<progress id="prog" max=20 value=<?php echo $question_no; ?>><?php $question_no; ?></progress>
			
			<p>
				Question #<?php echo $question_no; ?>:
				<strong><?php echo $question_text; ?></strong>
			</p>
			
			<?php
				if ($redir) {
					echo "
						<p>
							<label for=\"confidence\" style=\"text-shadow: 0.1em 0.1em white; color: black;\">Confidence: </label>
							<meter id=\"confidence\" min=0 max=2 low=1 high=1.5 optimum=2 value=$confidence>$confidence</meter>
						</p>
					";
				}
			?>
			
			<p>
				<?php
					if ($redir) {
						echo "
							<a class=\"btn\" style=\"background-color: green;\" href=\"end.php?ans=t&cookie=$cookie&key=$key&target=$target_character\">Yes</a>
							<a class=\"btn\" style=\"background-color: red;\" href=\"end.php?ans=f&cookie=$cookie&key=$key&target=$target_character\">No</a>
						";
					} else {
						echo "
							<a class=\"btn\" style=\"background-color: green;\" href=\"?cookie=$cookie_t&key=$key\">True</a>
							<a class=\"btn\" style=\"background-color: red;\" href=\"?cookie=$cookie_f&key=$key\">False</a>
							<a class=\"btn\" style=\"background-color: blue;\"href=\"?cookie=$cookie&key=$key\">Skip</a>
						";
					}
				?>
			</p>
		</div>
	</body>
</html>
