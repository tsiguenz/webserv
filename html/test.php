<?php
	session_start();
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Upload files</title>
	</head>
	<body>
		<h1>Upload files</h1> 
		<?php
			// faire une page un peu styley
			// print_r($_SESSION);
			if (!isset($_SESSION["compt"]))
				$_SESSION["compt"] = 1;
			else
				$_SESSION["compt"] += 1;
			$compt = $_SESSION["compt"];
			echo "<p>$compt</p>";
			// if ($handle = opendir('./images')) {
			// 	while (false !== ($entry = readdir($handle))) {
			// 		if ($entry != "." && $entry != "..") {
			// 			echo "- <a href=\"./images/$entry\" target=\"_blank\">$entry</a><br><br>";
			// 		}
			// 	}
			// 	closedir($handle);
			// }
		?>
	</body>
</html>
