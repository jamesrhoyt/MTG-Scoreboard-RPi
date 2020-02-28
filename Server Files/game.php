<?php
   $url=$_SERVER['REQUEST_URI'];
   header("Refresh: 5; URL=$url");
?>

<html>
<head>
	<link rel="stylesheet" href="game_style.php" media="screen">
	<?php
		// Set Session variables
		session_start();
		if(isset($_POST["game"])){ //first time
			$_SESSION['game'] = $_POST["game"];
		}
		
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

		$sql = "SELECT name FROM games Where id =".$_SESSION['game'];
		$result = $conn->query($sql);

		if ($result->num_rows == 1) {
			// output data of each row
			while($row = $result->fetch_assoc()) {
				// echo "Welcome to ".$row['name'];
				$gameName = $row['name'];
			}
		} else {
			echo "Couldn't Find Magic Game!";
		}
		
		//Indices for which Commander counters to load, per player.
		if(isset($_SESSION["cmdrIndices"]))
		{
		}
		else
		{
			$_SESSION["cmdrIndices"] = array();
			$count = "SELECT COUNT(*) AS c FROM teams WHERE game_id=".$_SESSION['game'];
			$result2 = $conn->query($count);
			$row2 = mysqli_fetch_assoc($result);
			$numplayers = intval($row2['c']);
			for($k = 0; $k < $numplayers; $k++)
			{
				$_SESSION["cmdrIndices"][$k] = 0;
			}
		}
		
		//Value for which "page" of Player widgets to load.
		if(isset($_SESSION["pageNumber"]))
		{
		}
		else
		{
			$_SESSION["pageNumber"] = 0;
		}
		
		$conn->close();
	?>
	<title><?php echo $gameName; ?></title>
</head>

<body>
<!-- Welcome <?php echo $_POST["game"]; ?><br> 
-->

<?php
/*
$width = "<script type=\"text/javascript\"> document.write(window.innerWidth);; </script>";
echo "width = ".$width;
$_SESSION['width'] = $width;
 
$height = "<script type=\"text/javascript\">document.write(window.innerHeight);;</script>";
$_SESSION['height'] = $height;
echo  "height = ".$height;
*/
?>

<?php	
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

	$sql = "SELECT id, life, poison, player_id, name, partner1_id, partner1_life, partner1_poison, partner2_id, partner2_life, partner2_poison FROM teams Where game_id =".$_SESSION['game'];
	$result = $conn->query($sql);

	if ($result->num_rows > 0) {
		// Get game metadata
		$sq3 = "SELECT game_type, team_size FROM games WHERE id=".$_SESSION['game'];
		$gameResult = $conn->query($sq3);
		$gameInfo = $gameResult->fetch_assoc();
		// output data of each row
		$posID = 0;
		while($row = $result->fetch_assoc()) {
			// echo "Welcome to ".$row['id'];
			$sq2 = "SELECT name, avatar FROM players Where id =".$row['player_id'];
			$playerInfo = $conn->query($sq2);
			
			if ($playerInfo->num_rows == 1) {
				while($info = $playerInfo->fetch_assoc()) {
					//Create the div for each visible Player.
					$display = '<div id="player_'.$posID.'">';
					//Create the form that enables the inputs.
					$display .= '<form action="game.php" method="post">';
					//Display the Player's avatar and name.
					if($gameInfo['game_type'] != 2)
					{
						$display .=  '<div class="player_life" id="name_'.$posID.'">';
					}
					else
					{
						$display .=  '<div class="player_name_emp_1" id="name_'.$posID.'">';
					}
					$display .=   '<img src="'.$info['avatar'].'" id="image_'.$posID.'" alt="Picture Missing!" onerror="this.src=\'player_images/default.jpg\';" style="width:100px;height:100px;">';
					//If this is a Single game (or an Emperor game), use the Player's name.
					if($gameInfo['team_size'] == 1 || $gameInfo['game_type'] == 2)
					{
						$display .=   ' '.$info['name'].'';
					}
					//Otherwise, use the Team's name.
					else
					{
						$display .=   ' '.$row['name'].'';
					}
					$display .=  '</div>';
					//Display the Player's life value.
					if($gameInfo['game_type'] != 2)
					{
						$display .=  '<div class="life_new" id="life_'.$posID.'">';
					}
					else
					{
						$display .= '<div class="player_life_emp_1" id="life_'.$posID.'">';
					}
					$display .=   $row['life'];
					$display .=  '</div>';
					//Save the Player's ID value, to be used by the inputs.
					$display .=  '<input type="hidden" name="playerId" value="'.$row['player_id'].'">';
					//Save the Game Type value, to use in the positioning code.
					$display .=  '<input type="hidden" id="gameType" value="'.$gameInfo['game_type'].'">';
					//Save the Player's current "commander" index, to be used by the "cmdr" inputs.
					$display .=  '<input type="hidden" name="cmdrIndex" value="'.$_SESSION["cmdrIndices"][$posID].'">';
					//Save the Player's current index in the game, to use when scrolling their Commander counters.
					$display .=  '<input type="hidden" name="playerIndex" value="'.$posID.'">';
					//Display the Player's "+" Life button.
					if($gameInfo['game_type'] != 2)
					{
						$display .=  '<div class="player_addlife_button">';
					}
					else
					{
						$display .= '<div class="player_addlife_emp_1">';
					}
					$display .=   '<input type="submit" name="AddValue" value="+" />';
					$display .=  '</div>';
					//Display the Player's "-" Life button.
					if($gameInfo['game_type'] != 2)
					{
						$display .=  '<div class="player_sublife_button">';
					}
					else
					{
						$display .= '<div class="player_sublife_emp_1">';
					}
					$display .=   '<input type="submit" name="SubValue" value="-" />';
					$display .=  '</div>';
					//Display the Player's poison value.
					//If this is a Commander game, use the "alternate" CSS positioning.
					if($gameInfo['game_type'] == 1)
					{
						$display .=  '<div class="player_poison_alt" id="poison_'.$posID.'">';
					}
					//If this is an Emperor game, use the "emperor" CSS positioning.
					else if ($gameInfo['game_type'] == 2)
					{
						$display .= '<div class="player_poison_emp_1" id="poison_'.$posID.'">';
					}
					//Otherwise, use the "original" CSS positioning.
					else
					{
						$display .=  '<div class="player_poison" id="poison_'.$posID.'">';
					}
					$display .=   $row['poison'];
					$display .=  '</div>';
					//Display the Player's "+" Poison button.
					//If this is a Commander game, use the "alternate" CSS positioning.
					if($gameInfo['game_type'] == 1)
					{
						$display .=  '<div class="player_addpoison_button_alt">';
					}
					//If this is an Emperor game, use the "emperor" CSS positioning.
					else if ($gameInfo['game_type'] == 2)
					{
						$display .=  '<div class="player_addpoison_emp_1">';
					}
					//Otherwise, use the "original" CSS positioning.
					else
					{
						$display .=  '<div class="player_addpoison_button">';
					}
					$display .=   '<input type="submit" name="AddPoison" value="+" />';
					$display .=  '</div>';
					//Display the Player's "-" Poison button.
					//If this is a Commander game, use the "alternate" CSS positioning.
					if($gameInfo['game_type'] == 1)
					{
						$display .=  '<div class="player_subpoison_button_alt">';
					}
					//If this is an Emperor game, use the "emperor" CSS positioning.
					else if ($gameInfo['game_type'] == 2)
					{
						$display .=  '<div class="player_subpoison_emp_1">';
					}
					//Otherwise, use the "original" CSS positioning.
					else
					{
						$display .=  '<div class="player_subpoison_button">';
					}
					$display .=   '<input type="submit" name="SubPoison" value="-" />';
					$display .=  '</div>';
					//If this is an Emperor game with a team size of 2 or greater, display the 1st partner's info.
					if($gameInfo['game_type'] == 2 && $gameInfo['team_size'] >= 2)
					{
						//Get the 1st Partner's info.
						$sqp1 = "SELECT name, avatar FROM players Where id =".$row['partner1_id'];
						$partnerInfo1 = $conn->query($sqp1);
						$pInfo1 = $partnerInfo1->fetch_assoc();
						//Save the 1st Partner's ID value for the inputs to use.
						$display .= '<input type="hidden" name="partner1Id" value="'.$row['partner1_id'].'">';
						//Set the 1st Partner's Name and Avatar.
						$display .= '<div class="player_name_emp_2" id="nameP1_'.$posID.'">';
						$display .=  '<img src="'.$pInfo1['avatar'].'" id="imageP1_'.$posID.'" onerror="this.src=\'player_images/default.jpg\';" style="width:100px;height:100px;">';
						$display .=  ''.$pInfo1['name'].'';
						$display .= '</div>';
						//Set the 1st Partner's Life Counter.
						$display .= '<div class="player_life_emp_2" id="lifeP1_'.$posID.'">';
						$display .=  ''.$row['partner1_life'].'';
						$display .= '</div>';
						//Set the 1st Partner's AddLife Button.
						$display .= '<div class="player_addlife_emp_2">';
						$display .=  '<input type="submit" name="AddLifePartner1" value="+" />';
						$display .= '</div>';
						//Set the 1st Partner's SubLife Button.
						$display .= '<div class="player_sublife_emp_2">';
						$display .=  '<input type="submit" name="SubLifePartner1" value="-" />';
						$display .= '</div>';
						//Set the 1st Partner's Poison Counter.
						$display .= '<div class="player_poison_emp_2" id="poisonP1_'.$posID.'">';
						$display .=  ''.$row['partner1_poison'].'';
						$display .= '</div>';
						//Set the 1st Partner's AddPoison Button.
						$display .= '<div class="player_addpoison_emp_2">';
						$display .=  '<input type="submit" name="AddPoisonPartner1" value="+" />';
						$display .= '</div>';
						//Set the 1st Partner's SubPoison Button.
						$display .= '<div class="player_subpoison_emp_2">';
						$display .=  '<input type="submit" name="SubPoisonPartner1" value="-" />';
						$display .= '</div>';
					}
					//If this is an Emperor game with a team size of 3, display the 2nd partner's info.
					if($gameInfo['game_type'] == 2 && $gameInfo['team_size'] == 3)
					{
						//Get the 2nd Partner's info.
						$sqp2 = "SELECT name, avatar FROM players Where id =".$row['partner2_id'];
						$partnerInfo2 = $conn->query($sqp2);
						$pInfo2 = $partnerInfo2->fetch_assoc();
						//Save the 2nd Partner's ID value for the inputs to use.
						$display .= '<input type="hidden" name="partner2Id" value="'.$row['partner2_id'].'">';
						//Set the 2nd Partner's Name and Avatar.
						$display .= '<div class="player_name_emp_3" id="nameP2_'.$posID.'">';
						$display .=  '<img src="'.$pInfo2['avatar'].'" id="imageP2_'.$posID.'" onerror="this.src=\'player_images/default.jpg\';" style="width:100px;height:100px;">';
						$display .=  ''.$pInfo2['name'].'';
						$display .= '</div>';
						//Set the 2nd Partner's Life Counter.
						$display .= '<div class="player_life_emp_3" id="lifeP2_'.$posID.'">';
						$display .=  ''.$row['partner2_life'].'';
						$display .= '</div>';
						//Set the 2nd Partner's AddLife Button.
						$display .= '<div class="player_addlife_emp_3">';
						$display .=  '<input type="submit" name="AddLifePartner2" value="+" />';
						$display .= '</div>';
						//Set the 2nd Partner's SubLife Button.
						$display .= '<div class="player_sublife_emp_3">';
						$display .=  '<input type="submit" name="SubLifePartner2" value="-" />';
						$display .= '</div>';
						//Set the 2nd Partner's Poison Counter.
						$display .= '<div class="player_poison_emp_3" id="poisonP2_'.$posID.'">';
						$display .=  ''.$row['partner2_poison'].'';
						$display .= '</div>';
						//Set the 2nd Partner's AddPoison Button.
						$display .= '<div class="player_addpoison_emp_3">';
						$display .=  '<input type="submit" name="AddPoisonPartner2" value="+" />';
						$display .= '</div>';
						//Set the 2nd Partner's SubPoison Button.
						$display .= '<div class="player_subpoison_emp_3">';
						$display .=  '<input type="submit" name="SubPoisonPartner2" value="-" />';
						$display .= '</div>';
					}
					//If this is a Commander game, create the Commander counters.
					if($gameInfo['game_type'] == 1)
					{
						$cmdrArray = array();
						$index = 0;
						$sqcmdr = "SELECT to_id, life FROM commander_dmg WHERE from_id=".$row['player_id']." AND game_id=".$_SESSION['game']." ORDER BY id ASC";
						$cmdrInfo = $conn->query($sqcmdr);
						while ($cmdrResult = $cmdrInfo->fetch_assoc())
						{
							$cmdrArray[$index] = $cmdrResult;
							$index++;
						}
						$display .= '<div class="player_cmdr_scroll_left">';
						$display .=  '<input type="submit" name="CmdrLeft" value="<" />';
						$display .= '</div>';
						for ($i = 1; $i <= 3; $i++)
						{
							$display .= '<div class="player_commander_'.$i.'">';
							$display .= ''.$cmdrArray[($_SESSION["cmdrIndices"][$posID] + ($i-1)) % $result->num_rows]["life"].'';
							$display .= '</div>';
							$display .= '<div class="player_addcmdr_button_'.$i.'">';
							$display .=  '<input type="submit" name="CmdrAdd'.$i.'" value="+" />';
							$display .= '</div>';
							$display .= '<div class="player_subcmdr_button_'.$i.'">';
							$display .=  '<input type="submit" name="CmdrSub'.$i.'" value="-" />';
							$display .= '</div>';
							$display .= '<div class="player_cmdr_avatar_'.$i.'">';
							$display .=  '<img src="player_images/'.$cmdrArray[($_SESSION["cmdrIndices"][$posID] + ($i-1)) % $result->num_rows]["to_id"].'.jpg" onerror="this.src=\'player_images/default.jpg\';" style="width:20px;height:26px;">';
							$display .= '</div>';
						}
						$display .= '<div class="player_cmdr_scroll_right">';
						$display .=  '<input type="submit" name="CmdrRight" value=">" />';
						$display .= '</div>';
					}
					//Close the Player's Form.
					$display .= '</form>';
					//Close the "player" div.
					$display .='</div>';
					//Display this div.
					echo $display;
					//Increment the counter for the next Player.
					$posID++;
				}
			} else {
				echo "Bad Player Info!";
			}
		}
		$display = '<form action="game.php" method="post">';
		$display .= '<input type="hidden" id="pageNumber" value="'.$_SESSION["pageNumber"].'">';
		if($_SESSION["pageNumber"] > 0)
		{
			$display .= '<div class="page_left_button">';
			$display .= '<input type="submit" name="PageLeft" value="<" />';
			$display .= '</div>';
		}
		if(($_SESSION["pageNumber"] == 0 and $posID > 4) or ($_SESSION["pageNumber"] == 1 and $posID > 8))
		{
			$display .= '<div class="page_right_button">';
			$display .= '<input type="submit" name="PageRight" value=">" />';
			$display .= '</div>';
		}
		$display .= '</form>';
		echo $display;
	} else {
		echo "Couldn't Find Players!";
	}
	$conn->close();
	?>

<?php
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['AddValue']))
    {
        addLife();
		$_POST['AddValue'] = null;
		echo '<meta http-equiv="refresh" content="0">';
    }
    function addLife()
    {
        echo "+ Button Pressed for player ".$_POST['playerId'];   
		
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
		
		$sql = "UPDATE teams SET life = life + 1 Where player_id =".$_POST['playerId'];
		$result = $conn->query($sql);
		
		$conn->close();
    }
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['SubValue']))
    {
        subLife();
		$_POST['SubValue'] = null;
		echo '<meta http-equiv="refresh" content="0">';
    }
    function subLife()
    {
        echo "- Button Pressed for player ".$_POST['playerId'];  

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
		
		$sql = "UPDATE teams SET life = life - 1 Where player_id =".$_POST['playerId'];
		$result = $conn->query($sql);
		
		$conn->close();
    }
    if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['AddPoison']))
    {
        addPoison();
		$_POST['AddPoison'] = null;
		echo '<meta http-equiv="refresh" content="0">';
    }
    function addPoison()
    {
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
		
		$sql = "UPDATE teams SET poison = poison + 1 Where player_id =".$_POST['playerId'];
		$result = $conn->query($sql);
		
		$conn->close();
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['SubPoison']))
    {
        subPoison();
		$_POST['SubPoison'] = null;
		echo '<meta http-equiv="refresh" content="0">';
    }
    function subPoison()
	{
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
		
		$sql = "UPDATE teams SET poison = poison - 1 Where player_id =".$_POST['playerId'];
		$result = $conn->query($sql);
		
		$conn->close();
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['CmdrAdd1']))
	{
		addCmdr(0);
		$_POST['CmdrAdd1'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['CmdrAdd2']))
	{
		addCmdr(1);
		$_POST['CmdrAdd2'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['CmdrAdd3']))
	{
		addCmdr(2);
		$_POST['CmdrAdd3'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	function addCmdr($cmdrOffset)
	{
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
		
		$sqltoid = "SELECT to_id FROM commander_dmg WHERE from_id=".$_POST['playerId']." AND game_id=".$_SESSION['game']." ORDER BY id ASC";
		$toidresult = $conn->query($sqltoid);
		$toidarray = array();
		$index = 0;
		while($toidinfo = $toidresult->fetch_assoc())
		{
			$toidarray[$index] = $toidinfo['to_id'];
			$index++;
		}
		
		$sql = "UPDATE commander_dmg SET life = life + 1 WHERE game_id=".$_SESSION['game']." AND from_id=".$_POST['playerId']." AND to_id=".$toidarray[($_POST['cmdrIndex'] + $cmdrOffset) % count($toidarray)]."";
		$result = $conn->query($sql);
		
		$conn->close();
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['CmdrSub1']))
	{
		subCmdr(0);
		$_POST['CmdrSub1'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['CmdrSub2']))
	{
		subCmdr(1);
		$_POST['CmdrSub2'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['CmdrSub3']))
	{
		subCmdr(2);
		$_POST['CmdrSub3'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	function subCmdr($cmdrOffset)
	{
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
		
		$sqltoid = "SELECT to_id FROM commander_dmg WHERE from_id=".$_POST['playerId']." AND game_id=".$_SESSION['game']." ORDER BY id ASC";
		$toidresult = $conn->query($sqltoid);
		$toidarray = array();
		$index = 0;
		while($toidinfo = $toidresult->fetch_assoc())
		{
			$toidarray[$index] = $toidinfo['to_id'];
			$index++;
		}
		
		$sql = "UPDATE commander_dmg SET life = life - 1 WHERE game_id=".$_SESSION['game']." AND from_id=".$_POST['playerId']." AND to_id=".$toidarray[($_POST['cmdrIndex'] + $cmdrOffset) % count($toidarray)]."";
		$result = $conn->query($sql);
		
		$conn->close();
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['CmdrLeft']))
	{
		scrollCmdr(-1);
		$_POST['CmdrLeft'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['CmdrRight']))
	{
		scrollCmdr(1);
		$_POST['CmdrRight'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	function scrollCmdr($scrollValue)
	{
		$_SESSION["cmdrIndices"][$_POST['playerIndex']] += $scrollValue;
		if($_SESSION["cmdrIndices"][$_POST['playerIndex']] < 0)
		{
			$_SESSION["cmdrIndices"][$_POST['playerIndex']] = count($_SESSION["cmdrIndices"]);
		}
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['PageLeft']))
	{
		scrollPage(-1);
		$_POST['PageLeft'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['PageRight']))
	{
		scrollPage(1);
		$_POST['PageRight'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	function scrollPage($pageValue)
	{
		$_SESSION["pageNumber"] += $pageValue;
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['AddLifePartner1']))
	{
		addLifePartner(1);
		$_POST['AddLifePartner1'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['AddLifePartner2']))
	{
		addLifePartner(2);
		$_POST['AddLifePartner2'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	function addLifePartner($partnerValue)
	{
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
		
		//If "partnerValue" is 1, increase the 1st partner's life.
		if($partnerValue == 1)
		{
			$sql = "UPDATE teams SET partner1_life = partner1_life + 1 WHERE game_id=".$_SESSION['game']." AND partner1_id=".$_POST['partner1Id'];
		}
		//If "partnerValue" is 2, increase the 2nd partner's life.
		else if ($partnerValue == 2)
		{
			$sql = "UPDATE teams SET partner2_life = partner2_life + 1 WHERE game_id=".$_SESSION['game']." AND partner2_id=".$_POST['partner2Id'];
		}
		$result = $conn->query($sql);
		
		$conn->close();
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['SubLifePartner1']))
	{
		subLifePartner(1);
		$_POST['SubLifePartner1'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['SubLifePartner2']))
	{
		subLifePartner(2);
		$_POST['SubLifePartner2'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	function subLifePartner($partnerValue)
	{
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
		
		//If "partnerValue" is 1, decrease the 1st partner's life.
		if($partnerValue == 1)
		{
			$sql = "UPDATE teams SET partner1_life = partner1_life - 1 WHERE game_id=".$_SESSION['game']." AND partner1_id=".$_POST['partner1Id'];
		}
		//If "partnerValue" is 2, decrease the 2nd partner's life.
		else if ($partnerValue == 2)
		{
			$sql = "UPDATE teams SET partner2_life = partner2_life - 1 WHERE game_id=".$_SESSION['game']." AND partner2_id=".$_POST['partner2Id'];
		}
		$result = $conn->query($sql);
		
		$conn->close();
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['AddPoisonPartner1']))
	{
		addPoisonPartner(1);
		$_POST['AddPoisonPartner1'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['AddPoisonPartner2']))
	{
		addPoisonPartner(2);
		$_POST['AddPoisonPartner2'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	function addPoisonPartner($partnerValue)
	{
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
		
		//If "partnerValue" is 1, increase the 1st partner's poison.
		if($partnerValue == 1)
		{
			$sql = "UPDATE teams SET partner1_poison = partner1_poison + 1 WHERE game_id=".$_SESSION['game']." AND partner1_id=".$_POST['partner1Id'];
		}
		//If "partnerValue" is 2, increase the 2nd partner's poison.
		else if ($partnerValue == 2)
		{
			$sql = "UPDATE teams SET partner2_poison = partner2_poison + 1 WHERE game_id=".$_SESSION['game']." AND partner2_id=".$_POST['partner2Id'];
		}
		$result = $conn->query($sql);
		
		$conn->close();
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['SubPoisonPartner1']))
	{
		subPoisonPartner(1);
		$_POST['SubPoisonPartner1'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['SubPoisonPartner2']))
	{
		subPoisonPartner(2);
		$_POST['SubPoisonPartner2'] = null;
		echo '<meta http-equiv="refresh" content="0">';
	}
	function subPoisonPartner($partnerValue)
	{
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
		
		//If "partnerValue" is 1, decrease the 1st partner's poison.
		if($partnerValue == 1)
		{
			$sql = "UPDATE teams SET partner1_poison = partner1_poison - 1 WHERE game_id=".$_SESSION['game']." AND partner1_id=".$_POST['partner1Id'];
		}
		//If "partnerValue" is 2, decrease the 2nd partner's poison.
		else if ($partnerValue == 2)
		{
			$sql = "UPDATE teams SET partner2_poison = partner2_poison - 1 WHERE game_id=".$_SESSION['game']." AND partner2_id=".$_POST['partner2Id'];
		}
		$result = $conn->query($sql);
		
		$conn->close();
	}
?>
<script>
	var borderSize = 10;
	var life_width = (window.innerHeight/3);
	var i;
	var start = 4 * document.getElementById("pageNumber").getAttribute("value");
	var gameType = document.getElementById("gameType").getAttribute("value");
	for(i = start; i < (start + 4); i++)
	{
		var player = document.getElementById("player_" + i);
		player.style.top = (window.innerHeight/4) * (i - start);
		player.style.left = 0;
		player.style.borderWidth = "10px";
		player.style.height = (window.innerHeight/4)-borderSize;
		player.style.width = window.innerWidth-borderSize;
		
		var image = document.getElementById("image_" + i);
		image.style.height = (window.innerHeight/24);
		image.style.width = (window.innerHeight/24);
		image.style.top = player.style.top + (player.style.height / 12);
		image.style.left = player.style.left + (player.style.width * .22);
		
		var elem = document.getElementById("name_" + i);
		elem.style.fontSize = (window.innerHeight / 12);
		if(gameType == 2)
		{
			elem.style.width = (window.innerWidth / 4);
		}
		else
		{
			elem.style.width = (window.innerWidth / 2);
		}
		elem.style.cssFloat = 'center';
		
		var life = document.getElementById("life_" + i);
		life.style.fontSize = (window.innerHeight / 12);
		life.style.top = player.style.top + (window.innerHeight / 12);
		life.style.left =  player.style.left + (window.innerWidth / 3);
		
		life.style.height = (window.innerHeight / 6);
		life.style.width = life_width;
		
		/*
		life.style.borderColor = "#000";
		life.style.borderStyle = "solid";
		life.style.borderWidth = "10px";
		*/
		life.style.cssFloat = 'center';
		// alert( window.innerWidth-(2*borderSize)-life_width);
		
		var poison = document.getElementById("poison_" + i);
		poison.style.fontSize = (window.innerHeight / 16);
		poison.style.top = player.style.top + (window.innerHeight * .185);
		poison.style.left = player.style.left + (window.innerWidth / 2);
		poison.style.height = (window.innerHeight / 16);
		poison.style.width = life_width;
		poison.style.cssFloat = 'center';
		
		if(gameType == 2)
		{
			//Set the Partner1 values (emperor-only)
			var imageP1 = document.getElementById("imageP1_" + i);
			imageP1.style.height = (window.innerHeight/24);
			imageP1.style.width = (window.innerHeight/24);
			imageP1.style.top = player.style.top + (player.style.height / 12);
			imageP1.style.left = image.style.left - (player.style.width * .9);
			
			var elemP1 = document.getElementById("nameP1_" + i);
			elemP1.style.fontSize = (window.innerHeight / 24);
			elemP1.style.width = (window.innerWidth / 4);
			elemP1.style.cssFloat = 'left';
			
			var lifeP1 = document.getElementById("lifeP1_" + i);
			lifeP1.style.fontSize = (window.innerHeight / 12);
			lifeP1.style.top = player.style.top + (window.innerHeight / 12);
			lifeP1.style.left = life.style.left - (player.style.width * .3);
			lifeP1.style.height = (window.innerHeight / 12);
			lifeP1.style.width = life_width;
			lifeP1.style.cssFloat = 'center';
			
			var poisonP1 = document.getElementById("poisonP1_" + i);
			poisonP1.style.fontSize = (window.innerHeight / 16);
			poisonP1.style.top = player.style.top + (window.innerHeight * .185);
			poisonP1.style.left = poison.style.left - (player.style.width * .3);
			poisonP1.style.height = (window.innerHeight / 16);
			poisonP1.style.width = life_width;
			poisonP1.style.cssFloat = 'center';
			
			//Set the Partner2 values (emperor-only)
			var imageP2 = document.getElementById("imageP2_" + i);
			imageP2.style.height = (window.innerHeight/24);
			imageP2.style.width = (window.innerHeight/24);
			imageP2.style.top = player.style.top + (player.style.height / 12);
			imageP2.style.left = image.style.left + (player.style.width * .9);
			
			var elemP2 = document.getElementById("nameP2_" + i);
			elemP2.style.fontSize = (window.innerHeight / 24);
			elemP2.style.width = (window.innerWidth / 4);
			elemP2.style.cssFloat = 'right';
			
			var lifeP2 = document.getElementById("lifeP2_" + i);
			lifeP2.style.fontSize = (window.innerHeight / 12);
			lifeP2.style.top = player.style.top + (window.innerHeight / 12);
			lifeP2.style.left = life.style.left + (player.style.width * .3);
			lifeP2.style.height = (window.innerHeight / 12);
			lifeP2.style.width = life_width;
			lifeP2.style.cssFloat = 'center';
			
			var poisonP2 = document.getElementById("poisonP2_" + i);
			poisonP2.style.fontSize = (window.innerHeight / 16);
			poisonP2.style.top = player.style.top + (window.innerHeight * .185);
			poisonP2.style.left = poison.style.left + (player.style.width * .3);
			poisonP2.style.height = (window.innerHeight / 16);
			poisonP2.style.width = life_width;
			poisonP2.style.cssFloat = 'center';
		}
	}
	var j;
	for (j = 0; j < 10; j++)
	{
		var player = document.getElementById("player_" + j);
		if(j < start || j >= (start + 4))
		{
			player.style.display = "none";
		}
	}
</script>
</body>
</html>

