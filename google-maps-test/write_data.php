<?php

	if (!empty($_GET['latitude']) && !empty($_GET['longitude']) &&
		!empty($_GET['time']) && !empty($_GET['satellites']) &&
		!empty($_GET['speedOTG']) && !empty($_GET['course'])) {

		function getParameter($par, $default = null){
			if (isset($_GET[$par]) && strlen($_GET[$par])) return $_GET[$par];
			elseif (isset($_POST[$par]) && strlen($_POST[$par])) 
				return $_POST[$par];
			else return $default;
		}

		$file = 'gps.txt';
		$lat = getParameter("latitude");
		$lon = getParameter("longitude");
		$time = getParameter("time");
		$sat = getParameter("satellites");
		$speed = getParameter("speedOTG");
		$course = getParameter("course");
		$person = $lat.",".$lon.",".$time.",".$sat.",".$speed.",".$course."\n";
		
		echo "
			DATA:\n
			Latitude: ".$lat."\n
			Longitude: ".$lon."\n
			Time: ".$time."\n
			Satellites: ".$sat."\n
			Speed OTG: ".$speed."\n
			Course: ".$course;

		if (!file_put_contents($file, $person, FILE_APPEND | LOCK_EX))
			echo "\n\t Error saving Data\n";
		else echo "\n\t Data Save\n";
	}
	else {

?>


