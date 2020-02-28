<html>
 <head>
  <title>Create Game</title>
  <link rel="stylesheet" type="text/css" href="mtgstyle.css">
	<?php
		// Set Session variables
		session_start();
		if(isset($_SESSION["players"])){ 
			/*if(isset($_POST["player"])){ 
				if(in_array($_POST["player"], $_SESSION["players"])){
				}
				else{
					array_push($_SESSION["players"], $_POST["player"]);
				}
			}*/
		}
		else{
			$_SESSION["players"] = array();
		}
		
        //Load how many Team Entries to display.
        if(isset($_POST['teamcount']))
        {
            $_SESSION["teamcount"] = $_POST['teamcount'];
        }
        else
        {
            $_SESSION["teamcount"] = 2;
        }

		// Setup mysql connection
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
		
		function createGame()
		{	
			// Setup mysql connection
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
			
			//Set the game's "id" value.
			$query = "SELECT COUNT(*) AS c FROM games";
			$result = $conn->query($query);
			$row = mysqli_fetch_assoc($result);
			$game_id = intval($row['c']);
			//Set the game's "name" value.
			$game_name = $_POST['gamename'];
			//Set the game's "game_type" value.
			if(isset($_POST['gtype']))
			{
				$game_type = intval($_POST['gtype']);
			}
			//Set the game's "team size" value.
			if(isset($_POST['tsize']))
			{
				$team_size = intval($_POST['tsize']);
			}
			//Set the game's start time.
			$start_time = date('Y-m-d H:i:s');
			//Create the "teams" table entries.
			$count = $_POST['teamcount'];
            $count = intval($count);
			//Set the starting life (and "emperor") values based on game type.
			switch($game_type)
			{
				//Case 0: Standard
				case 0:
					$startLife = 20;
					$emperor = 0;
					break;
				//Case 1: Commander
				case 1:
					$startLife = 40;
					$emperor = 0;
					break;
				//Case 2: Emperor
				case 2:
					$startLife = 20;
					$emperor = 1;
					break;
				//Case 3: Brawl
				case 3:
					//If there are only two teams, startLife is 25.
					if($count == 2)
					{
						$startLife = 25;
					}
					//Otherwise, startLife is 30.
					else
					{
						$startLife = 30;
					}
					$emperor = 0;
					break;
			}
			//Increase startLife according to the Team Size.
			//(Only do this for non-Emperor games.)
			if($game_type != 2)
			{
				switch($team_size)
				{
					//Case 0: Single
					case 1:
						break;
					//Case 1: Two-Headed Giant
					case 2:
						$startLife = $startLife * 1.5;
						break;
					//Case 2: Three-Headed Giant
					case 3:
						$startLife = ($startLife * 1.5) + 15;
						break;
				}
			}
			$startLife = intval($startLife);
			//Set up each "teams" entry.
			for ($i = 0; $i < $count; $i++)
			{
				//Set the team's "id" value.
				$query = "SELECT COUNT(*) AS c FROM teams";
				$result = $conn->query($query);
				$row = mysqli_fetch_assoc($result);
				$id = intval($row['c']);
				//Get the "id" of this team's leader.
				$player_id = $_POST['leader' + $i];
				$player_id = intval($player_id);
                array_push($_SESSION["players"], $player_id);
                //Set this Team's Name.
                $team_name = $_POST['teamname'];
				//Set the "team_id" using "i".
				$team_id = $i;
				$team_id = intval($team_id);
				//Set "life" to the predetermined starting life value.
				$life = $startLife;
				//Poison will always start at 0, and isAlive will always start at 1.
				$poison = 0;
				$isAlive = 1;
				//Check if this is (at least) a Two-Headed game.
				if($team_size > 1)
				{
					//If this is an Emperor game, give the 1st partner their own life value.
					if($game_type == 2)
					{
						$partner1_life = $startLife;
					}
					//Otherwise, just set it to 0.
					else
					{
						$partner1_life = 0;
					}
					//Set the 1st partner's id to the next value in the list.
					$partner1_id = $_POST['partnerA' + $i];
					$partner1_id = intval($partner1_id);
                    array_push($_SESSION["players"], $partner1_id);
					//Poison will always start at 0.
					$partner1_poison = 0;
				}
				//If this is a Single game, copy the "partner1" values from "player".
				else
				{
					$partner1_id = $player_id;
					$partner1_life = 0;
					$partner1_poison = 0;
				}
				//Check if this is a Three-Headed game.
				if($team_size == 3)
				{
					//If this is an Emperor game, give the 2nd partner their own life value.
					if($game_type == 2)
					{
						$partner2_life = $startLife;
					}
					//Otherwise, just set it to 0.
					else
					{
						$partner2_life = 0;
					}
					//Set the 2nd partner's id to the next value in the list.
					$partner2_id = $_POST['partnerB' + $i];
					$partner2_id = intval($partner2_id);
                    array_push($_SESSION["players"], $partner2_id);
					//Poison will always start at 0.
					$partner2_poison = 0;
				}
				//If this is a Single game, copy the "partner2" values from "player".
				else
				{
					$partner2_id = $player_id;
					$partner2_life = 0;
					$partner2_poison = 0;
				}
				//Add this team to the "teams" table.
				$query = "INSERT INTO teams (`id`, `team_id`, `name`, `game_id`, `life`, `player_id`, `partner1_id`, `partner1_life`, `partner1_poison`, `partner2_id`, `partner2_life`, `partner2_poison`, `emperor`, `poison`, `isAlive`) VALUES ('$id', '$team_id', '$team_name', '$game_id', '$life', '$player_id', '$partner1_id', '$partner1_life', '$partner1_poison', '$partner2_id', '$partner2_life', '$partner2_poison', '$emperor', '$poison', '$isAlive')";
				$conn->query($query);
			}
			//If this is a Commander Game, set up each "commander_dmg" table entry.
			if($game_type == 1)
			{
				//Iterate through every player in this game.
				for ($j = 0; $j < $count * $team_size; $j++)
				{
					//Pair that player against every player, including itself.
					for ($k = 0; $k < $count * $team_size; $k++)
					{
						$query = "SELECT COUNT(*) AS c FROM commander_dmg";
						$result = $conn->query($query);
						$row = mysqli_fetch_assoc($result);
						$cmdrCount = intval($row['c']);
						$from_id = $_SESSION["players"][$j];
						$from_id = intval($from_id);
						$to_id = $_SESSION["players"][$k];
						$to_id = intval($to_id);
						$sql = "INSERT INTO commander_dmg (`id`, `game_id`, `from_id`, `to_id`, `life`) VALUES ('$cmdrCount', '$game_id', '$from_id', '$to_id', '0')";
						$conn->query($sql);
					}
				}
			}
            //Insert the new "games" entry.
            //(Do this *after* inserting the "teams" and "commander_dmg" table data, to prevent the Program from crashing when it finds this "games" table entry.)
			$query = "INSERT INTO games (`id`, `name`, `active`, `team_size`, `game_type`, `start_time`, `turns`) VALUES ('$game_id', '$game_name', '1', '$team_size', '$game_type', '$start_time', '0')";
			$conn->query($query);
			$conn->close();
		}
	?>
 </head>
 <body>
 
	<header>
			<span>Game Builder</span>
	</header>
		
	<nav>
	</nav>
	
	<div class="Container">
		
		<section class="main_section">
			<?php
				$sql = "SELECT id, name FROM players";
				$result = $conn->query($sql);

				if ($result->num_rows > 0) {
					$select= '<form action="/create_game.php" target="/create_game.php" method="post">';
                    //Add the Labels for the 4 basic options.
					$select.= '<span>Game Name (make it unique!)*:</span>';
					$select.= '<span class="teamcount"># of Players/Teams*:</span>';
					$select.= '<span class="teamsize">Team Size*:</span>';
					$select.= '<span class="gametype">Game Type*:</span><br>';
                    //Add the Text Field for the Game Name.
					$select.= '<span><input type="text" name="gamename"></span>';
                    //Add the drop-down list for the number of Teams.
					$select.= '<span class="teamcount"><select name="teamcount">';
					$select.= 	'<option value="2" selected>2</option>';
					$select.= 	'<option value="3">3</option>';
					$select.= 	'<option value="4">4</option>';
					$select.= 	'<option value="5">5</option>';
					$select.= 	'<option value="6">6</option>';
					$select.= 	'<option value="7">7</option>';
					$select.= 	'<option value="8">8</option>';
					$select.= '</select></span>';
                    //Add the Team Size drop down list.
					$select.= '<span class="teamsize"><select name="tsize">';
					$select.= 	'<option value="1">Single</option>';
					$select.= 	'<option value="2">Two-Headed Giant</option>';
					$select.= 	'<option value="3">Three-Headed Giant</option>';
					$select.= '</select></span>';
                    //Add the Game Type drop down list.
					$select.= '<span class="gametype"><select name="gtype">';
					$select.= 	'<option value="0">Standard</option>';
					$select.= 	'<option value="1">Commander</option>';
					$select.= 	'<option value="2">Emperor</option>';
					$select.= 	'<option value="3">Brawl</option>';
					$select.= '</select></span><br>';
                    //Add the Header Text for the Player Selection section.
					$select.= '<p>Select Players/Teams:</p>';
					$select.= '<span>Team Name:</span>';
					$select.= '<span class="teamcount">Leader:</span>';
					$select.= '<span class="teamsize">Partner 1:</span>';
					$select.= '<span class="gametype">Partner 2:</span><br>';
                    //Add input lines for each Team.
					for ($i = 0; $i < $_SESSION["teamcount"]; $i++)
					{
                        //Add a number for this Team entry.
                        $select.= '<span>'.$i.'.</span>';
                        //Add an Entry field for the Name of this Team. 
                        $select.= '<span><input type="text" name="teamname'.$i.'"></span>';
                        //Add a drop-down list for the Team Leader (or the single Player).
                        $select.= '<select name="leader'.$i.'" id="game" class="nameentry1">';
                        while($row = $result->fetch_assoc())
                        {
                            $select.= '<option value="'.$row['id'].'">'.$row['name'].'</option>';
                        }
                        $select.= '</select>';
                        //Reset "$result" to populate the next drop-down.
                        $result = $conn->query($sql);
                        //Add a drop-down list for the 1st Partner.
                        $select.= '<select name="partnerA'.$i.'" id="game" class="nameentry2">';
                        while($row = $result->fetch_assoc())
                        {
                            $select.= '<option value="'.$row['id'].'">'.$row['name'].'</option>';
                        }
                        $select.= '</select>';
                        //Reset "$result" to populate the next drop-down.
                        $result = $conn->query($sql);
                        //Add a drop-down list for the 2nd Partner.
                        $select.= '<select name="partnerB'.$i.'" id="game" class="nameentry3">';
                        while($row = $result->fetch_assoc())
                        {
                            $select.= '<option value="'.$row['id'].'">'.$row['name'].'</option>';
                        }
                        $select.= '</select><br>';
                        //Reset "$result" to populate the next drop-down.
                        $result = $conn->query($sql);
					}
					//$select.='<p><input type="submit" id="join" value="Add Player"></p>';
                    $select.= '<input type="button" id="createP" value="Create Player" onclick="location=\'uploadFiles.php\'" />';
                    $select.= '<input type="submit" id="create" name="createGame" value="Create Game" />';
					$select.='</form>';
					
					echo $select;
				} else {
					echo "No Active Magic Games!";
				}

				$conn->close();
			?>
		</section>
		
		<?php
		/*
		<aside class="side_section">
			<p>Active Players:</p>
			
			<?php
				$players_selected= '';
				foreach ($_SESSION["players"] as &$value) {
					$players_selected.= '<p>'.$value.'</p>';
				}
				unset($value); // break the reference with the last element
				
				echo $players_selected;
			?>
			<form action="create_game.php" method="post">
				<input type="submit" name="clearCommand" value="Clear Players" />
			</form>
			<input type="button" id="createP" value="Create Player" onclick="location='uploadFiles.php'" />
		</aside>
		*/
		?>
	</div>

	<?php
		/*if(isset($_SERVER['REQUEST_METHOD']) and $_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['clearCommand']))
		{
			$_SESSION["players"] = array();
			$_POST['clearCommand'] = null;
			echo '<meta http-equiv="refresh" content="0">';
		}*/
		
		if(isset($_SERVER['REQUEST_METHOD']) and $_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['createGame']))
		{
			createGame();
			$_POST['createGame'] = null;
			echo '<meta http-equiv="refresh" content="1;url=mtg_lobby.php">';
		}

        if(isset($_SESSION["teamcount"]) and isset($_POST['teamcount']) and $_SESSION["teamcount"] != $_POST['teamcount'])
        {
            echo '<meta http-equiv="refresh" content="0">';
        }
	?>
 </body>
</html>




