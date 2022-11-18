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
			//TODO faire une page un peu styley
			if (!isset($_SESSION["compt"]))
				$_SESSION["compt"] = 1;
			else
				$_SESSION["compt"] += 1;
			$compt = $_SESSION["compt"];
			echo "<p>$compt</p>";
		?>
	</body>
</html>
