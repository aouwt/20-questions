<?php
	require 'cfg.php';
	
	if (isset ($_GET ['dump'])) {
		switch ($_GET ['dump']) {
			case 'v': $queery = 'SELECT * FROM verinfo;'; break;
			case 'c': $queery = 'SELECT * FROM characters WHERE name != \'\';'; break;
			case 'q': $queery = 'SELECT * FROM questions WHERE text != \'\';'; break;
			default: http_response_code (400); exit ();
		}
		
		$res = $db -> query ($queery);
		$all = $res -> fetch_all (MYSQLI_ASSOC);
		$res -> close ();
		
		$db -> close ();
		
		header ('Content-Type: application/json');
		echo json_encode ($all);
		exit ();
	}
	
	
	
	function rset () {
		global $question_total, $key, $db;
		
		if (! isset ($key)) { $key = mk_key (); }
		echo '<meta http-equiv="refresh" content="0;url=';
		echo '?cookie=' . str_repeat ('q', $question_total);
		echo "&key=$key";
		echo '" />';
		
		
		$db -> commit ();
		exit ();
	}
	
	$db -> begin_transaction ();
	
	
	# load questions
	$res = $db -> query ('SELECT id, text FROM questions;');
	$questions = array ();
	$fetch = $res -> fetch_all (MYSQLI_NUM);
	foreach ($fetch as $q) {
		$questions [$q [0]] = $q [1];
	}
	$question_total = $res -> num_rows;
	$res -> close ();
	unset ($res, $fetch, $q);
	
	
	# check for reqd params
	if (! isset ($_GET ['cookie'])) { rset (); }
	elseif ($_GET ['cookie'] === "") { rset (); }
	
	if (! isset ($_GET ['key'])) { rset (); }
	elseif ($_GET ['key'] === "") { rset (); }
	
	$key = get_key ($_GET ['key']);
	
	
	# parse arguments (1)
	if (isset ($_GET ['do'])) {
		switch ($_GET ['do']) {
			case 'restart':
				rset ();
			break;
			case 'report':
				if (! isset ($_GET ['qid'])) {
					http_response_code (400);
					exit ('<p><strong>Error:</strong> missing one or more required parameters</p>');
				}
				
				$qid = intval ($_GET ['qid']);
				
				$stmt = $db -> prepare ('INSERT INTO reports (qtext, qid, time) VALUES (?, ?, ?);');
				$stmt -> bind_param ('sii', $questions [$qid], $qid, time ());
				$stmt -> execute ();
				$stmt -> close ();
				
				$stmt = $db -> prepare ('UPDATE questions SET text = \'\' WHERE id = ?;');
				$stmt -> bind_param ('i', $qid);
				$stmt -> execute ();
				$stmt -> close ();
				
				unset ($stmt);
				
				echo '<meta http-equiv="refresh" content="0;url=?cookie=' . $_GET ['cookie'] . '&key=' . $_GET ['key'] . '" />';
				
				$db -> commit ();
				$db -> close ();
				exit ();
			break;
		}
	}
	
	$cookie = $_GET ['cookie'];


	# get user answers
	$str = str_split ($cookie);
	$userans = array ();
	foreach ($str as $i => $c) { $userans [$i + 1] = strtoans ($c); }
	unset ($str, $i, $c);
	
	# check answers
	$userans_ = array_count_values ($userans);
	if (! isset ($userans_ [0])) { $userans_ [0] = 0; }
	if (! isset ($userans_ [1])) { $userans_ [1] = 0; }
	if (! isset ($userans_ [-1])) { $userans_ [-1] = 0; }
	if ($userans_ [0] == 0) { rset (); } # ran out of questions to ask? retry (this shouldn't happen though)
	
	
	# load characters
	$res = $db -> query ('SELECT * FROM characters;');
	$all = $res -> fetch_all (MYSQLI_ASSOC);
	$keys = array_keys ($questions);
	$character = array ();
	foreach ($all as $chr) {
		$cname = $chr ['name'];
		foreach ($keys as $i) {
			$character [$cname] [$i] = $chr ["q_$i"];
		}
	}
	$res -> close ();
	unset ($res, $all, $keys, $chr, $cname, $i);
	
	
	# calculate character probabilities
	foreach ($character as $cname => $cdat) {
		$denom = 0; $num = 0.0;
		
		foreach ($cdat as $i => $q) { # each q
			if ($userans [$i] != 0) {
				$num += $q * $userans [$i];
				$denom ++;
			}
		}
		
		if ($denom != 0) { $chance [$cname] = $num / $denom; }
		else { $chance [$cname] = 0; }
	}
	unset ($cname, $cdat, $denom, $num, $i, $q);
	
	
	# find highest probablility character
	$max = -10.0;
	foreach ($chance as $i => $chr) {
		if ($chr >= $max) {
			$max = $chr;
			$target_character = $i;
		}
	}
	$confidence = $max + 1;
	unset ($i, $chr, $max);
	
	
	# check if end of game
	$question_no = $userans_ [1] + $userans_ [-1];
	if ($question_no >= 20) {
		$redir = true;
		$question_text = "Are you $target_character?";
		goto restofdoc;
	} else { $redir = false; }
	
	
	# get question
	$qid = 0;
	while (true) {
		while ($userans [$qid = rand (1, $question_total)] != 0); # make sure its not one we already asked!
		if ($questions [$qid] === '') { continue; } # blank questions are hidden/deleted
		
		if ($character [$target_character] [$qid] == 0) { break; }
		elseif (rand (0, 2) === 0) { break; }
	}
	$question_text = $questions [$qid];
	$question_id = $qid;
	unset ($qid);
	
	
	$cookie_f = $cookie_t = $cookie;
	
	$cookie_t [$question_id - 1] = 't';
	$cookie_f [$question_id - 1] = 'f';
	
	
restofdoc:
	#update cookie
	$ck = array ();
	foreach ($userans as $i => $chr) {
		$ck [$i] = anstostr ($chr);
	}
	$cookie = implode ($ck);
	unset ($ck, $i, $c);
	
	
	$db -> commit ();
	$db -> close ();
	
?>
<!DOCTYPE html>
<html>
	<head>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<link rel="stylesheet" href="main.css" />
		<title>QGame 3.2</title>
		<meta charset="UTF-8">
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
			
			<?php
				if ($redir) {
					echo "
						<p>
							<a class=\"btn\" style=\"background-color: green;\" href=\"end.php?ans=t&cookie=$cookie&key=$key&target=$target_character\">Yes</a>
							<a class=\"btn\" style=\"background-color: red;\" href=\"end.php?ans=f&cookie=$cookie&key=$key&target=$target_character\">No</a>
						</p>
					";
				} else {
					echo "
						<p>
							<a class=\"btn\" style=\"background-color: green;\" href=\"?cookie=$cookie_t&key=$key\">True</a>
							<a class=\"btn\" style=\"background-color: red;\" href=\"?cookie=$cookie_f&key=$key\">False</a>
							<a class=\"btn\" style=\"background-color: blue;\" href=\"?cookie=$cookie&key=$key\">Skip</a>
						</p>
						<p>
							<small><a class=\"btn\" style=\"background-color: red;\" href=\"?do=report&qid=$question_id&cookie=$cookie&key=$key\">Report question</a></small>
						</p>
					";
				}
			?>
		</div>
	</body>
</html>
