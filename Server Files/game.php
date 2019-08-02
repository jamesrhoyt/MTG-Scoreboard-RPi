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
					$display .=  '<div class="player_life" id="name_'.$posID.'">';
					$display .=   '<img src="'.$info['avatar'].'" id="image_'.$posID.'" alt="Picture Missing!" onerror="this.src=\'player_images/default.jpg\';" style="width:100px;height:100px;">';
					//If this is a Single game, use the Player's name.
					if($gameInfo['team_size'] == 1)
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
					$display .=  '<div class="life_new" id="life_'.$posID.'">';
					$display .=   $row['life'];
					$display .=  '</div>';
					//Save the Player's ID value, to be used by the inputs.
					$display .=  '<input type="hidden" name="playerId" value="'.$row['id'].'">';
					//Save the Player's current "commander" index, to be used by the "cmdr" inputs.
					$display .=  '<input type="hidden" name="cmdrIndex" value="'.$_SESSION["cmdrIndices"][$posID].'">';
					//Save the Player's current index in the game, to use when scrolling their Commander counters.
					$display .=  '<input type="hidden" name="playerIndex" value="'.$posID.'">';
					//Display the Player's "+" Life button.
					$display .=  '<div class="player_addlife_button">';
					$display .=   '<input type="submit" name="AddValue" value="+" />';
					$display .=  '</div>';
					//Display the Player's "-" Life button.
					$display .=  '<div class="player_sublife_button">';
					$display .=   '<input type="submit" name="SubValue" value="-" />';
					$display .=  '</div>';
					//Display the Player's poison value.
					//If this isn't a Commander game, use the "original" CSS positioning.
					if($gameInfo['game_type'] != 1)
					{
						$display .=  '<div class="player_poison" id="poison_'.$posID.'">';
					}
					//Otherwise, use the "alternate" CSS positioning.
					else
					{
						$display .=  '<div class="player_poison_alt" id="poison_'.$posID.'">';
					}
					$display .=   $row['poison'];
					$display .=  '</div>';
					//Display the Player's "+" Poison button.
					//If this isn't a Commander game, use the "original" CSS positioning.
					if($gameInfo['game_type'] != 1)
					{
						$display .=  '<div class="player_addpoison_button">';
					}
					//Otherwise, use the "alternate" CSS positioning.
					else
					{
						$display .=  '<div class="player_addpoison_button_alt">';
					}
					$display .=   '<input type="submit" name="AddPoison" value="+" />';
					$display .=  '</div>';
					//Display the Player's "-" Poison button.
					//If this isn't a Commander game, use the "original" CSS positioning.
					if($gameInfo['game_type'] != 1)
					{
						$display .=  '<div class="player_subpoison_button">';
					}
					//Otherwise, use the "alternate" CSS positioning.
					else
					{
						$display .=  '<div class="player_subpoison_button_alt">';
					}
					$display .=   '<input type="submit" name="SubPoison" value="-" />';
					$display .=  '</div>';
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
		
		$sql = "UPDATE teams SET life = life + 1 Where id =".$_POST['playerId'];
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
		
		$sql = "UPDATE teams SET life = life - 1 Where id =".$_POST['playerId'];
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
		
		$sql = "UPDATE teams SET poison = poison + 1 Where id =".$_POST['playerId'];
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
		
		$sql = "UPDATE teams SET poison = poison - 1 Where id =".$_POST['playerId'];
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
?>
<script>
	var borderSize = 10;
	var life_width = (window.innerHeight/6);
	var i;
	var start = 4 * document.getElementById("pageNumber").getAttribute("value");
	for(i = start; i < (start + 4); i++)
	{
		var player = document.getElementById("player_" + i);
		player.style.top = (window.innerHeight/2) * Math.floor((i - start)/ 2);
		player.style.left = (window.innerWidth/2) * (i % 2);
		player.style.borderWidth = "10px";
		player.style.height = (window.innerHeight/2)-borderSize;
		player.style.width = (window.innerWidth/2)-(2*borderSize);
		
		var image = document.getElementById("image_" + i);
		image.style.height = (window.innerHeight/12);
		image.style.width = (window.innerHeight/12);
		image.style.top = player.style.top + (player.style.height / 12);
		image.style.left = player.style.left + (player.style.width * .22);
		
		var elem = document.getElementById("name_" + i);
		elem.style.fontSize = (window.innerHeight / 12);
		elem.style.width = (window.innerWidth / 2);
		elem.style.cssFloat = 'left';
		
		var life = document.getElementById("life_" + i);
		life.style.fontSize = (window.innerHeight / 6);
		life.style.top = player.style.top + (window.innerHeight / 6);
		life.style.left =  player.style.left + (window.innerWidth / 6);
		
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
		poison.style.fontSize = (window.innerHeight / 8);
		poison.style.top = player.style.top + (window.innerHeight * .37);
		poison.style.left = player.style.left + (window.innerWidth / 4);
		poison.style.height = (window.innerHeight / 8);
		poison.style.width = life_width;
		poison.style.cssFloat = 'center';
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

