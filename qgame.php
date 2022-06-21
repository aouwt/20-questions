<?php
	function cset () {
		global $question_total;
		setcookie ('game', str_repeat ('q', $question_total));
		echo '<meta http-equiv="refresh" content="0;url=?" />';
		echo '<p>Please note that this game uses cookies. Please enable cookies to play this game. Thanks!</p>';
		exit (0);
	}
	
	$db = new SQLite3 ('/usr/share/cgi-data/QGame/php.db');
	
	
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
	
	
	# parse arguments (1)
	if (isset ($_GET ['do'])) {
		switch ($_GET ['do']) {
			case 'restart': cset ();
			break;
		}
	}
	
	# check for cookies
	if (! isset ($_COOKIE ['game'])) { cset (); }
	elseif ($_COOKIE ['game'] === "") { cset (); }


	# get user answers
	$s = str_split ($_COOKIE ['game']);
	$userans = array ();
	foreach ($s as $i => $c) {
		switch ($c) {
			case 't': $userans [$i + 1] = 1;
			break;
			case 'f': $userans [$i + 1] = -1;
			break;
			case 'q': $userans [$i + 1] = 0;
			break;
		}
	}
	unset ($s, $i, $c);
	
	
	# parse arguments (2)
	if (isset ($_GET ['ans']) and isset ($_GET ['qid'])) {
		switch ($_GET ['ans']) {
			case 't': $userans [$_GET ['qid']] = 1;
			break;
			case 'f': $userans [$_GET ['qid']] = -1;
			break;
			case 'q': $userans [$_GET ['qid']] = 0;
			break;
		}
	}
	
	
	# check answers
	$userans_ = array_count_values ($userans);
	if (! isset ($userans_ [0])) { $userans_ [0] = 0; }
	if (! isset ($userans_ [1])) { $userans_ [1] = 0; }
	if (! isset ($userans_ [-1])) { $userans_ [-1] = 0; }
	if ($userans_ [0] == 0) { cset (); } # ran out of questions to ask? retry (this shouldn't happen though)
	
	
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
	
	
	# update cookie
	$a = array ();
	foreach ($userans as $i => $c) {
		switch ($c) {
			case 0: $a [$i] = 'q';
			break;
			case 1: $a [$i] = 't';
			break;
			case -1: $a [$i] = 'f';
			break;
		}
	}
	setcookie ('game', implode ($a));
	unset ($a, $i, $c);
restofdoc:
?>
<!DOCTYPE html>
<html>
	<head>
	</head>
	<body>
		<p>
			<small><a href="?do=restart">Restart game</a></small>
		</p>
		<p>
			Question #<?php echo $question_no; ?>:
			<strong><?php echo $question_text; ?></strong>
		</p>
		<br />
		<p>
			<?php
				if ($redir) {
					echo
						'<a href="end.php?ans=t&cookie=' . $_COOKIE ['game'] . '">Yes</a> ' .
						'<a href="end.php?ans=f&cookie=' . $_COOKIE ['game'] . '">No</a>'
					;
				} else {
					echo
						"<a href=\"?ans=t&qid=$question_id\">True</a> " .
						"<a href=\"?ans=f&qid=$question_id\">False</a> " .
						"<a href=\"?ans=q&qid=$question_id\">Skip</a>"
					;
				}
			?>
		</p>
	</body>
</html>
