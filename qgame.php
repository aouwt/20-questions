<?php
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
	
	# check for cookies
	if (! isset ($_COOKIE ['game'])) {
		setcookie ('game', str_repeat ('q', $question_total));
		header ('Refresh:0');
		echo '<p>Please note that this game uses cookies. Please enable cookies to play this game. Thanks!</p>';
		exit (0);
	}
	
	# get user answers
	$s = str_split ($_COOKIE ['game']);
	$userans = array ();
	foreach ($s as $i => $c) {
		switch ($c) {
			case 't': $userans [$i + 1] = 1.0;
			break;
			case 'f': $userans [$i + 1] = -1.0;
			break;
			case 'q': $userans [$i + 1] = 0.0;
			break;
		}
	}
	unset ($s, $i, $c);
	
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
	
	
	
	
	foreach ($character as $ci => $c) {
		$d = 0; $n = 0.0;
		foreach ($c as $i => $q) {
			if ($userans [$i] !== 0) {
				$n += $q * $userans [$i];
				$d ++;
			}
		}
		
		$chance [$ci] = $n / $d;
	}
	unset ($ci, $c, $d, $n, $i, $q);
	
	
	
	$max = 0.0;
	foreach ($chance as $i => $c) {
		if ($c > $max) {
			$max = $c;
			$target_character = $i;
		}
	}
	unset ($i, $c, $max);
?>
<!DOCTYPE html>
<html>
	<head>
	</head>
	<body>
		<p>
			Question #<?php echo $question_no; ?>:
			<strong><?php echo $question_text; ?></strong>
		</p>
		<br />
		<p>
			<a href="?ans=t">True</a>
			<a href="?ans=f">False</a>
		</p>
	</body>

</html>
