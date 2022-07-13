<?php
	require 'cfg.php';
	
	# is localhost
	$is_authorized = in_array ($_SERVER ['REMOTE_ADDR'], ['::1', '127.0.0.1', 'localhost']);
	
	
	$db = new SQLite3 ($DB_PATH, SQLITE3_OPEN_READWRITE);
	
	if (isset ($_GET ['commit'])) {
		if (! $is_authorized) { http_response_code (403); exit ('Unauthorized remote address.'); }
		$db -> exec ('BEGIN TRANSACTION;');
		
		$qs = explode ("\n", $_GET ['commit']);
		
		$o = array ();
		foreach ($qs as $q) {
			if ($q === '') { continue; }
			$r = $db -> query ($q);
			$o2 = array ();
			
			while (true) {
				$a = $r -> fetchArray (SQLITE3_ASSOC);
				if ($a === false) { break; }
				$o2 [] = $a;
			}
			
			$o [] = $o2;
			
			$r -> finalize ();
		}
		
		header ('Content-Type: application/json');
		echo json_encode ($o);
		
		$db -> exec ('COMMIT;');
		unset ($r, $o, $a, $q, $o2, $qs);
		exit ();
	}
	
	$current_query = rawurlencode ($_GET ['q'] . "\n");
?>
<!DOCTYPE html>
<html>
	<head>
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<style>
			<?php include_once $CSS_PATH; ?>
		</style>
		<meta charset="UTF-8">
	</head>
	<body>
		<div class="main">
			<a class="btn" style="background-color: green;" href="?commit=<?php echo $current_query; ?>">Commit changes</a>
			<a class="btn" style="background-color: red;" href="?">Clear changes</a>
		</div>
		
		<br />
			
		<div class="main">
			<strong>Reports</strong>
			
			<table>
				<tr>
					<th>(ID) Question</th>
					<th>Reported at</th>
					<th>Actions</th>
				</tr>
				
				<?php
					$r = $db -> query ('SELECT text, qid, time FROM reports;');
					
					while (true) {
						$q = $r -> fetchArray (SQLITE3_NUM);
						
						if ($q === false) { break; }
						
						$a = htmlspecialchars (date ('c', $q [2]));
						$t = htmlspecialchars ($q [0]);
						$i = htmlspecialchars ($q [1]);
						
						$del_uri = $current_query . rawurlencode ("DELETE FROM reports WHERE qid = $i -- Delete report\n");
						$k_uri = $current_query . rawurlencode ("UPDATE questions SET text = '$t' WHERE id = $i -- Replace question\nDELETE FROM reports WHERE qid = $i -- Delete report\n");
						echo "
							<tr>
								<td>
									($i) <q>$t</q>
								</td>
								<td>
									<time>$a</time>
								</td>
								<td>
									<a class=\"btn\" style=\"background-color: red\" href=\"?q=$del_uri\" title=\"Delete question\">✗</a>
									<a class=\"btn\" style=\"background-color: green\" href=\"?q=$k_uri\" title=\"Keep question\">✓</a>
								</td>
							</tr>
						";
					}
					$r -> finalize ();
					unset ($r, $q, $a, $t, $i, $del_uri, $k_uri);
				?>
			</table>
		
		</div>
		
		<br />
		
		<div class="main">
			<strong>Active sessions</strong>
			
			<table>
				<tr>
					<th>Value</th>
					<th>Expiry</th>
					<th>Actions</th>
				</tr>
				
				<?php
					if ($is_authorized) {
						$r = $db -> query ('SELECT value, expiry FROM keys;');
						
						while (true) {
							$q = $r -> fetchArray (SQLITE3_NUM);
							
							if ($q === false) { break; }
							
							$e = date ('c', $q [1]);
							$k = htmlspecialchars ($q [0]);
							$u = $current_query . rawurlencode ("DELETE FROM keys WHERE value = '$k' -- Delete key\n");
							echo "
								<tr>
									<td>
										<code>$k</code>
									</td>
									<td>
										<time>$e</time>
									</td>
									<td>
										<a class=\"btn\" style=\"background-color: red;\" href=\"?q=$u\" title=\"Remove session\">🗑</a>
									</td>
										
								</tr>
							";
						}
						$r -> finalize ();
						unset ($r, $q, $e, $k, $u);
					}
				?>
			</table>	
		</div>
		
		<br />
		
		<div class="main">
			<strong>SQL database</strong>
			
			<form action="?" method="get">
				<label for="q">SQL:</label>
				<br />
				<textarea name="q" id="q"><?php echo htmlspecialchars (rawurldecode ($current_query)); ?></textarea>
				<br />
				<input class="btn" type="submit" value="→" />
			</form>
		</div>
	</body>
</html>
