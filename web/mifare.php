<?php

// MIFARE Poetry
// Roni Bandini, August 2024
// MIT License
// Brief: receives data from html form and saves Json for Arduino UNO R4 WiFi query
// Assign write permissions to mifare.json file


echo 'MIFARE Poetry<br>';
echo '<br>Line 1:' . htmlspecialchars($_POST["line1"]) . ' Ok';
echo '<br>Line 2:' . htmlspecialchars($_POST["line2"]) . ' Ok';
echo '<br>Line 3:' . htmlspecialchars($_POST["line3"]) . ' Ok';
echo '<br>Mode:' . htmlspecialchars($_POST["mode"]) . ' Ok';

$myfile = fopen("mifare.json", "w") or die("Unable to open config file");
$txt = "{\"mode\":\"".htmlspecialchars($_POST["mode"])."\",\"line1\":\"".htmlspecialchars($_POST["line1"])."\", \"line2\":\"".htmlspecialchars($_POST["line2"])."\", \"line3\":\"".htmlspecialchars($_POST["line3"])."\"}";
fwrite($myfile, $txt);
fclose($myfile);

echo '<br><br>File updated';
?>