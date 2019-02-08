/*
 * gamePanelEmperor.cpp
 */

#include "gamePanelEmperor.h"

//Local Game Constructor
gamePanelEmperor::gamePanelEmperor(std::vector<std::string> names, std::vector<int> ids, int teamSize)
{
	//Give this game a placeholder ID (it'll be needed later).
	remoteGameID = -1;
	//Set the Background Sprite.
	background = sf::Sprite(*panelTexture);
	//Set the internal variables.
	gameTypeID = 2;
	teamSizeID = teamSize;
	//Set up the Player vector.
	for (int i = 0; i < (int)names.size(); i++)
	{
		//Create a new Player object.
		player newPlayer;
		playerList.push_back(newPlayer);
		//Create the Player's "Button" sprites.
		playerList[playerList.size() - 1].buttonLifePlusOne = sf::Sprite(*buttonLifePlusOneTexture);
		playerList[playerList.size() - 1].buttonLifePlusFive = sf::Sprite(*buttonLifePlusFiveTexture);
		playerList[playerList.size() - 1].buttonLifeMinusFive = sf::Sprite(*buttonLifeMinusFiveTexture);
		playerList[playerList.size() - 1].buttonLifeMinusTen = sf::Sprite(*buttonLifeMinusTenTexture);
		playerList[playerList.size() - 1].buttonPoisonPlusOne = sf::Sprite(*buttonPoisonPlusOneTexture);
		playerList[playerList.size() - 1].buttonPoisonMinusOne = sf::Sprite(*buttonPoisonMinusOneTexture);
		//Set up the Life Counter (except size and position).
		playerList[playerList.size() - 1].lifeCounter.setFont(*font_dejavu);
		playerList[playerList.size() - 1].lifeCounter.setFillColor(sf::Color::Black);
		playerList[playerList.size() - 1].startingLife = 20;
		playerList[playerList.size() - 1].life = playerList[playerList.size() - 1].startingLife;
		playerList[playerList.size() - 1].lifeCounter.setString(std::to_string(playerList[playerList.size() - 1].life));
		playerList[playerList.size() - 1].isAlive = true;
		playerList[playerList.size() - 1].handshake = true;
		//Set up the Poison Counter (except size and position).
		playerList[playerList.size() - 1].poisonCounter.setFont(*font_dejavu);
		playerList[playerList.size() - 1].poisonCounter.setFillColor(sf::Color::Black);
		playerList[playerList.size() - 1].startingPoison = 0;
		playerList[playerList.size() - 1].poison = playerList[playerList.size() - 1].startingPoison;
		playerList[playerList.size() - 1].poisonCounter.setString(std::to_string(playerList[playerList.size() - 1].poison));
		//Set up the Name Text (except size and position).
		playerList[playerList.size() - 1].displayName = tgui::TextBox::create();
		gui->add(playerList[playerList.size() - 1].displayName);
		playerList[playerList.size() - 1].displayName->getRenderer()->setFont(*font_mtg);
		playerList[playerList.size() - 1].displayName->setReadOnly(false);
		playerList[playerList.size() - 1].displayName->getRenderer()->setBackgroundColor(sf::Color::Transparent);
		playerList[playerList.size() - 1].displayName->getRenderer()->setBorderColor(sf::Color::Transparent);
		playerList[playerList.size() - 1].displayName->getRenderer()->setTextColor(sf::Color::Black);
		playerList[playerList.size() - 1].name = names[i];
		playerList[playerList.size() - 1].displayName->setText(playerList[playerList.size() - 1].name);
		//Set up the Player's ID number.
		playerList[playerList.size() - 1].id = ids[i];
		//Initialize the Avatar.
		playerList[playerList.size() - 1].avatar = sf::Sprite(*avatarTextures[ids[i]]);
	}
}

//Remote Game Constructor
gamePanelEmperor::gamePanelEmperor(int gameID, std::string gameName)
{
	remoteGameID = gameID;
	//Set the Background Sprite.
	background = sf::Sprite(*panelTexture);
	//Set up the Game Title Display.
	gameDisplayName.setFont(*font_mtg);
	gameDisplayName.setFillColor(sf::Color::Black);
	gameDisplayName.setCharacterSize(24);
	gameDisplayName.setString(gameName);
	//Set the game-related variables.
	gameTypeID = 2;
	res = stmt->executeQuery(sql::SQLString("SELECT team_size FROM games WHERE id=").append(std::to_string(gameID)));
	while (res->next())
	{
		teamSizeID = res->getInt("team_size") - 1;
	}
	//Set up the Player vector.
	int numberOfPlayers;
	res = stmt->executeQuery(sql::SQLString("SELECT COUNT(*) FROM teams WHERE game_id=").append(std::to_string(gameID)));
	while (res->next())
	{
		numberOfPlayers = res->getInt(1) * (teamSizeID + 1);
	}
	res = stmt->executeQuery(sql::SQLString("SELECT team_id, name, life, player_id, partner1_id, partner1_life, partner1_poison, partner2_id, partner2_life, partner2_poison, poison, isAlive FROM teams WHERE game_id=").append(std::to_string(gameID)).append(" ORDER BY team_id ASC"));
	for (int i = 0; i < numberOfPlayers; i++)
	{
		res->next();
		AddPlayer(i, res->getInt("player_id"), res->getInt("life"), res->getInt("poison"));
		//If the team size is greater than 1, set up the first "General" player.
		if (teamSizeID > 0)
		{
			//Increase the loop iteration by 1.
			i++;
			//Add a new Player using the "partner1" data.
			AddPlayer(i, res->getInt("partner1_id"), res->getInt("partner1_life"), res->getInt("partner1_poison"));
		}
		//If the team size is 3, set up the second "General" player.
		if (teamSizeID == 2)
		{
			//Increase the loop iteration by 1.
			i++;
			//Add a new Player using the "partner2" data.
			AddPlayer(i, res->getInt("partner2_id"), res->getInt("partner2_life"), res->getInt("partner2_poison"));
		}
	}
}

void gamePanelEmperor::AddPlayer()
{

}

//Add a new Player to a Remote Emperor Game.
	//i: The current index in "playerList".
	//id: The ID number of the Player being added.
	//life: The Life value of the Player being added.
	//poison: The Poison value of the Player being added.
void gamePanelEmperor::AddPlayer(int i, int id, int life, int poison)
{
	//Create a new Player object.
	player newPlayer;
	playerList.push_back(newPlayer);
	//Set up the Player ID.
	playerList[i].id = id;
	//Set up the Life Counter (except size and position).
	playerList[i].lifeCounter.setFont(*font_dejavu);
	playerList[i].lifeCounter.setFillColor(sf::Color::Black);
	playerList[i].startingLife = life;
	playerList[i].life = playerList[i].startingLife;
	playerList[i].lifeCounter.setString(std::to_string(playerList[i].life));
	playerList[i].isAlive = res->getBoolean("isAlive");
	playerList[i].handshake = true;
	//Set up the Poison Counter (except size and position).
	playerList[i].poisonCounter.setFont(*font_dejavu);
	playerList[i].poisonCounter.setFillColor(sf::Color::Black);
	playerList[i].startingPoison = poison;
	playerList[i].poison = playerList[i].startingPoison;
	playerList[i].poisonCounter.setString(std::to_string(playerList[i].poison));
	//Set up the Name Text (except size and position).
	sql::ResultSet *playerName = stmt->executeQuery(sql::SQLString("SELECT name, text_color, background_color FROM players WHERE id=").append(std::to_string(id)));
	playerName->next();
	playerList[i].displayName = tgui::TextBox::create();
	gui->add(playerList[i].displayName);
	playerList[i].displayName->getRenderer()->setFont(*font_mtg);
	playerList[i].displayName->setReadOnly(true);
	playerList[i].displayName->getRenderer()->setBackgroundColor(sf::Color(
		sf::Uint8(strtol(playerName->getString("background_color").substr(0, 2).c_str(), NULL, 16)),
		sf::Uint8(strtol(playerName->getString("background_color").substr(2, 2).c_str(), NULL, 16)),
		sf::Uint8(strtol(playerName->getString("background_color").substr(4, 2).c_str(), NULL, 16))));
	playerList[i].displayName->getRenderer()->setBorderColor(sf::Color::Transparent);
	playerList[i].displayName->getRenderer()->setTextColor(sf::Color(
		sf::Uint8(strtol(playerName->getString("text_color").substr(0, 2).c_str(), NULL, 16)),
		sf::Uint8(strtol(playerName->getString("text_color").substr(2, 2).c_str(), NULL, 16)),
		sf::Uint8(strtol(playerName->getString("text_color").substr(4, 2).c_str(), NULL, 16))));
	playerList[i].name = playerName->getString("name");
	playerList[i].displayName->setText(playerList[i].name);
	playerList[i].displayName->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	//Initialize the Player's Avatar Sprite.
	playerList[i].avatar = sf::Sprite(*avatarTextures[id]);
}

gamePanelEmperor::~gamePanelEmperor()
{
}

void gamePanelEmperor::setTargetPosition(int x, int y)
{
	setPosition(x, y);
}

void gamePanelEmperor::setPosition(int x, int y)
{
	background.setPosition((float)x, (float)y);
	//Position the Game Title.
	if (remoteGameID != -1)
	{
		gameDisplayName.setPosition(background.getPosition().x + 8, background.getPosition().y + 8);
	}
	//Get the number of teams in this Game.
	int teams = (int)playerList.size() / (teamSizeID + 1);
	//Create a "buffer" value to use for positioning.
	float largeBuffer = (playerList[0].lifeCounter.getGlobalBounds().width / 2) + (playerList[0].buttonLifePlusOne.getGlobalBounds().width / 2) + 16;
	float smallBuffer = playerList[0].buttonLifePlusOne.getGlobalBounds().width + 16;
	//Landscape Positioning:
	if (background.getGlobalBounds().width >= background.getGlobalBounds().height)
	{
		//2 Teams, 2 Players Each:
		if (teams == 2 && teamSizeID == 1)
		{
			//Position the 1st Emperor's Life Counter.
			playerList[0].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 3.0f), background.getPosition().y + (background.getGlobalBounds().height / 4.0f));
			//Position the 1st General's Life Counter.
			playerList[1].lifeCounter.setPosition(background.getPosition().x + ((background.getGlobalBounds().width * 2.0f) / 3.0f), background.getPosition().y + (background.getGlobalBounds().height / 4.0f));
			//Position the 2nd Emperor's Life Counter.
			playerList[2].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 3.0f), background.getPosition().y + ((background.getGlobalBounds().height * 3.0f) / 4.0f));
			//Position the 2nd General's Life Counter.
			playerList[3].lifeCounter.setPosition(background.getPosition().x + ((background.getGlobalBounds().width * 2.0f) / 3.0f), background.getPosition().y + ((background.getGlobalBounds().height * 3.0f) / 4.0f));
		}
		//2 Teams, 3 Players Each:
		else if (teams == 2 && teamSizeID == 2)
		{
			//Position the 1st Emperor's Life Counter.
			playerList[0].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 2.0f), background.getPosition().y + (background.getGlobalBounds().height / 4.0f));
			//Position the 1st General's Life Counter.
			playerList[1].lifeCounter.setPosition(playerList[0].lifeCounter.getPosition().x - (background.getGlobalBounds().width * .3f), playerList[0].lifeCounter.getPosition().y);
			//Position the 2nd General's Life Counter.
			playerList[2].lifeCounter.setPosition(playerList[0].lifeCounter.getPosition().x + (background.getGlobalBounds().width * .3f), playerList[0].lifeCounter.getPosition().y);
			//Position the 2nd Emperor's Life Counter.
			playerList[3].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 2.0f), background.getPosition().y + ((background.getGlobalBounds().height * 3.0f) / 4.0f));
			//Position the 3rd General's Life Counter.
			playerList[4].lifeCounter.setPosition(playerList[3].lifeCounter.getPosition().x - (background.getGlobalBounds().width * .3f), playerList[3].lifeCounter.getPosition().y);
			//Position the 4th General's Life Counter.
			playerList[5].lifeCounter.setPosition(playerList[3].lifeCounter.getPosition().x + (background.getGlobalBounds().width * .3f), playerList[3].lifeCounter.getPosition().y);
		}
		//3 Teams, 2 Players Each:
		else if (teams == 3 && teamSizeID == 1)
		{
			//Position the 1st Emperor's Life Counter.
			playerList[0].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width * .2f), background.getPosition().y + (background.getGlobalBounds().height / 4.0f));
			//Position the 1st General's Life Counter.
			playerList[1].lifeCounter.setPosition(playerList[0].lifeCounter.getPosition().x, background.getPosition().y + ((background.getGlobalBounds().height * 3.0f) / 4.0f));
			//Position the 2nd Emperor's Life Counter.
			playerList[2].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 2.0f), background.getPosition().y + (background.getGlobalBounds().height / 4.0f));
			//Position the 2nd General's Life Counter.
			playerList[3].lifeCounter.setPosition(playerList[2].lifeCounter.getPosition().x, background.getPosition().y + ((background.getGlobalBounds().height * 3.0f) / 4.0f));
			//Position the 3rd Emperor's Life Counter.
			playerList[4].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width * .8f), background.getPosition().y + (background.getGlobalBounds().height / 4.0f));
			//Position the 3rd General's Life Counter.
			playerList[5].lifeCounter.setPosition(playerList[4].lifeCounter.getPosition().x, background.getPosition().y + ((background.getGlobalBounds().height * 3.0f) / 4.0f));
		}
	}
	//Portrait Positioning:
	else
	{
		//2 Teams, 2 Players Each:
		if (teams == 2 && teamSizeID == 1)
		{
			//Position the 1st Emperor's Life Counter.
			playerList[0].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 4.0f), background.getPosition().y + (background.getGlobalBounds().height / 3.0f));
			//Position the 1st General's Life Counter.
			playerList[1].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 4.0f), background.getPosition().y + ((background.getGlobalBounds().height * 2.0f) / 3.0f));
			//Position the 2nd Emperor's Life Counter.
			playerList[2].lifeCounter.setPosition(background.getPosition().x + ((background.getGlobalBounds().width * 3.0f) / 4.0f), background.getPosition().y + (background.getGlobalBounds().height / 3.0f));
			//Position the 2nd General's Life Counter.
			playerList[3].lifeCounter.setPosition(background.getPosition().x + ((background.getGlobalBounds().width * 3.0f) / 4.0f), background.getPosition().y + ((background.getGlobalBounds().height * 2.0f) / 3.0f));
		}
		//2 Teams, 3 Players Each:
		else if (teams == 2 && teamSizeID == 2)
		{
			//Position the 1st Emperor's Life Counter.
			playerList[0].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 4.0f), background.getPosition().y + (background.getGlobalBounds().height / 2.0f));
			//Position the 1st General's Life Counter.
			playerList[1].lifeCounter.setPosition(playerList[0].lifeCounter.getPosition().x, playerList[0].lifeCounter.getPosition().y - (background.getGlobalBounds().height * .3f));
			//Position the 2nd General's Life Counter.
			playerList[2].lifeCounter.setPosition(playerList[0].lifeCounter.getPosition().x, playerList[0].lifeCounter.getPosition().y + (background.getGlobalBounds().height * .3f));
			//Position the 2nd Emperor's Life Counter.
			playerList[3].lifeCounter.setPosition(background.getPosition().x + ((background.getGlobalBounds().width  * 3.0f) / 4.0f), background.getPosition().y + (background.getGlobalBounds().height / 2.0f));
			//Position the 1st General's Life Counter.
			playerList[4].lifeCounter.setPosition(playerList[3].lifeCounter.getPosition().x, playerList[3].lifeCounter.getPosition().y - (background.getGlobalBounds().height * .3f));
			//Position the 2nd General's Life Counter.
			playerList[5].lifeCounter.setPosition(playerList[3].lifeCounter.getPosition().x, playerList[3].lifeCounter.getPosition().y + (background.getGlobalBounds().height * .3f));
		}
		//3 Teams, 2 Players Each:
		else if (teams == 3 && teamSizeID == 1)
		{
			//Position the 1st Emperor's Life Counter.
			playerList[0].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 4.0f), background.getPosition().y + (background.getGlobalBounds().height * .2f));
			//Position the 1st General's Life Counter.
			playerList[1].lifeCounter.setPosition(background.getPosition().x + ((background.getGlobalBounds().width * 3.0f) / 4.0f), playerList[0].lifeCounter.getPosition().y);
			//Position the 2nd Emperor's Life Counter.
			playerList[2].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 4.0f), background.getPosition().y + (background.getGlobalBounds().height / 2.0f));
			//Position the 2nd General's Life Counter.
			playerList[3].lifeCounter.setPosition(background.getPosition().x + ((background.getGlobalBounds().width * 3.0f) / 4.0f), playerList[2].lifeCounter.getPosition().y);
			//Position the 3rd Emperor's Life Counter.
			playerList[4].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 4.0f), background.getPosition().y + (background.getGlobalBounds().height * .8f));
			//Position the 3rd General's Life Counter.
			playerList[5].lifeCounter.setPosition(background.getPosition().x + ((background.getGlobalBounds().width * 3.0f) / 4.0f), playerList[4].lifeCounter.getPosition().y);
		}
	}
	//Position the rest of the Player buttons.
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		//Life Buttons:
		playerList[i].buttonLifeMinusTen.setPosition(playerList[i].lifeCounter.getPosition().x - largeBuffer, playerList[i].lifeCounter.getPosition().y + (smallBuffer / 2.0f) + (playerList[i].buttonLifeMinusTen.getGlobalBounds().height / 2));
		playerList[i].buttonLifeMinusFive.setPosition(playerList[i].lifeCounter.getPosition().x - largeBuffer, playerList[i].lifeCounter.getPosition().y - (smallBuffer / 2.0f) + (playerList[i].buttonLifeMinusFive.getGlobalBounds().height / 2));
		playerList[i].buttonLifePlusOne.setPosition(playerList[i].lifeCounter.getPosition().x + largeBuffer, playerList[i].lifeCounter.getPosition().y + (smallBuffer / 2.0f) + (playerList[i].buttonLifePlusOne.getGlobalBounds().height / 2));
		playerList[i].buttonLifePlusFive.setPosition(playerList[i].lifeCounter.getPosition().x + largeBuffer, playerList[i].lifeCounter.getPosition().y - (smallBuffer / 2.0f) + (playerList[i].buttonLifePlusFive.getGlobalBounds().height / 2));
		//Poison Counter and Buttons:
		playerList[i].poisonCounter.setPosition(playerList[i].lifeCounter.getPosition().x, playerList[i].lifeCounter.getPosition().y + largeBuffer);
		playerList[i].buttonPoisonMinusOne.setPosition(playerList[i].poisonCounter.getPosition().x - smallBuffer, playerList[i].poisonCounter.getPosition().y);
		playerList[i].buttonPoisonPlusOne.setPosition(playerList[i].poisonCounter.getPosition().x + smallBuffer, playerList[i].poisonCounter.getPosition().y);
		//Name:
		playerList[i].displayName->setPosition(playerList[i].lifeCounter.getGlobalBounds().left, playerList[i].lifeCounter.getGlobalBounds().top - (playerList[i].displayName->getFullSize().y * 1.5f));
		//Avatar:
		playerList[i].avatar.setPosition(playerList[i].displayName->getPosition().x - playerList[i].avatar.getGlobalBounds().width, playerList[i].displayName->getPosition().y - (playerList[i].avatar.getGlobalBounds().height / 2.0f));
	}
}

void gamePanelEmperor::setTargetSize(int width, int height)
{
	resize(width, height);
}

void gamePanelEmperor::resize(int width, int height)
{
	//Resize the Background.
	background.scale(width / background.getGlobalBounds().width, height / background.getGlobalBounds().height);
	//Set Life Counter Size
		//If the Panel is landscape, scale the Counter by its height.
	if (background.getGlobalBounds().height < background.getGlobalBounds().width)
	{
		while (!(playerList[0].lifeCounter.getGlobalBounds().height >= background.getGlobalBounds().height / 7
			&& playerList[0].lifeCounter.getGlobalBounds().height <= background.getGlobalBounds().height / 6))
		{
			//If the Life Counter is too small, increase its font size.
			if (playerList[0].lifeCounter.getGlobalBounds().height < background.getGlobalBounds().height / 7)
			{
				playerList[0].lifeCounter.setCharacterSize(playerList[0].lifeCounter.getCharacterSize() + 2);
			}
			//If the Life Counter is too large, decrease its font size.
			else if (playerList[0].lifeCounter.getGlobalBounds().height > background.getGlobalBounds().height / 6)
			{
				playerList[0].lifeCounter.setCharacterSize(playerList[0].lifeCounter.getCharacterSize() - 2);
			}
		}
	}
	//Otherwise, scale it by its width.
	else
	{
		while (!(playerList[0].lifeCounter.getGlobalBounds().width >= background.getGlobalBounds().width / 5
			&& playerList[0].lifeCounter.getGlobalBounds().width <= background.getGlobalBounds().width / 4))
		{
			//If the Life Counter is too small, increase its font size.
			if (playerList[0].lifeCounter.getGlobalBounds().width < background.getGlobalBounds().width / 5)
			{
				playerList[0].lifeCounter.setCharacterSize(playerList[0].lifeCounter.getCharacterSize() + 2);
			}
			//If the Life Counter is too large, decrease its font size.
			else if (playerList[0].lifeCounter.getGlobalBounds().width > background.getGlobalBounds().width / 4)
			{
				playerList[0].lifeCounter.setCharacterSize(playerList[0].lifeCounter.getCharacterSize() - 2);
			}
		}
	}
	//Resize the visual elements for each player, and reset their Origins.
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		//Resize the rest of the Life Counters.
		playerList[i].lifeCounter.setCharacterSize(playerList[0].lifeCounter.getCharacterSize());
		//Resize the Display Names and Avatars.
		playerList[i].displayName->setTextSize(playerList[i].lifeCounter.getCharacterSize() / 6);
		playerList[i].displayName->setSize(playerList[i].displayName->getTextSize() * playerList[i].name.length(), playerList[i].displayName->getTextSize() + 2);
		playerList[i].avatar.setScale((playerList[i].displayName->getFullSize().y * 2) / playerList[i].avatar.getTexture()->getSize().x, (playerList[i].displayName->getFullSize().y * 2) / playerList[i].avatar.getTexture()->getSize().y);
		//Resize all of the Life Buttons.
		playerList[i].buttonLifePlusOne.scale((playerList[i].lifeCounter.getGlobalBounds().height / 2) / playerList[i].buttonLifePlusOne.getGlobalBounds().height,
			(playerList[i].lifeCounter.getGlobalBounds().height / 2) / playerList[i].buttonLifePlusOne.getGlobalBounds().height);
		playerList[i].buttonLifePlusFive.setScale(playerList[i].buttonLifePlusOne.getScale());
		playerList[i].buttonLifeMinusFive.setScale(playerList[i].buttonLifePlusOne.getScale());
		playerList[i].buttonLifeMinusTen.setScale(playerList[i].buttonLifePlusOne.getScale());
		//Resize each of the Poison Counters.
		playerList[i].poisonCounter.setCharacterSize((unsigned int)(playerList[i].lifeCounter.getCharacterSize() - 24));
		//Resize all of the Poison Buttons.
		playerList[i].buttonPoisonMinusOne.scale(playerList[i].poisonCounter.getGlobalBounds().height / playerList[i].buttonPoisonMinusOne.getGlobalBounds().height,
			playerList[i].poisonCounter.getGlobalBounds().height / playerList[i].buttonPoisonMinusOne.getGlobalBounds().height);
		playerList[i].buttonPoisonPlusOne.setScale(playerList[i].buttonPoisonMinusOne.getScale());
		//Set the origin points for each of the visual elements (except the Display Names) to their centers.
		playerList[i].lifeCounter.setOrigin(playerList[i].lifeCounter.getGlobalBounds().width / 2.0f, playerList[i].lifeCounter.getGlobalBounds().height / 2.0f);
		playerList[i].buttonLifePlusOne.setOrigin(playerList[i].buttonLifePlusOne.getGlobalBounds().width / 2, playerList[i].buttonLifePlusOne.getGlobalBounds().height / 2);
		playerList[i].buttonLifePlusFive.setOrigin(playerList[i].buttonLifePlusFive.getGlobalBounds().width / 2, playerList[i].buttonLifePlusFive.getGlobalBounds().height / 2);
		playerList[i].buttonLifeMinusFive.setOrigin(playerList[i].buttonLifeMinusFive.getGlobalBounds().width / 2, playerList[i].buttonLifeMinusFive.getGlobalBounds().height / 2);
		playerList[i].buttonLifeMinusTen.setOrigin(playerList[i].buttonLifeMinusTen.getGlobalBounds().width / 2, playerList[i].buttonLifeMinusTen.getGlobalBounds().height / 2);
		playerList[i].poisonCounter.setOrigin(playerList[i].poisonCounter.getGlobalBounds().width / 2.0f, playerList[i].poisonCounter.getGlobalBounds().height / 2.0f);
		playerList[i].buttonPoisonPlusOne.setOrigin(playerList[i].buttonPoisonPlusOne.getGlobalBounds().width / 2, playerList[i].buttonPoisonPlusOne.getGlobalBounds().height / 2);
		playerList[i].buttonPoisonMinusOne.setOrigin(playerList[i].buttonPoisonMinusOne.getGlobalBounds().width / 2, playerList[i].buttonPoisonMinusOne.getGlobalBounds().height / 2);
	}
}

//Draw the Game Panel's visual elements.
void gamePanelEmperor::draw()
{
	//Draw the Background.
	window->draw(background);
	//Draw the Game Title.
	window->draw(gameDisplayName);
	//Draw the elements for every Player.
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		//Avatar:
		window->draw(playerList[i].avatar);
		//Life Counter:
		window->draw(playerList[i].lifeCounter);
		//Life Buttons:
		window->draw(playerList[i].buttonLifeMinusTen);
		window->draw(playerList[i].buttonLifeMinusFive);
		window->draw(playerList[i].buttonLifePlusOne);
		window->draw(playerList[i].buttonLifePlusFive);
		//Poison Counter:
		window->draw(playerList[i].poisonCounter);
		//Poison Buttons:
		window->draw(playerList[i].buttonPoisonMinusOne);
		window->draw(playerList[i].buttonPoisonPlusOne);
	}
	//Draw the Gui objects (the Players' names)
	gui->draw();
}

void gamePanelEmperor::update()
{
	//Set each Player's "handshake" flag to false.
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		playerList[i].handshake = false;
	}
	//Pull the list of Players in this game.
	res = stmt->executeQuery(sql::SQLString("SELECT team_id, name, life, player_id, partner1_id, partner1_life, partner1_poison, partner2_id, partner2_life, partner2_poison, poison, isAlive FROM teams WHERE game_id=").append(std::to_string(remoteGameID)).append(" ORDER BY team_id ASC"));
	sql::ResultSet *currentPlayer;
	while (res->next())
	{
		//Reset the flag that says whether this result has been used.
		currentPlayerUsed = false;
		//Check the "Emperor"'s ID first.
		currentPlayer = stmt->executeQuery(sql::SQLString("SELECT name FROM players WHERE id=").append(std::to_string(res->getInt("player_id"))));
		currentPlayer->next();
		//Look through all of the Players currently in this game.
		for (int i = 0; i < (int)playerList.size(); i++)
		{
			//If this Player's name matches any of the ones in the game, update their game values to match what's in the table.
			if (playerList[i].name == currentPlayer->getString("name"))
			{
				playerList[i].life = res->getInt("life");
				playerList[i].lifeCounter.setString(std::to_string(playerList[i].life));
				playerList[i].isAlive = res->getBoolean("isAlive");
				playerList[i].poison = res->getInt("poison");
				playerList[i].poisonCounter.setString(std::to_string(playerList[i].poison));
				//Set the Player's "handshake" to true when their stats get updated.
				playerList[i].handshake = true;
				//Set the flag that says this result has been used.
				currentPlayerUsed = true;
			}
		}
		//If this result hasn't updated a Player's stats, make a new Player using it.
		if (!currentPlayerUsed)
		{
			AddPlayer(playerList.size() - 1, res->getInt("player_id"), res->getInt("life"), res->getInt("poison"));
		}
		//Check the ID of the first "General" on the team.
		if (teamSizeID > 0)
		{
			//Reset the flag that says whether this result has been used.
			currentPlayerUsed = false;
			currentPlayer = stmt->executeQuery(sql::SQLString("SELECT name FROM players WHERE id=").append(std::to_string(res->getInt("partner1_id"))));
			currentPlayer->next();
			for (int i = 0; i < (int)playerList.size(); i++)
			{
				//If this Player's name matches any of the ones in the game, update their game values to match what's in the table.
				if (playerList[i].name == currentPlayer->getString("name"))
				{
					playerList[i].life = res->getInt("partner1_life");
					playerList[i].lifeCounter.setString(std::to_string(playerList[i].life));
					playerList[i].isAlive = res->getBoolean("isAlive");
					playerList[i].poison = res->getInt("partner1_poison");
					playerList[i].poisonCounter.setString(std::to_string(playerList[i].poison));
					//Set the Player's "handshake" to true when their stats get updated.
					playerList[i].handshake = true;
					//Set the flag that says this result has been used.
					currentPlayerUsed = true;
				}
			}
			//If this result hasn't updated a Player's stats, make a new Player using it.
			if (!currentPlayerUsed)
			{
				AddPlayer(playerList.size() - 1, res->getInt("partner1_id"), res->getInt("partner1_life"), res->getInt("partner1_poison"));
			}
		}
		//Check the ID of the second "General" on the team.
		if (teamSizeID == 2)
		{
			//Reset the flag that says whether this result has been used.
			currentPlayerUsed = false;
			currentPlayer = stmt->executeQuery(sql::SQLString("SELECT name FROM players WHERE id=").append(std::to_string(res->getInt("partner2_id"))));
			currentPlayer->next();
			for (int i = 0; i < (int)playerList.size(); i++)
			{
				//If this Player's name matches any of the ones in the game, update their game values to match what's in the table.
				if (playerList[i].name == currentPlayer->getString("name"))
				{
					playerList[i].life = res->getInt("partner2_life");
					playerList[i].lifeCounter.setString(std::to_string(playerList[i].life));
					playerList[i].isAlive = res->getBoolean("isAlive");
					playerList[i].poison = res->getInt("partner2_poison");
					playerList[i].poisonCounter.setString(std::to_string(playerList[i].poison));
					//Set the Player's "handshake" to true when their stats get updated.
					playerList[i].handshake = true;
					//Set the flag that says this result has been used.
					currentPlayerUsed = true;
				}
			}
			//If this result hasn't updated a Player's stats, make a new Player using it.
			if (!currentPlayerUsed)
			{
				AddPlayer(playerList.size() - 1, res->getInt("partner2_id"), res->getInt("partner2_life"), res->getInt("partner2_poison"));
			}
		}
	}
	//Remove any Player that failed the "handshake" check, as they've likely been removed from the remote instance of the game.
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		if (!playerList[i].handshake)
		{
			playerList.erase(playerList.begin() + i);
			resize((int)background.getGlobalBounds().width, (int)background.getGlobalBounds().height);
			setPosition((int)background.getPosition().x, (int)background.getPosition().y);
		}
	}
	delete currentPlayer;
}

//Disable the Panel's GUI objects when the Game Setup Panel is open.
void gamePanelEmperor::disableGUI()
{
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		playerList[i].displayName->setEnabled(false);
	}
}

//Re-enable the Panel's GUI objects when the Game Setup Panel is closed.
void gamePanelEmperor::enableGUI()
{
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		playerList[i].displayName->setEnabled(true);
	}
}
