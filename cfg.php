<?php
	include ('/home/kit/auth/qgame_3.php');
	
	$db = new mysqli ("localhost", "qgame_3", $DB_PASS, "qgame_3");
	
	$db -> begin_transaction ();
	
	function get_key ($key) {
		global $db;
		
		#delete old keys
		$stmt = $db -> prepare ('DELETE FROM cookies WHERE expiry < ?;');
		$stmt -> bind_param ('i', time ());
		$stmt -> execute ();
		$stmt -> close ();
		
		$stmt = $db -> prepare ('SELECT * FROM cookies WHERE cookie = ?;');
		$stmt -> bind_param ('s', $key);
		$stmt -> execute ();
		$stmt -> store_result ();
		
		#check if key exists
		if ($stmt -> num_rows === 0) {
			http_response_code (403);
			
			$stmt -> free_result ();
			$stmt -> close ();
			
			$db -> commit ();
			$db -> close ();
			
			exit ('<p><strong>Error:</strong> invalid key</p><p><a href="qgame.php">OK</a></p>');
		}
		
		$stmt -> free_result ();
		$stmt -> close ();	
		
		return $key;
	}
	
	function mk_key () {
		global $db;
		$key = bin2hex (random_bytes (16));
		$time = (string) (time () + (60 * 10));
		
		$stmt = $db -> prepare ('INSERT INTO cookies (cookie, expiry) VALUES (?, ?);');
		$stmt -> bind_param ('si', $key, $time);
		$stmt -> execute ();
		$stmt -> close ();
		
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
