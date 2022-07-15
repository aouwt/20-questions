<?php
	require 'cfg.php';
	
	# is localhost
	$is_authorized = in_array ($_SERVER ['REMOTE_ADDR'], ['::1', '127.0.0.1', 'localhost']);
	
	if (isset ($_GET ['commit'])) {
		if (! $is_authorized) { http_response_code (403); exit ('Unauthorized remote address.'); }
		$db -> begin_transaction ();
		
		$db -> 
		
		exit ();
	}
	
	$current_query = rawurlencode ($_GET ['q'] . "\n");
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
					$res = $db -> query ('SELECT qtext, qid, time FROM reports;');
					$all = $res -> fetch_all (MYSQLI_ASSOC);
						
					foreach ($all as $row) {
						$time = htmlspecialchars (date ('c', $row ['time']));
						$qtext = htmlspecialchars ($row ['qtext']);
						$qid = htmlspecialchars ($row ['qid']);
						
						$del_uri = $current_query . rawurlencode ("DELETE FROM reports WHERE qid = $qid -- Delete report\n");
						$keep_uri = $current_query . rawurlencode (
							"UPDATE questions SET text = '$qtext' WHERE id = $qid -- Replace question\n" .
							"DELETE FROM reports WHERE qid = $qid -- Delete report\n"
						);
						echo "
							<tr>
								<td>
									($qid) <q>$qtext</q>
								</td>
								<td>
									<time>$time</time>
								</td>
								<td>
									<a class=\"btn\" style=\"background-color: red\" href=\"?q=$del_uri\" title=\"Delete question\">✗</a>
									<a class=\"btn\" style=\"background-color: green\" href=\"?q=$keep_uri\" title=\"Keep question\">✓</a>
								</td>
							</tr>
						";
					}
					$res -> close ();
					unset ($res, $all, $row, $time, $qtext, $qid, $del_uri, $keep_uri);
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
						$res = $db -> query ('SELECT cookie, expiry FROM cookies;');
						$all = $res -> fetch_all (MYSQLI_ASSOC);
						
						foreach ($all as $row) {
							
							$expiry = date ('c', $row ['expiry']);
							$cookie = htmlspecialchars ($row ['cookie']);
							$queery = $current_query . rawurlencode ("DELETE FROM cookies WHERE cookie = '$k' -- Delete cookie\n");
							echo "
								<tr>
									<td>
										<code>$cookie</code>
									</td>
									<td>
										<time>$expiry</time>
									</td>
									<td>
										<a class=\"btn\" style=\"background-color: red;\" href=\"?q=$queery\" title=\"Remove session\">🗑</a>
									</td>
										
								</tr>
							";
						}
						$res -> close ();
						unset ($res, $all, $expiry, $cookie, $queery);
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
