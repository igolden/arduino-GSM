<?php

// Setup our DB login info
$servername = "localhost";
$username = "root";
$password = "root";
$dbname = "arduino";

	if (!empty($_GET['lat']) && !empty($_GET['lon'])) {

		function getParameter($par, $default = null){
			if (isset($_GET[$par]) && strlen($_GET[$par])) return $_GET[$par];
			elseif (isset($_POST[$par]) && strlen($_POST[$par])) 
				return $_POST[$par];
			else return $default;
		}

		$lat = getParameter("lat");
		$lon = getParameter("lon");
		$person = $lat.",".$lon.",".$time.",".$sat.",".$speed.",".$course."\n";
		
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "INSERT INTO gps_data (latitude, longitude)
VALUES (".$lat.", ".$lon.")";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
// close connection
$conn->close();
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}
	}
	else {
    echo "Empty Params";
  };
?>

<br /> Testing the Response

