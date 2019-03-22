<html>
 <head>
  <title>Create Game</title>
  <link rel="stylesheet" type="text/css" href="mtgstyle.css">
  
	<?php
		// Set Session variables
		session_start();
		if(isset($_SESSION["players"])){ 
			if(isset($_POST["player"])){ 
				if(in_array($_POST["player"], $_SESSION["players"])){
				}
				else{
					array_push($_SESSION["players"], $_POST["player"]);
				}
			}
		}
		else{
			$_SESSION["players"] = array();
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
					$select.= '<p>Choose Players:</p>';
					$select.= '<select name="player" id="game" multiple>';
					// output data of each row
					while($row = $result->fetch_assoc()) {
						$select.='<option value="'.$row['id'].'">'.$row['name'].'</option>';
					}
					
					$select.='</select>';
					$select.='<p><input type="submit" id="join" value="Add Player"></p>';
					$select.='</form>';
					
					echo $select;
				} else {
					echo "No Active Magic Games!";
				}

				$conn->close();
			?>
		</section>
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
			<input type="button" id="createP" value="Create Player" onclick="location='mtg_lobby.php'" />
		</aside>
	</div>
	
	<input type="button" id="create" name="createGame" value="Create Game" onclick="location='mtg_lobby.php'" />
	
	<p>Game Type:</p>
	<form>
	  <input type="radio" name="gtype" value="s" checked> Standard<br>
	  <input type="radio" name="gtype" value="c"> Commander (EDH)<br>
	  <input type="radio" name="gtype" value="e"> Emperor<br>
	  <input type="radio" name="gtype" value="b"> Brawl
	</form>
	
	<p>Team Size:</p>
	<form>
	  <input type="radio" name="tsize" value="1" checked> Single<br>
	  <input type="radio" name="tsize" value="2"> Two-Headed Giant<br>
	  <input type="radio" name="tsize" value="3"> Three-Headed Giant
	</form>
	
	<?php
		if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['clearCommand']))
		{
			$_SESSION["players"] = array();
			$_POST['clearCommand'] = null;
			echo '<meta http-equiv="refresh" content="0">';
		}
		
		if($_SERVER['REQUEST_METHOD'] == "POST" and isset($_POST['createGame']))
		{
			createGame();
			$_POST['createGame'] = null;
		}
		
		function createGame()
		{
			//Set the game's "id" value.
			$sql = "SELECT COUNT(*) AS count FROM games";
			$result = $conn->query($sql);
			$game_id = $result['count'];
			
			//Set the game's "game_type" value.
			if(isset($_POST['gtype']))
			{
				switch($_POST['gtype'])
				{
					//Case 0: Standard
					case "s":
						$game_type = 0;
						break;
					//Case 1: Commander
					case "c":
						$game_type = 1;
						break;
					//Case 2: Emperor
					case "e":
						$game_type = 2;
						break;
					//Case 3: Brawl
					case "b":
						$game_type = 3;
						break;
				}
			}
			//Set the game's "team size" value.
			if(isset($_POST['tsize']))
			{
				$team_size = $_POST['tsize'];
			}
			//Insert the new "games" entry.
			$sql = "INSERT INTO games (id, name, active, team_size, game_type, turns) 
			VALUES ('$game_id', '', '1', '$team_size', '$game_type', '0')";
			$conn->query($sql);
			//Create the "teams" table entries.
			$count = count($_SESSION["players"]);
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
					if($count/$team_size == 2)
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
			switch($team_size)
			{
				//Case 0: Single
				case 0:
					break;
				//Case 1: Two-Headed Giant
				case 1:
					$startLife = ($startLife / 3) * 2;
					break;
				//Case 2: Three-Headed Giant
				case 2:
					$startLife = (($startLife / 3) * 2) + 15;
					break;
			}
			//Set up each "teams" entry.
			for ($i = 0; $i < $count; $i++)
			{
				//Set the team's "id" value.
				$sql = "SELECT COUNT(*) AS count FROM teams";
				$result = $conn->query($sql);
				$id = $result['count'];
				//Get the "id" of this team's leader.
				$player_id = $_SESSION["players"][$i];
				//Set the "team_id" using "i" and "team_size"
				//(e.g., i=4 divided by a team size of 2 means a team_id of 2.)
				$team_id = $i / $team_size;
				//Set "life" to the predetermined "starting life value.
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
					$partner1_id = $_SESSION["players"][$i + 1];
					//Poison will always start at 0.
					$partner1_poison = 0;
					//Increment the counter, to skip making a team for the 1st partner.
					$i++;
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
					$partner2_id = $_SESSION["players"][$i + 1];
					//Poison will always start at 0.
					$partner2_poison = 0;
					//Increment the counter, to skip making a team for the 2nd partner.
					$i++;
				}
				//If this is a Single game, copy the "partner2" values from "player".
				else
				{
					$partner2_id = $player_id;
					$partner2_life = 0;
					$partner2_poison = 0;
				}
				//Add this team to the "teams" table.
				$sql = "INSERT INTO teams (id, team_id, name, game_id, life, player_id, partner1_id, partner1_life, partner1_poison, 
				partner2_id, partner2_life, partner2_poison, emperor, poison, isAlive) VALUES ('$id', '$team_id', '', '$game_id, 
				'$life', '$player_id', '$partner1_id', '$partner1_life', '$partner1_poison', '$partner2_id', '$partner2_life', 
				'$partner2_poison', '$emperor', '$poison', '$isAlive')";
				$conn->query($sql);
			}
		}
	?>
 </body>
</html>




