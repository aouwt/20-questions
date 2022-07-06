<?php
	require 'cfg.php';

	
	$db = new SQLite3 ($DB_PATH, SQLITE3_OPEN_READWRITE);
	
	# var_dump ($_POST);
	$db -> exec ('BEGIN TRANSACTION;');
	
	if ($_SERVER ['REQUEST_METHOD'] === 'POST') {
		# exit ();
		# check for required params
		if (
			(! isset ($_POST ['key'])) or
			(! isset ($_POST ['ans'])) or
			(! isset ($_POST ['cookie'])) or
			(! isset ($_POST ['who']))
		) {
			http_response_code (400);
			exit ('<p><strong>Error:</strong> missing one or more required parameters</p>');
		}
		
		
		$key = get_key ($_POST ['key']);
		
		# chr variable
		$_POST ['who'] = htmlspecialchars ($_POST ['who']);

		$chr = $db -> querySingle ('SELECT * FROM characters WHERE name = \'' . $_POST ['who'] . '\';', true);

		if ($chr === array ()) {
			$db -> exec ('INSERT INTO characters (name) VALUES (\'' . $_POST ['who'] . '\');');
			$chr = array ();
		}
		
		
		
		
		if (isset ($_POST ['q'])) { if ($_POST ['q'] !== '') {
			# check if extant
			$question_id = $db -> querySingle ('SELECT id FROM questions WHERE text = \'' . htmlspecialchars (strtolower ($_POST ['q'])) . '\';');
			
			
			$_POST ['q'] = htmlspecialchars ($_POST ['q']);
			
			
			if ($question_id === null) {
				#insert question
				$db -> exec ('INSERT INTO questions (text) VALUES (\'' . $_POST ['q'] . '\');');
				$question_id = $db -> querySingle ('SELECT id FROM questions WHERE text = \'' . $_POST ['q'] . '\';');
				$db -> exec ("ALTER TABLE characters ADD q_$question_id;");
			}


			if (isset ($_POST ['q_ans'])) { if ($_POST ['q_ans'] !== '') {
				$_POST ['cookie'] [$question_id - 1] = $_POST ['q_ans'];
			} }


			
		} }
		
		$s = array ();
		$ar = str_split ($_POST ['cookie']);
		if (! isset ($chr ['name'])) { $chr ['name'] = $_POST ['who']; }
		# var_dump ($chr);
		foreach ($ar as $n => $a) {
			# average out
			$i = $n + 1;
			
			if (! isset ($chr ["q_$i"])) { $chr ["q_$i"] = 0; }
			if ($chr ["q_$i"] === null) { $chr ["q_$i"] = 0; }
			
			if ($a !== 'q') {
				$chr ["q_$i"] = (($chr ["q_$i"] * 9.0) + strtoans ($a)) / 10.0;
			}
			$s [$n] = "q_$i = " . $chr ["q_$i"];
		}
		$s = implode (', ', $s);
		$db -> exec ("UPDATE characters SET $s WHERE name = '" . $chr ['name'] . '\';');
		unset ($ar, $n, $a, $i, $b, $s);
		
		
		$db -> exec ('UPDATE verinfo SET value = ' . time () . ' WHERE key = \'last_update\';');
		

		$db -> exec ("DELETE FROM keys WHERE value = '$key';");
				
		$db -> exec ('COMMIT;');

		echo '<h1>Thank you!</h1>';
		exit ();
	} else {
		# GETs have reqd params too!
		if (
			(! isset ($_GET ['key'])) or
			(! isset ($_GET ['ans'])) or
			(! isset ($_GET ['cookie'])) or
			(! isset ($_GET ['target']))
		) {
			http_response_code (400);
			exit ('<p><strong>Error:</strong> missing one or more required parameters</p>');
		}
	}
?>

<!DOCTYPE html>
<html>
	<head>
		<meta name="viewport" content="width=device-width, initial-scale=1"> 
		<style>
			<?php include_once $CSS_PATH; ?>
		</style>
	</head>
	<body>
		<div class="main">
			<p>Leave any field blank to omit answer from collection process. The information supplied here will be used to further train the AI. Do not put any personal information in here. This information will not be sold to advertisers. (who tf would buy it?)</p>
			<p>None of these fields are required, but input is recommended to improve the game.</p>
			<form action="?" method="post">
				<input type="hidden" name="ans" value="<?php echo htmlspecialchars ($_GET ['ans']); ?>" />
				<input type="hidden" name="cookie" value="<?php echo htmlspecialchars ($_GET ['cookie']); ?>" />
				<input type="hidden" name="key" value="<?php echo htmlspecialchars ($_GET ['key']); ?>" />
				<fieldset>
					<label for="who">I was: </label>
					<?php
						if ($_GET ['ans'] === 't') {
							echo '<input type="hidden" name="who" value="' . htmlspecialchars ($_GET ['target']) . '" />';
							echo '<input type="text" id="who" disabled=true value="' . htmlspecialchars ($_GET ['target']) . '" />';
						} else {
							echo '<input type="text" id="who" name="who" list="characters" />';
						}
					?>
				</fieldset>
				<br />

				<fieldset>
					<label for="q">Add a question! </label>
					<input type="text" name="q" id="q" autocomplete="off" list="questions" />

					<br />

					<label for="q_ans">My character would've answered: </label>
					<select name="q_ans" id="q_ans">
						<option>Skip</option>
						<option value="t">True</option>
						<option value="f">False</option>
					</select>
				</fieldset>

				<br />

				<input class="btn" style="background-color: blue;" type="submit" value="Help train the AI!">
				
				
				
				<datalist id="characters">
					<?php
						$r = $db -> query ("SELECT name FROM characters ORDER BY name ASC;");
						
						while (true) {
							$q = $r -> fetchArray (SQLITE3_NUM);
							if ($q === false) { break; }
							echo '<option value="' . $q [0] . '" />' . "\n";
						}
						$r -> finalize ();
						unset ($r, $q);
					?>
				</datalist>
				
				<datalist id="questions">
					<?php
						$r = $db -> query ("SELECT text FROM questions ORDER BY text ASC;");
						
						while (true) {
							$q = $r -> fetchArray (SQLITE3_NUM);
							if ($q === false) { break; }
							echo '<option value="' . $q [0] . '" />' . "\n";
						}
						$r -> finalize ();
						unset ($r, $q);
					?>
				</datalist>
			</form>
		</div>
	</body>
</html>
