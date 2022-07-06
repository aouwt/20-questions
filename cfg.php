<?php
	$DB_PATH = '/home/kit/git/20-questions/php.db';
	$CSS_PATH = '/home/kit/git/20-questions/main.css';
	
	function get_key ($key) {
		global $db;
		
		#delete old keys
		$key = htmlspecialchars ($key);
		$db -> exec ('DELETE FROM keys WHERE expiry < ' . time () . ';');
		
		#check if key exists
		if ($db -> querySingle ("SELECT * FROM keys WHERE value = '$key';") === null) {
			http_response_code (403);
			$db -> exec ('COMMIT;');
			exit ('<p><strong>Error:</strong> invalid key</p><p><a href="qgame.php">OK</a></p>');
		}
		
		return $key;
	}
	
	function mk_key () {
		global $db;
		$key = bin2hex (random_bytes (16));
		$time = (string) (time () + (60 * 10));
		$db -> exec ("INSERT INTO keys (value, expiry) VALUES ('$key', $time);");
		return $key;
	}
	
	
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
	
?>
