<?php
	function strtoans ($i) {
		switch ($i) {
			case 't': return 1;
			break;
			case 'f': return -1;
			break;
			case 'q': return 0;
			break;
		}
	}
	
	function anstostr ($i) {
		switch ($i) {
			case 1: return 't';
			break;
			case -1: return 'f';
			break;
			case 0: return 'q';
			break;
		}
	}
	
	
	$db = new SQLite3 ('/usr/share/cgi-data/QGame/php.db');
	
	
	
	
	if ($_SERVER ['REQUEST_METHOD'] === 'POST') {
		# check for required params
		if (
			(! isset ($_POST ['key'])) or
			(! isset ($_POST ['ans'])) or
			(! isset ($_POST ['cookie']))
		) { http_response_code (400); exit (); }
		
		
		#delete old keys
		$db -> exec ('DELETE FROM keys WHERE expiry < ' . time () . ';');
		
		#check if key exists
		if ($db -> querySingle ('SELECT * FROM keys WHERE value = \'' . htmlspecialchars ($_POST ['key']) . '\';') === null) { http_response_code (403); exit (); }
		
		$db -> exec ('DELETE FROM keys WHERE value = \'' . $_POST ['key'] . '\';');
		
		if (isset ($_POST ['who'])) {
			
		}
		
		
		
		
		if (isset ($_POST ['q'])) {
			#check if extant
			$question_id = $db -> querySingle ('SELECT id FROM questions WHERE text = \'' . htmlspecialchars (strtolower ($_POST ['q'])) . '\';');
			
			
			$_POST ['q'] = htmlspecialchars ($_POST ['q']);
			
			
			if ($question_id === null) {		
				#insert question
				$db -> exec ('INSERT INTO questions (text) VALUES (\'' . $_POST ['q'] . '\');');
				$question_id = $db -> querySingle ('SELECT id FROM questions WHERE text = \'' . $_POST ['q'] . '\';');
			}
			
			
			if (isset ($_POST ['q_ans'])) {
				$_POST ['cookie'] [$question_id - 1] = $_POST ['q_ans'];
			}
		}
		
		
		echo '<h1>Thank you!</h1>';
		exit ();
	} else {
		# GETs have reqd params too!
		if (
			(! isset ($_GET ['key'])) or
			(! isset ($_GET ['ans'])) or
			(! isset ($_GET ['cookie']))
		) { http_response_code (400); exit (); }
	}
?>

<!DOCTYPE html>
<html>
	<body>
		<p>Leave any field blank to omit answer from collection process. The information supplied here will be used to further train the AI. Do not put any personal information in here. This information will not be sold to advertisers. (who tf would buy it?)</p>
		<p>None of these fields are required, but input is recommended to improve the game.</p>
		<form action="?" method="post">
			<input type="hidden" name="ans" value="<?php echo htmlspecialchars ($_GET ['ans']); ?>" />
			<input type="hidden" name="cookie" value="<?php echo htmlspecialchars ($_GET ['cookie']); ?>" />
			<input type="hidden" name="key" value="<?php echo htmlspecialchars ($_GET ['key']); ?>" />
			<fieldset>
				<label for="who">I was: </label>
				<input type="text" name="who" id="corr" autocomplete="off" list="characters" />
			</fieldset>
			<br />

			<fieldset>
				<label for="extraq">Add a question! </label>
				<input type="text" name="extraq" id="q" autocomplete="off" list="questions" />

				<br />

				<label for="extraq_ans">My character would've answered: </label>
				<select name="extraq_ans" id="q_ans">
					<option>Skip</option>
					<option value="t">True</option>
					<option value="f">False</option>
				</select>
			</fieldset>

			<br />

			<input type="submit" value="Help train the AI!">
			
			
			
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
	</body>
</html>
