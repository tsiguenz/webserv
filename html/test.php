<!DOCTYPE html>
<html>
	<head>
		<title>Upload files</title>
	</head>
	<body>
		<h1>Upload files</h1>
		<?php
			if ($handle = opendir('./images')) {
				while (false !== ($entry = readdir($handle))) {
					if ($entry != "." && $entry != "..") {
						echo "- <a href=\"./images/$entry\" target=\"_blank\">$entry</a><br><br>";
					}
				}
				closedir($handle);
			}
		?>
	</body>
</html>
