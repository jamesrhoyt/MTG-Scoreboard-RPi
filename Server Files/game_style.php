<?php
/*** set the content type header ***/
	header("Content-type: text/css");

	session_start();
	
	$servername = "localhost";
	$username = "root";
	$password = "mtgscoreboard";
	$dbname = "mtg";
	
	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	} 

	
	$sql = "SELECT COUNT(player_id) as total FROM teams Where game_id =".$_SESSION['game'];
	$result = $conn->query($sql);

	if ($result->num_rows == 1) {
		while($row = $result->fetch_assoc()) {
			$numPlayers = $row['total'];
		}
	} else {
		echo "Players Couldn't be found!";
	}
	
	
	$sql = "SELECT id, life, poison, player_id FROM teams Where game_id =".$_SESSION['game'];
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		// output data of each row
		$posID = 0;
		while($row = $result->fetch_assoc()) {
			// echo "Welcome to ".$row['id'];
			$sq2 = "SELECT name, background_color, text_color FROM players Where id =".$row['player_id'];
			$playerInfo = $conn->query($sq2);
			
			/*
			.player_0 {
				position: fixed;
				top: 0px;
				left: 0px;
				height: 416px;
				width: 1600px;
				background-color: coral;
				border: 10px solid #313131;
			}  
			*/
			
			
			if ($playerInfo->num_rows == 1) {
				while($info = $playerInfo->fetch_assoc()) {
					// echo "Player Name ".$info['name'];
					$display = '#player_'.$posID++.' {';
					$display .= 'position: fixed;';
					$display .= 'border: 10px solid #313131;';
					$display .= 'background-color: #'.$info['background_color'].';';
					$display .= 'color: #'.$info['text_color'].';';
					$display .= '}';
					echo $display;
				}
			} else {
				echo "Bad Player Info!";
			}
		}
	} else {
		echo "Couldn't Find Players!";
	}
	
	$conn->close();
	
	/*
	$containerSize = 100;
	if ( $numPlayers > 0){
		$containerSize = $containerSize/$numPlayers;
	}
	*/
?>

.player_life {
	font-size: 12vh;
	line-height: 1.0;
	white-space: nowrap;
	text-align: center;
	top: 33%;
	left: 50%;
}

.player_name_emp_1 {
	position: absolute;
	font-size: 11vh;
	line-height: 1.0;
	white-space: nowrap;
	text-align: center;
	top: 5%;
	left: 25%;
}

.player_name_emp_2 {
	position: absolute;
	font-size: 11vh;
	line-height: 1.0;
	white-space: nowrap;
	text-align: center;
	top: 5%;
	left: -5%;
}

.player_name_emp_3 {
	position: absolute;
	font-size: 11vh;
	line-height: 1.0;
	white-space: nowrap;
	text-align: center;
	top: 5%;
	left: 55%;
}

.life_new {
	position: absolute;
	text-align: center;
	font-size: 13vh;
	line-height: 1.0;
	white-space: nowrap;
	top: 23%;
	left: 44%;
}

.player_life_emp_1 {
	position: absolute;
	text_align: center;
	font-size: 13vh;
	line-height: 1.0;
	white-space: nowrap;
	top: 29%;
	left: 44%;
}

.player_life_emp_2 {
	position: absolute;
	text_align: center;
	font-size: 13vh;
	line-height: 1.0;
	white-space: nowrap;
	top: 29%;
	left: 14%;
}

.player_life_emp_3 {
	position: absolute;
	text_align: center;
	font-size: 13vh;
	line-height: 1.0;
	white-space: nowrap;
	top: 29%;
	left: 74%;
}

.player_addlife_button {
	display: inline-block;
	position: absolute;
	top: 31%;
	left: 74%;
	width: 32%;
	height: 8%;
}

.player_sublife_button {
	display: inline-block;
	position: absolute;
	top: 31%;
	left: 18%;
	width: 32%;
	height: 8%;
}

.player_addlife_emp_1 {
	display: inline-block;
	position: absolute;
	top: 24%;
	left: 46%;
	width: 40%;
	height: 4%;
}

.player_sublife_emp_1 {
	display: inline-block;
	position: absolute;
	top: 62%;
	left: 46%;
	width: 40%;
	height: 4%;
}

.player_addlife_emp_2 {
	display: inline-block;
	position: absolute;
	top: 24%;
	left: 16%;
	width: 40%;
	height: 4%;
}

.player_sublife_emp_2 {
	display: inline-block;
	position: absolute;
	top: 62%;
	left: 16%;
	width: 40%;
	height: 4%;
}

.player_addlife_emp_3 {
	display: inline-block;
	position: absolute;
	top: 24%;
	left: 76%;
	width: 40%;
	height: 4%;
}

.player_sublife_emp_3 {
	display: inline-block;
	position: absolute;
	top: 62%;
	left: 76%;
	width: 40%;
	height: 4%;
}

.player_poison {
	position: absolute;
	font-size: 9vh;
	line-height: 1.0;
	text-align: center;
	top: 68%;
	left: 44%;
}

.player_addpoison_button {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 64%;
	width: 24%;
	height: 6%;
}

.player_subpoison_button {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 30%;
	width: 24%;
	height: 6%;
}

.player_poison_alt {
	position: absolute;
	font-size: 9vh;
	line-height: 1.0;
	text-align: center;
	top: 68%;
	left: 14%;
}

.player_addpoison_button_alt {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 30%;
	width: 24%;
	height: 6%;
}

.player_subpoison_button_alt {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 6%;
	width: 24%;
	height: 6%;
}

.player_poison_emp_1 {
	position: absolute;
	font-size: 9vh;
	line-height: 1.0;
	text-align: center;
	top: 68%;
	left: 44%;
}

.player_poison_emp_2 {
	position: absolute;
	font-size: 9vh;
	line-height: 1.0;
	text-align: center;
	top: 68%;
	left: 14%;
}

.player_poison_emp_3 {
	position: absolute;
	font-size: 9vh;
	line-height: 1.0;
	text-align: center;
	top: 68%;
	left: 74%;
}

.player_addpoison_emp_1 {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 60%;
	width: 24%;
	height: 6%;
}

.player_subpoison_emp_1 {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 36%;
	width: 24%;
	height: 6%;
}

.player_addpoison_emp_2 {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 30%;
	width: 24%;
	height: 6%;
}

.player_subpoison_emp_2 {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 6%;
	width: 24%;
	height: 6%;
}

.player_addpoison_emp_3 {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 90%;
	width: 24%;
	height: 6%;
}

.player_subpoison_emp_3 {
	display: inline-block;
	position: absolute;
	top: 74%;
	left: 66%;
	width: 24%;
	height: 6%;
}

.player_commander_1 {
	position: absolute;
	font-size: 6vh;
	line-height: 1.0;
	text-align: center;
	top: 65%;
	left: 58%;
}

.player_cmdr_avatar_1 {
	position: absolute;
	top: 56%;
	left: 53%;
	width: 10%;
	height: 3%;
}

.player_addcmdr_button_1 {
	display: inline-block;
	position: absolute;
	top: 56%;
	left: 58%;
	width: 20%;
	height: 5%;
}

.player_subcmdr_button_1 {
	display: inline-block;
	position: absolute;
	top: 88%;
	left: 58%;
	width: 20%;
	height: 5%;
}

.player_commander_2 {
	position: absolute;
	font-size: 6vh;
	line-height: 1.0;
	text-align: center;
	top: 65%;
	left: 70%;
}

.player_cmdr_avatar_2 {
	position: absolute;
	top: 56%;
	left: 65%;
	width: 10%;
	height: 3%;
}

.player_addcmdr_button_2 {
	display: inline-block;
	position: absolute;
	top: 56%;
	left: 70%;
	width: 20%;
	height: 5%;
}

.player_subcmdr_button_2 {
	display: inline-block;
	position: absolute;
	top: 88%;
	left: 70%;
	width: 20%;
	height: 5%;
}

.player_commander_3 {
	position: absolute;
	font-size: 6vh;
	line-height: 1.0;
	text-align: center;
	top: 65%;
	left: 82%;
}

.player_cmdr_avatar_3 {
	position: absolute;
	top: 56%;
	left: 77%;
	width: 10%;
	height: 3%;
}

.player_addcmdr_button_3 {
	display: inline-block;
	position: absolute;
	top: 56%;
	left: 82%;
	width: 20%;
	height: 5%;
}

.player_subcmdr_button_3 {
	display: inline-block;
	position: absolute;
	top: 88%;
	left: 82%;
	width: 20%;
	height: 5%;
}

.player_cmdr_scroll_left {
	display: inline-block;
	position: absolute;
	top: 72%;
	left: 48%;
	width: 10%;
	height: 10%;
}

.player_cmdr_scroll_right {
	display: inline-block;
	position: absolute;
	top: 72%;
	left: 90%;
	width: 10%;
	height: 10%;
}

.page_left_button {
	display: inline-block;
	position: absolute;
	top: 47%;
	left: 3%;
	width: 8%;
	height: 6%;
}

.page_right_button {
	display: inline-block;
	position: absolute;
	top: 47%;
	left: 93%;
	width: 8%;
	height: 6%;
}

input[type=submit] {
    width: 50%;
	height: 100%;
	font-size: 4vh;
}

body, html {
    margin: 0px;
    padding: 0px;
	overflow: hidden;
	background-color: #313131;
    font-family: 'Trebuchet MS';
}
