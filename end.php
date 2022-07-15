<?php
	require 'cfg.php';
	
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
		
		
		#insert if not extant
		$stmt = $db -> prepare ('
			IF NOT EXISTS (
				SELECT name FROM characters WHERE name = ?
			) THEN
				INSERT INTO characters (name) VALUES (?);
			END IF;
		');
		$stmt -> bind_param ('ss', $_POST ['who'], $_POST ['who']);
		$stmt -> execute ();
		$stmt -> close ();
		
		$stmt = $db -> prepare ('SELECT * FROM characters WHERE name = ?;');
		$stmt -> bind_param ('s', $_POST ['who']);
		$stmt -> execute ();
		
		$res = $stmt -> get_result ();
		$chr = $res -> fetch_assoc ();
		$res -> close ();
		$stmt -> close ();
		
		
		
		if (isset ($_POST ['q'])) { if ($_POST ['q'] !== '') {
			
			$stmt = $db -> prepare ('SELECT id FROM questions WHERE text = ?;');
			$stmt -> bind_param ('s', $_POST ['q']);
			$stmt -> store_result ();
			
			if ($stmt -> num_rows === 0) {
				#insert question
				$stmt -> free_result (); $stmt -> close ();
				
				$stmt = $db -> prepare ('INSERT INTO questions (text) VALUES (?);');
				$stmt -> bind_param ('s', $_POST ['q']);
				$stmt -> execute ();
				$stmt -> close ();
				
				$stmt = $db -> prepare ('SELECT id FROM questions WHERE text = ?;');
				$stmt -> bind_param ('s', $_POST ['q']);
				$stmt -> execute ();
				
				$res = $stmt -> get_result ();
				$question_id = $res -> fetch_column ();
				$res -> close ();
				
				$stmt -> close ();
				
				$db -> query ('ALTER TABLE characters ADD q_$question_id FLOAT;');
			}


			if (isset ($_POST ['q_ans'])) { if ($_POST ['q_ans'] !== '') {
				$_POST ['cookie'] [$question_id - 1] = $_POST ['q_ans'];
			} }


			
		} }
		
		$queery = array ();
		$cookie = str_split ($_POST ['cookie']);
		if (! isset ($chr ['name'])) { $chr ['name'] = $_POST ['who']; }
		# var_dump ($chr);
		foreach ($cookie as $i => $c) {
			# average out
			$cnt = $i + 1;
			
			if (! isset ($chr ["q_$cnt"])) { $chr ["q_$cnt"] = 0; }
			if ($chr ["q_$cnt"] === null) { $chr ["q_$cnt"] = 0; }
			
			if ($c !== 'q') {
				$chr ["q_$cnt"] = (($chr ["q_$cnt"] * 9.0) + strtoans ($c)) / 10.0;
				echo $chr ["q_$cnt"];
			}
			$queery [] = "q_$cnt = " . $chr ["q_$cnt"];
		}
		# var_dump ($queery);
		$queery = implode (', ', $queery);
		# echo $queery;
		$stmt = $db -> prepare ("UPDATE characters SET $queery WHERE name = ?;");
		$stmt -> bind_param ('s', $chr ['name']);
		$stmt -> execute ();
		$stmt -> close ();
		unset ($cookie, $i, $c, $cnt, $queery, $stmt);
		
		$stmt = $db -> prepare ('DELETE FROM cookies WHERE cookie = ?;');
		$stmt -> bind_param ('s', $key);
		$stmt -> execute ();
		$stmt -> close ();
		
				
		$db -> commit ();
		$db -> close ();

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
		<link rel="stylesheet" href="main.css" />
		<meta charset="UTF-8">
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
						$res = $db -> query ('SELECT name FROM characters WHERE text != \'\' ORDER BY name ASC;');
						
						while (true) {
							$row = $res -> fetch_row ();
							if ($row === null) { break; }
							echo '<option value="' . $row [0] . '" />' . "\n";
						}
						$res -> close ();
						unset ($row, $res);
					?>
				</datalist>
				
				<datalist id="questions">
					<?php
						$res = $db -> query ('SELECT text FROM questions WHERE text != \'\' ORDER BY text ASC;');
						
						while (true) {
							$row = $res -> fetch_row ();
							if ($row === null) { break; }
							echo '<option value="' . $row [0] . '" />' . "\n";
						}
						$res -> close ();
						unset ($row, $res);
					?>
				</datalist>
			</form>
		</div>
	</body>
</html>
