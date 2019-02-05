/*
 * gamePanel.cpp
 */

#include "gamePanel.h"

 //Local Game Constructor
gamePanel::gamePanel(std::vector<std::string> names, std::vector<int> ids, int gameType, int teamSize)
{
	//Give this game a placeholder ID (it'll be needed later).
	remoteGameID = -1;
	//Set the Background Sprite.
	background = sf::Sprite(*panelTexture);
	//Set the internal variables.
	gameTypeID = gameType;
	teamSizeID = teamSize;
	//Get each Player's starting life value from the Game Type, and save it temporarily.
	int startLife;
	switch (gameType)
	{
		//Standard
	case 0:
		startLife = 20;
		break;
		//Commander
	case 1:
		startLife = 40;
		break;
		//
		//There is no case for Emperor, as that is handled in gamePanelEmperor.cpp.
		//
		//Brawl
	case 3:
		//If there are only two players/teams, start at 25 life. Otherwise, start at 30.
		startLife = (names.size() / (teamSize + 1) == 2) ? startLife = 25 : startLife = 30;
		break;
	}
	//Increase the Starting Life by how many Players are in each team.
	switch (teamSize)
	{
		//Solo
	case 0:
		break;
		//Two-Headed
	case 1:
		startLife = (startLife * 3) / 2;
		break;
		//Three-Headed
	case 2:
		startLife = ((startLife * 3) / 2) + 15;
	}
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
		playerList[playerList.size() - 1].startingLife = startLife;
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
		//Set up the Commander Damage Objects (except size and position).
		playerList[playerList.size() - 1].cmdrCloseButton = sf::Sprite(*buttonCommanderCloseTexture);
		playerList[playerList.size() - 1].cmdrCloseButton.setColor(sf::Color(255, 255, 255, 0));
		playerList[playerList.size() - 1].cmdrCountersExpanded = false;
		playerList[playerList.size() - 1].startingCmdrDmg = 0;
		for (int i = 0; i < (int)names.size() / (teamSize + 1); i++)
		{
			playerList[playerList.size() - 1].cmdrDmg.push_back(playerList[playerList.size() - 1].startingCmdrDmg);
			playerList[playerList.size() - 1].cmdrDmgCounters.push_back(sf::Text(std::to_string(playerList[playerList.size() - 1].cmdrDmg[i]), *font_dejavu));
			playerList[playerList.size() - 1].cmdrDmgCounters[i].setFillColor(sf::Color::Black);
			playerList[playerList.size() - 1].cmdrAvatars.push_back(sf::Sprite(*avatarTextures[ids[i * (teamSize + 1)]]));
			playerList[playerList.size() - 1].cmdrButtons.push_back(sf::Sprite(*buttonCommanderMinusOneTexture));
			playerList[playerList.size() - 1].cmdrButtons[i].setColor(sf::Color(255, 255, 255, 0));
		}
		//Set up the Name Text (except size and position).
		playerList[playerList.size() - 1].displayName = tgui::TextBox::create();
		gui->add(playerList[playerList.size() - 1].displayName);
		playerList[playerList.size() - 1].displayName->setFont(*font_mtg);
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
		//Set up the 1st Partner's Name Text and Avatar (if there is one).
		if (teamSize > 0)
		{
			playerList[playerList.size() - 1].partnerDisplayNameOne = tgui::TextBox::create();
			gui->add(playerList[playerList.size() - 1].partnerDisplayNameOne);
			playerList[playerList.size() - 1].partnerDisplayNameOne->setFont(*font_mtg);
			playerList[playerList.size() - 1].partnerDisplayNameOne->setReadOnly(false);
			playerList[playerList.size() - 1].partnerDisplayNameOne->getRenderer()->setBackgroundColor(sf::Color::Transparent);
			playerList[playerList.size() - 1].partnerDisplayNameOne->getRenderer()->setBorderColor(sf::Color::Transparent);
			playerList[playerList.size() - 1].partnerDisplayNameOne->getRenderer()->setTextColor(sf::Color::Black);
			playerList[playerList.size() - 1].partnerNameOne = names[i + 1];
			playerList[playerList.size() - 1].partnerDisplayNameOne->setText(playerList[playerList.size() - 1].partnerNameOne);
			playerList[playerList.size() - 1].partnerAvatarOne = sf::Sprite(*avatarTextures[ids[i + 1]]);
			i++;
		}
		//Set up the 2nd Partner's Name Text (if there is one).
		if (teamSize == 2)
		{
			playerList[playerList.size() - 1].partnerDisplayNameTwo = tgui::TextBox::create();
			gui->add(playerList[playerList.size() - 1].partnerDisplayNameTwo);
			playerList[playerList.size() - 1].partnerDisplayNameTwo->setFont(*font_mtg);
			playerList[playerList.size() - 1].partnerDisplayNameTwo->setReadOnly(false);
			playerList[playerList.size() - 1].partnerDisplayNameTwo->getRenderer()->setBackgroundColor(sf::Color::Transparent);
			playerList[playerList.size() - 1].partnerDisplayNameTwo->getRenderer()->setBorderColor(sf::Color::Transparent);
			playerList[playerList.size() - 1].partnerDisplayNameTwo->getRenderer()->setTextColor(sf::Color::Black);
			playerList[playerList.size() - 1].partnerNameTwo = names[i + 1];
			playerList[playerList.size() - 1].partnerDisplayNameTwo->setText(playerList[playerList.size() - 1].partnerNameTwo);
			playerList[playerList.size() - 1].partnerAvatarTwo = sf::Sprite(*avatarTextures[ids[i + 1]]);
			i++;
		}
	}
	//Setup the "Add Player" Button.
	addPlayerButton = sf::Sprite(*addPlayerPanelButtonTexture);
	addPlayerButton.setScale(.75f, .75f);
	//Set up the Commander Counter Hover Text.
	cmdrHoverText = tgui::TextBox::create();
	gui->add(cmdrHoverText);
	cmdrHoverText->getRenderer()->setBorders(tgui::Borders(0));
	cmdrHoverText->getRenderer()->setBackgroundColor(sf::Color::Transparent);
	cmdrHoverText->getRenderer()->setTextColor(sf::Color::Transparent);
	cmdrHoverText->getRenderer()->setCaretColor(sf::Color::Transparent);
	cmdrHoverText->setReadOnly(false);
}

//Remote Game Constructor
gamePanel::gamePanel(int gameID, std::string gameName)
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
	res = stmt->executeQuery(sql::SQLString("SELECT team_size, game_type FROM games WHERE id=").append(std::to_string(gameID)));
	while (res->next())
	{
		teamSizeID = res->getInt("team_size") - 1;
		gameTypeID = res->getInt("game_type");
	}
	//Set up the Player vector.
	int numberOfPlayers;
	res = stmt->executeQuery(sql::SQLString("SELECT COUNT(*) FROM teams WHERE game_id=").append(std::to_string(gameID)));
	while (res->next())
	{
		numberOfPlayers = res->getInt(1);
	}
	res = stmt->executeQuery(sql::SQLString("SELECT team_id, name, life, player_id, partner1_id, partner2_id, emperor, poison, isAlive FROM teams WHERE game_id=").append(std::to_string(gameID)).append(" ORDER BY team_id ASC"));
	for (int i = 0; i < numberOfPlayers; i++)
	{
		res->next();
		//Create a new Player object.
		player newPlayer;
		playerList.push_back(newPlayer);
		//Set up the Player ID.
		playerList[i].id = res->getInt("player_id");
		//Set up the Life Counter (except size and position).
		playerList[i].lifeCounter.setFont(*font_dejavu);
		playerList[i].lifeCounter.setFillColor(sf::Color::Black);
		playerList[i].startingLife = res->getInt("life");
		playerList[i].life = playerList[i].startingLife;
		playerList[i].lifeCounter.setString(std::to_string(playerList[i].life));
		playerList[i].isAlive = res->getBoolean("isAlive");
		playerList[i].handshake = true;
		//Set up the Poison Counter (except size and position).
		playerList[i].poisonCounter.setFont(*font_dejavu);
		playerList[i].poisonCounter.setFillColor(sf::Color::Black);
		playerList[i].startingPoison = 0;
		playerList[i].poison = playerList[i].startingPoison;
		playerList[i].poisonCounter.setString(std::to_string(playerList[i].poison));
		//Set up the Commander Damage Counters (except size and position), if this is a Commander game.
		if (gameTypeID == 1)
		{
			sql::ResultSet *commanderDamage = stmt->executeQuery(sql::SQLString("SELECT from_id, to_id, life FROM commander_dmg WHERE game_id=").append(std::to_string(gameID)).append(sql::SQLString(" AND from_id=")).append(std::to_string(res->getInt("player_id"))).append(sql::SQLString(" ORDER BY to_id ASC")));
			playerList[i].startingCmdrDmg = 0;
			playerList[i].cmdrCountersExpanded = false;
			for (int j = 0; j < (int)commanderDamage->rowsCount(); j++)
			{
				commanderDamage->next();
				playerList[i].cmdrDmg.push_back(commanderDamage->getInt("life"));
				playerList[i].cmdrDmgCounters.push_back(sf::Text(std::to_string(playerList[i].cmdrDmg[j]), *font_dejavu));
				playerList[i].cmdrDmgCounters[j].setFillColor(sf::Color::Black);
				playerList[i].cmdrAvatars.push_back(sf::Sprite(*avatarTextures[commanderDamage->getInt("to_id")]));
				playerList[i].cmdrButtons.push_back(sf::Sprite());
			}
		}
		//Set up the Name Text (except size and position).
		sql::ResultSet *playerName = stmt->executeQuery(sql::SQLString("SELECT name, text_color, background_color FROM players WHERE id=").append(std::to_string(res->getInt("player_id"))));
		playerName->next();
		playerList[i].displayName = tgui::TextBox::create();
		gui->add(playerList[i].displayName);
		playerList[i].displayName->setFont(*font_mtg);
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
		//Initialize the Player's Avatar Sprite.
		playerList[i].avatar = sf::Sprite(*avatarTextures[res->getInt("player_id")]);
		//If this team has a unique "partner1_id", set up the 2nd Player's name.
		if (res->getInt("player_id") != res->getInt("partner1_id"))
		{
			playerName = stmt->executeQuery(sql::SQLString("SELECT name, text_color, background_color FROM players WHERE id=").append(std::to_string(res->getInt("partner1_id"))));
			playerName->next();
			playerList[i].partnerDisplayNameOne = tgui::TextBox::create();
			gui->add(playerList[i].partnerDisplayNameOne);
			playerList[i].partnerDisplayNameOne->setFont(*font_mtg);
			playerList[i].partnerDisplayNameOne->setReadOnly(true);
			playerList[i].partnerDisplayNameOne->getRenderer()->setBackgroundColor(sf::Color(
				sf::Uint8(strtol(playerName->getString("background_color").substr(0, 2).c_str(), NULL, 16)),
				sf::Uint8(strtol(playerName->getString("background_color").substr(2, 2).c_str(), NULL, 16)),
				sf::Uint8(strtol(playerName->getString("background_color").substr(4, 2).c_str(), NULL, 16))));
			playerList[i].partnerDisplayNameOne->getRenderer()->setBorderColor(sf::Color::Transparent);
			playerList[i].partnerDisplayNameOne->getRenderer()->setTextColor(sf::Color(
				sf::Uint8(strtol(playerName->getString("text_color").substr(0, 2).c_str(), NULL, 16)),
				sf::Uint8(strtol(playerName->getString("text_color").substr(2, 2).c_str(), NULL, 16)),
				sf::Uint8(strtol(playerName->getString("text_color").substr(4, 2).c_str(), NULL, 16))));
			playerList[i].partnerNameOne = playerName->getString("name");
			playerList[i].partnerDisplayNameOne->setText(playerList[i].partnerNameOne);
			playerList[i].partnerAvatarOne = sf::Sprite(*avatarTextures[res->getInt("partner1_id")]);
			//Also set up the Team Name.
			playerList[i].teamDisplayName = tgui::TextBox::create();
			gui->add(playerList[i].teamDisplayName);
			playerList[i].teamDisplayName->setFont(*font_mtg);
			playerList[i].teamDisplayName->setReadOnly(true);
			playerList[i].teamDisplayName->getRenderer()->setBackgroundColor(sf::Color::Transparent);
			playerList[i].teamDisplayName->getRenderer()->setBorderColor(sf::Color::Transparent);
			playerList[i].teamDisplayName->getRenderer()->setTextColor(sf::Color::Black);
			playerList[i].teamName = res->getString("name");
			playerList[i].teamDisplayName->setText(playerList[i].teamName);
		}
		//If this team has a unique "partner2_id", set up the 3rd Player's name.
		if (res->getInt("player_id") != res->getInt("partner2_id") && res->getInt("partner1_id") != res->getInt("partner2_id"))
		{
			playerName = stmt->executeQuery(sql::SQLString("SELECT name, text_color, background_color FROM players WHERE id=").append(std::to_string(res->getInt("partner2_id"))));
			playerName->next();
			playerList[i].partnerDisplayNameTwo = tgui::TextBox::create();
			gui->add(playerList[i].partnerDisplayNameTwo);
			playerList[i].partnerDisplayNameTwo->setFont(*font_mtg);
			playerList[i].partnerDisplayNameTwo->setReadOnly(true);
			playerList[i].partnerDisplayNameTwo->getRenderer()->setBackgroundColor(sf::Color(
				sf::Uint8(strtol(playerName->getString("background_color").substr(0, 2).c_str(), NULL, 16)),
				sf::Uint8(strtol(playerName->getString("background_color").substr(2, 2).c_str(), NULL, 16)),
				sf::Uint8(strtol(playerName->getString("background_color").substr(4, 2).c_str(), NULL, 16))));
			playerList[i].partnerDisplayNameTwo->getRenderer()->setBorderColor(sf::Color::Transparent);
			playerList[i].partnerDisplayNameTwo->getRenderer()->setTextColor(sf::Color(
				sf::Uint8(strtol(playerName->getString("text_color").substr(0, 2).c_str(), NULL, 16)),
				sf::Uint8(strtol(playerName->getString("text_color").substr(2, 2).c_str(), NULL, 16)),
				sf::Uint8(strtol(playerName->getString("text_color").substr(4, 2).c_str(), NULL, 16))));
			playerList[i].partnerNameTwo = playerName->getString("name");
			playerList[i].partnerDisplayNameTwo->setText(playerList[i].partnerNameTwo);
			playerList[i].partnerAvatarTwo = sf::Sprite(*avatarTextures[res->getInt("partner2_id")]);
		}
		delete playerName;
	}
	//Set up the Commander Counter Hover Text.
	cmdrHoverText = tgui::TextBox::create();
	gui->add(cmdrHoverText);
	cmdrHoverText->getRenderer()->setBorders(tgui::Borders(0));
	cmdrHoverText->getRenderer()->setBackgroundColor(sf::Color::Transparent);
	cmdrHoverText->getRenderer()->setTextColor(sf::Color::Transparent);
	cmdrHoverText->getRenderer()->setCaretColor(sf::Color::Transparent);
	cmdrHoverText->setReadOnly(false);
}

//Default Constructor (shouldn't ever be used)
gamePanel::gamePanel()
{
}

void gamePanel::setTargetPosition(int x, int y)
{
	targetPosition = Vector2i(x, y);
	//if (panelRepositionThread.joinable()) { panelRepositionThread.join(); }
	//panelRepositionThread = std::thread(std::bind(&gamePanel::setPosition, this, x, y));
	setPosition(x, y);
}

//Set the new position for the Panel.
	//x: The new x-position of the Panel, on its left edge.
	//y: The new y-position of the Panel, on its upper edge.
void gamePanel::setPosition(int x, int y)
{
	//x = (x - background.getPosition().x) / 15.0f;
	//y = (y - background.getPosition().y) / 15.0f;
	//do
	//{
		//if (std::abs(x) > std::abs(targetPosition.x - background.getPosition().x)) { x = targetPosition.x - background.getPosition().x; }
		//if (std::abs(y) > std::abs(targetPosition.y - background.getPosition().y)) { y = targetPosition.y - background.getPosition().y; }
		//Set the position for the Panel's background.
		//background.move(x, y);
	background.setPosition((float)x, (float)y);
	//Position the Game Title.
	if (remoteGameID != -1)
	{
		gameDisplayName.setPosition(background.getPosition().x + 8, background.getPosition().y + 8);
	}
	//Set the position for the "Add Player" Button.
	addPlayerButton.setPosition((background.getPosition() + sf::Vector2f((background.getGlobalBounds().width / 2.0f) - (addPlayerButton.getGlobalBounds().width / 2.0f),
		(background.getGlobalBounds().height / 2.0f) - (addPlayerButton.getGlobalBounds().height / 2.0f))));
	//Get a "radius" base for the positions first.
	float xRadius = background.getGlobalBounds().width / 4;
	float yRadius = background.getGlobalBounds().height / 4;
	//Iterate through all of the Players.
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		float xOffset;
		float yOffset;
		//Position the Life Counters.
		//If the Panel is landscape, start at 0 degrees.
		if (background.getGlobalBounds().width >= background.getGlobalBounds().height)
		{
			xOffset = cos((360.0f / playerList.size())*i*(3.14f / 180.0f)) * xRadius;
			yOffset = sin((360.0f / playerList.size())*i*(3.14f / 180.0f)) * yRadius;
		}
		//If the Panel is portrait, start at 90 degrees.
		else
		{
			xOffset = cos(((((360.0f / playerList.size())*i) + 90)*(3.14f / 180.0f))) * xRadius;
			yOffset = sin(((((360.0f / playerList.size())*i) + 90)*(3.14f / 180.0f))) * yRadius;
		}
		playerList[i].lifeCounter.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 2) + xOffset,
			background.getPosition().y + (background.getGlobalBounds().height / 2) + yOffset);
		//Position the Life Buttons.
		float largeBuffer = (playerList[i].lifeCounter.getGlobalBounds().width / 2) + (playerList[i].buttonLifePlusOne.getGlobalBounds().width / 2) + 16;
		float smallBuffer = playerList[i].buttonLifePlusOne.getGlobalBounds().width + 16;
		playerList[i].buttonLifePlusOne.setPosition(playerList[i].lifeCounter.getPosition().x + largeBuffer, playerList[i].lifeCounter.getPosition().y);
		playerList[i].buttonLifePlusFive.setPosition(playerList[i].buttonLifePlusOne.getPosition().x + smallBuffer, playerList[i].lifeCounter.getPosition().y);
		playerList[i].buttonLifeMinusFive.setPosition(playerList[i].lifeCounter.getPosition().x - largeBuffer, playerList[i].lifeCounter.getPosition().y);
		playerList[i].buttonLifeMinusTen.setPosition(playerList[i].buttonLifeMinusFive.getPosition().x - smallBuffer, playerList[i].lifeCounter.getPosition().y);
		//Position the Poison Counter.
		//If this is not a Commander game, center the Counter.
		if (gameTypeID != 1)
		{
			playerList[i].poisonCounter.setPosition(playerList[i].lifeCounter.getPosition().x, playerList[i].lifeCounter.getPosition().y + playerList[i].lifeCounter.getGlobalBounds().height);
		}
		//Otherwise, position it slightly to the left.
		else
		{
			playerList[i].poisonCounter.setPosition(playerList[i].lifeCounter.getPosition().x - (playerList[i].lifeCounter.getGlobalBounds().width / 2), playerList[i].lifeCounter.getPosition().y + playerList[i].lifeCounter.getGlobalBounds().height);
		}
		//Position the Poison Buttons.
		playerList[i].buttonPoisonPlusOne.setPosition(playerList[i].poisonCounter.getPosition().x + smallBuffer, playerList[i].poisonCounter.getPosition().y);
		playerList[i].buttonPoisonMinusOne.setPosition(playerList[i].poisonCounter.getPosition().x - smallBuffer, playerList[i].poisonCounter.getPosition().y);
		//Position the Commander objects, if this is a Commander Game.
		if (gameTypeID == 1)
		{
			//If the Counters are not expanded, position the Counters and Avatars in the same place, next to one another.
			if (!playerList[i].cmdrCountersExpanded)
			{
				for (int j = 0; j < (int)playerList[i].cmdrDmgCounters.size(); j++)
				{
					playerList[i].cmdrDmgCounters[j].setPosition(playerList[i].lifeCounter.getPosition().x + (playerList[i].lifeCounter.getGlobalBounds().width / 2), playerList[i].lifeCounter.getPosition().y + playerList[i].lifeCounter.getGlobalBounds().height);
					playerList[i].cmdrAvatars[j].setPosition(playerList[i].cmdrDmgCounters[j].getPosition().x - (playerList[i].cmdrAvatars[j].getGlobalBounds().width * 1.5f), playerList[i].cmdrDmgCounters[j].getPosition().y - playerList[i].cmdrDmgCounters[j].getLocalBounds().top);
					playerList[i].cmdrButtons[j].setPosition(playerList[i].cmdrDmgCounters[j].getPosition().x - playerList[i].cmdrDmgCounters[j].getLocalBounds().left, playerList[i].cmdrDmgCounters[j].getPosition().y + (playerList[i].cmdrDmgCounters[j].getGlobalBounds().height / 2) + playerList[i].cmdrDmgCounters[j].getLocalBounds().top);
				}
			}
			//Otherwise, position the Counters next to each other, and the Avatars behind them.
			else
			{
				for (int j = 0; j < (int)playerList[i].cmdrDmgCounters.size(); j++)
				{
					playerList[i].cmdrDmgCounters[j].setPosition(playerList[i].cmdrCloseButton.getPosition().x - (playerList[i].cmdrDmgCounters[j].getGlobalBounds().width * ((playerList[i].cmdrDmg.size() - 1) / 2)) + (playerList[i].cmdrDmgCounters[j].getGlobalBounds().width * j),
						playerList[i].cmdrCloseButton.getPosition().y + playerList[i].cmdrCloseButton.getGlobalBounds().height);
					playerList[i].cmdrAvatars[j].setPosition(playerList[i].cmdrDmgCounters[j].getPosition());
					playerList[i].cmdrButtons[j].setPosition(playerList[i].cmdrDmgCounters[j].getPosition().x, playerList[i].cmdrDmgCounters[j].getPosition().y + (playerList[i].cmdrDmgCounters[j].getGlobalBounds().height / 2));
				}
			}
			playerList[i].cmdrCloseButton.setPosition(playerList[i].cmdrDmgCounters[0].getPosition());
		}
		//Position the Display Name(s) and Avatar(s).
		switch (teamSizeID)
		{
		case 0:
			playerList[i].displayName->setPosition(playerList[i].lifeCounter.getPosition().x - (playerList[i].lifeCounter.getGlobalBounds().width / 2), playerList[i].lifeCounter.getPosition().y - playerList[i].lifeCounter.getGlobalBounds().height);
			playerList[i].avatar.setPosition(playerList[i].displayName->getAbsolutePosition().x - (playerList[i].avatar.getGlobalBounds().width * 1.5f), playerList[i].displayName->getAbsolutePosition().y - (playerList[i].displayName->getFullSize().y / 4));
			break;
		case 1:
			playerList[i].displayName->setPosition(playerList[i].lifeCounter.getPosition().x - (playerList[i].lifeCounter.getGlobalBounds().width * .75), playerList[i].lifeCounter.getPosition().y - playerList[i].lifeCounter.getGlobalBounds().height);
			playerList[i].avatar.setPosition(playerList[i].displayName->getAbsolutePosition().x + ((playerList[i].displayName->getFullSize().x / 2) - playerList[i].avatar.getGlobalBounds().width / 2), playerList[i].displayName->getAbsolutePosition().y - (playerList[i].avatar.getGlobalBounds().height * 1.2f));
			playerList[i].partnerDisplayNameOne->setPosition(playerList[i].lifeCounter.getPosition().x + (playerList[i].lifeCounter.getGlobalBounds().width * .75), playerList[i].lifeCounter.getPosition().y - playerList[i].lifeCounter.getGlobalBounds().height);
			playerList[i].partnerAvatarOne.setPosition(playerList[i].partnerDisplayNameOne->getAbsolutePosition().x + ((playerList[i].partnerDisplayNameOne->getFullSize().x / 2) - playerList[i].partnerAvatarOne.getGlobalBounds().width / 2), playerList[i].partnerDisplayNameOne->getAbsolutePosition().y - (playerList[i].partnerAvatarOne.getGlobalBounds().height * 1.2f));
			if (remoteGameID != -1)
			{
				playerList[i].teamDisplayName->setPosition(playerList[i].displayName->getPosition().x, playerList[i].displayName->getPosition().y - playerList[i].displayName->getTextSize() * 5);
			}
			break;
		case 2:
			playerList[i].partnerDisplayNameOne->setPosition(playerList[i].lifeCounter.getPosition().x + (playerList[i].partnerDisplayNameOne->getFullSize().x / 2), playerList[i].lifeCounter.getPosition().y - playerList[i].lifeCounter.getGlobalBounds().height);
			playerList[i].partnerAvatarOne.setPosition(playerList[i].partnerDisplayNameOne->getAbsolutePosition().x + ((playerList[i].partnerDisplayNameOne->getFullSize().x / 2) - playerList[i].partnerAvatarOne.getGlobalBounds().width / 2), playerList[i].partnerDisplayNameOne->getAbsolutePosition().y - (playerList[i].partnerAvatarOne.getGlobalBounds().height * 1.2f));
			playerList[i].displayName->setPosition(playerList[i].partnerDisplayNameOne->getPosition().x - (playerList[i].displayName->getTextSize() * 3) - (playerList[i].displayName->getFullSize().x / 2), playerList[i].partnerDisplayNameOne->getPosition().y);
			playerList[i].avatar.setPosition(playerList[i].displayName->getAbsolutePosition().x + ((playerList[i].displayName->getFullSize().x / 2) - playerList[i].avatar.getGlobalBounds().width / 2), playerList[i].displayName->getAbsolutePosition().y - (playerList[i].avatar.getGlobalBounds().height * 1.2f));
			playerList[i].partnerDisplayNameTwo->setPosition(playerList[i].partnerDisplayNameOne->getPosition().x + (playerList[i].displayName->getTextSize() * 3) + (playerList[i].partnerDisplayNameTwo->getFullSize().x / 2), playerList[i].partnerDisplayNameOne->getPosition().y);
			playerList[i].partnerAvatarTwo.setPosition(playerList[i].partnerDisplayNameTwo->getAbsolutePosition().x + ((playerList[i].partnerDisplayNameTwo->getFullSize().x / 2) - playerList[i].partnerAvatarTwo.getGlobalBounds().width / 2), playerList[i].partnerDisplayNameTwo->getAbsolutePosition().y - (playerList[i].partnerAvatarTwo.getGlobalBounds().height * 1.2f));
			if (remoteGameID != -1)
			{
				playerList[i].teamDisplayName->setPosition(playerList[i].partnerDisplayNameOne->getPosition().x, playerList[i].partnerDisplayNameOne->getPosition().y - playerList[i].partnerDisplayNameOne->getTextSize() * 5);
			}
			break;
		}
		//Set the position and dimensions of this Player's bounding box.
		createBoundingBox(i);
	}
	//Shift the Players around to prevent overlap.
	//shiftPlayers();
//	std::this_thread::yield();
//	std::this_thread::sleep_for(tick);
//} while (background.getPosition() != sf::Vector2f(targetPosition));
}

void gamePanel::shiftPlayers()
{
	bool noOverlap = false;
	while (!noOverlap)
	{
		noOverlap = true;
		//Iterate through all of the Player widgets.
		for (int i = 0; i < (int)playerList.size(); i++)
		{
			//Check each of the widgets against the others.
			for (int j = 0; j < (int)playerList.size(); j++)
			{
				//Don't check a widget against itself.
				if (i != j)
				{
					//Check if the two Players' bounding boxes intersect.
					if (playerList[i].boundingBox.intersects(playerList[j].boundingBox))
					{
						noOverlap = false;
						//Figure out how to move the widgets.
						//Is Box "i" above and to the left of Box "j"?
						if (playerList[i].boundingBox.left < playerList[j].boundingBox.left && playerList[i].boundingBox.top < playerList[j].boundingBox.top)
						{
							//If the boxes overlap horizontally more than they do vertically, or there is more vertical room, move the Players vertically.
							if (playerList[j].boundingBox.top - playerList[i].boundingBox.top < playerList[j].boundingBox.left - playerList[i].boundingBox.left || background.getGlobalBounds().width < background.getGlobalBounds().height)
							{
								movePlayer(i, 0, (playerList[j].boundingBox.top - (playerList[i].boundingBox.top + playerList[i].boundingBox.height)) / 2);
								movePlayer(j, 0, -((playerList[j].boundingBox.top - (playerList[i].boundingBox.top + playerList[i].boundingBox.height)) / 2));
							}
							//Otherwise, move the Players horizontally.
							else
							{
								movePlayer(i, (playerList[j].boundingBox.left - (playerList[i].boundingBox.left + playerList[i].boundingBox.width)) / 2, 0);
								movePlayer(j, -((playerList[j].boundingBox.left - (playerList[i].boundingBox.left + playerList[i].boundingBox.width)) / 2), 0);
							}
						}
						//Is Box "i" above and to the right of (or directly above) Box "j"?
						else if (playerList[i].boundingBox.left >= playerList[j].boundingBox.left && playerList[i].boundingBox.top < playerList[j].boundingBox.top)
						{
							//If the boxes overlap horizontally more than they do vertically, or there is more vertical room, move the Players vertically.
							if (playerList[j].boundingBox.top - playerList[i].boundingBox.top < playerList[i].boundingBox.left - playerList[j].boundingBox.left || background.getGlobalBounds().width < background.getGlobalBounds().height)
							{
								movePlayer(i, 0, (playerList[j].boundingBox.top - (playerList[i].boundingBox.top + playerList[i].boundingBox.height)) / 2);
								movePlayer(j, 0, -((playerList[j].boundingBox.top - (playerList[i].boundingBox.top + playerList[i].boundingBox.height)) / 2));
							}
							//Otherwise, move the Players horizontally.
							else
							{
								movePlayer(i, ((playerList[j].boundingBox.left + playerList[j].boundingBox.width) - playerList[i].boundingBox.left) / 2, 0);
								movePlayer(j, -(((playerList[j].boundingBox.left + playerList[j].boundingBox.width) - playerList[i].boundingBox.left) / 2), 0);
							}
						}
						//Is Box "i" below and to the right of (or directly below/to the right of) Box "j"?
						else if (playerList[i].boundingBox.left >= playerList[j].boundingBox.left && playerList[i].boundingBox.top >= playerList[j].boundingBox.top)
						{
							//If the boxes overlap horizontally more than they do vertically, or there is more vertical room, move the Players vertically.
							if (playerList[i].boundingBox.top - playerList[j].boundingBox.top < playerList[i].boundingBox.left - playerList[j].boundingBox.left || background.getGlobalBounds().width < background.getGlobalBounds().height)
							{
								movePlayer(i, 0, ((playerList[j].boundingBox.top + playerList[j].boundingBox.height) - playerList[i].boundingBox.top) / 2);
								movePlayer(j, 0, -(((playerList[j].boundingBox.top + playerList[j].boundingBox.height) - playerList[i].boundingBox.top) / 2));
							}
							//Otherwise, move the Players horizontally.
							else
							{
								movePlayer(i, ((playerList[j].boundingBox.left + playerList[j].boundingBox.width) - playerList[i].boundingBox.left) / 2, 0);
								movePlayer(j, -(((playerList[j].boundingBox.left + playerList[j].boundingBox.width) - playerList[i].boundingBox.left) / 2), 0);
							}
						}
						//Is Box "i" below and to the left of (or just to the left of) Box "j"?
						else if (playerList[i].boundingBox.left < playerList[j].boundingBox.left && playerList[i].boundingBox.top >= playerList[j].boundingBox.top)
						{
							//If the boxes overlap horizontally more than they do vertically, or there is more vertical room, move the Players vertically.
							if (playerList[i].boundingBox.top - playerList[j].boundingBox.top < playerList[i].boundingBox.left - playerList[j].boundingBox.left || background.getGlobalBounds().width < background.getGlobalBounds().height)
							{
								movePlayer(i, 0, ((playerList[j].boundingBox.top + playerList[j].boundingBox.height) - playerList[i].boundingBox.top) / 2);
								movePlayer(j, 0, -(((playerList[j].boundingBox.top + playerList[j].boundingBox.height) - playerList[i].boundingBox.top) / 2));
							}
							//Otherwise, move the Players horizontally.
							else
							{
								movePlayer(i, (playerList[j].boundingBox.left - (playerList[i].boundingBox.left + playerList[i].boundingBox.width) / 2), 0);
								movePlayer(j, -((playerList[j].boundingBox.left - (playerList[i].boundingBox.left + playerList[i].boundingBox.width) / 2)), 0);
							}
						}
					}
				}
			}
			//If a widget is partially offscreen, move it further into the panel.
			//Left Edge:
			if (playerList[i].boundingBox.left < background.getPosition().x)
			{
				noOverlap = false;
				movePlayer(i, background.getPosition().x - playerList[i].boundingBox.left, 0);
			}
			//Right Edge:
			else if ((playerList[i].boundingBox.left + playerList[i].boundingBox.width) > (background.getPosition().x + background.getGlobalBounds().width))
			{
				noOverlap = false;
				movePlayer(i, (background.getPosition().x + background.getGlobalBounds().width) - (playerList[i].boundingBox.left + playerList[i].boundingBox.width), 0);
			}
			//Top Edge:
			if (playerList[i].boundingBox.top < background.getPosition().y)
			{
				noOverlap = false;
				movePlayer(i, 0, background.getPosition().y - playerList[i].boundingBox.top);
			}
			//Bottom Edge:
			else if ((playerList[i].boundingBox.top + playerList[i].boundingBox.height) > (background.getPosition().y + background.getGlobalBounds().height))
			{
				noOverlap = false;
				movePlayer(i, 0, (background.getPosition().y + background.getGlobalBounds().height) - (playerList[i].boundingBox.top + playerList[i].boundingBox.height));
			}
		}
	}
}

void gamePanel::movePlayer(int player, float xDiff, float yDiff)
{
	//Move the Bounding Box.
	playerList[player].boundingBox.left += xDiff;
	playerList[player].boundingBox.top += yDiff;
	//Move the Life Counter.
	playerList[player].lifeCounter.move(xDiff, yDiff);
	//Move the Poison Counter.
	playerList[player].poisonCounter.move(xDiff, yDiff);
	//Move the Display Name.
	playerList[player].displayName->move(xDiff, yDiff);
	//Move the Avatar.
	playerList[player].avatar.move(xDiff, yDiff);
	//Move the Local Game-exclusive objects.
	if (remoteGameID < 0)
	{
		//Move the "Life -10" Button.
		playerList[player].buttonLifeMinusTen.move(xDiff, yDiff);
		//Move the "Life -5" Button.
		playerList[player].buttonLifeMinusFive.move(xDiff, yDiff);
		//Move the "Life +1" Button.
		playerList[player].buttonLifePlusOne.move(xDiff, yDiff);
		//Move the "Life +5" Button.
		playerList[player].buttonLifePlusFive.move(xDiff, yDiff);
		//Move the "Poison -1" Button.
		playerList[player].buttonPoisonMinusOne.move(xDiff, yDiff);
		//Move the "Poison +1" Button.
		playerList[player].buttonPoisonPlusOne.move(xDiff, yDiff);
	}
	//Move the Commander Game-exclusive objects.
	if (gameTypeID == 1)
	{
		//Move the Commander "Close" Button.
		playerList[player].cmdrCloseButton.move(xDiff, yDiff);
		for (int i = 0; i < (int)playerList.size(); i++)
		{
			//Move all of the Commander Avatars.
			playerList[player].cmdrAvatars[i].move(xDiff, yDiff);
			//Move all of the Commander Counters.
			playerList[player].cmdrDmgCounters[i].move(xDiff, yDiff);
			//If this is a Local Game, move the Commander Buttons.
			if (remoteGameID < 0)
			{
				playerList[player].cmdrButtons[i].move(xDiff, yDiff);
			}
		}
	}
	//Move the 1st Partner's objects.
	if (teamSizeID > 0)
	{
		//Move the 1st Partner's Display Name.
		playerList[player].partnerDisplayNameOne->move(xDiff, yDiff);
		//Move the 1st Partner's Avatar.
		playerList[player].partnerAvatarOne.move(xDiff, yDiff);
		//If this is a Remote Game, move the Team Display Name.
		if (remoteGameID >= 0)
		{
			playerList[player].teamDisplayName->move(xDiff, yDiff);
		}
		//Move the 2nd Partner's objects.
		if (teamSizeID == 2)
		{
			//Move the 2nd Partner's Display Name.
			playerList[player].partnerDisplayNameTwo->move(xDiff, yDiff);
			//Move the 2nd Partner's Avatar.
			playerList[player].partnerAvatarTwo.move(xDiff, yDiff);
		}
	}
}

void gamePanel::setTargetSize(int width, int height)
{
	targetSize = Vector2i(width, height);
	//if (panelResizeThread.joinable()) { panelResizeThread.join(); }
	//panelResizeThread = std::thread(std::bind(&gamePanel::resize, this, width, height));
	resize(width, height);
}

//Resize the Panel and all of its visual elements.
	//width: The new width of the Panel as dictated by the program.
	//height: The new height of the Panel as dictated by the program.
void gamePanel::resize(int width, int height)
{
	//width = background.getGlobalBounds().width;
	//height = background.getGlobalBounds().height;
	//while (background.getGlobalBounds().width != targetSize.x || background.getGlobalBounds().height != targetSize.y)
	//{
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
	//Reduce the Life Counter Size by 16 for every Player in the game after the 2nd.
	playerList[0].lifeCounter.setCharacterSize(playerList[0].lifeCounter.getCharacterSize() - (16 * (playerList.size() - 2)));
	//Resize the visual elements for each player, and reset their Origins.
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		//Resize the rest of the Life Counters.
		playerList[i].lifeCounter.setCharacterSize(playerList[0].lifeCounter.getCharacterSize());
		//Resize the Display Names and Avatars.
		playerList[i].displayName->setTextSize((int)(playerList[i].lifeCounter.getCharacterSize() / 6));
		playerList[i].displayName->setSize(playerList[i].displayName->getTextSize() * playerList[i].name.length(), playerList[i].displayName->getTextSize() + 2);
		playerList[i].avatar.setScale((playerList[i].displayName->getFullSize().y * 2) / playerList[i].avatar.getTexture()->getSize().x, (playerList[i].displayName->getFullSize().y * 2) / playerList[i].avatar.getTexture()->getSize().y);
		if (teamSizeID > 0)
		{
			playerList[i].partnerDisplayNameOne->setTextSize(playerList[i].displayName->getTextSize());
			playerList[i].partnerDisplayNameOne->setSize(playerList[i].partnerDisplayNameOne->getTextSize() * playerList[i].partnerNameOne.length(), playerList[i].partnerDisplayNameOne->getTextSize() + 2);
			playerList[i].partnerAvatarOne.setScale((playerList[i].partnerDisplayNameOne->getFullSize().y * 2) / playerList[i].partnerAvatarOne.getTexture()->getSize().x, (playerList[i].partnerDisplayNameOne->getFullSize().y * 2) / playerList[i].partnerAvatarOne.getTexture()->getSize().y);
			//Only set up the Team Name if this is a remote game, for now.
			if (remoteGameID != -1)
			{
				playerList[i].teamDisplayName->setTextSize(playerList[i].displayName->getTextSize() + 6);
				playerList[i].teamDisplayName->setSize(playerList[i].teamDisplayName->getTextSize() * playerList[i].teamName.length(), playerList[i].teamDisplayName->getTextSize() + 2);
			}
		}
		if (teamSizeID == 2)
		{
			playerList[i].partnerDisplayNameTwo->setTextSize(playerList[i].displayName->getTextSize());
			playerList[i].partnerDisplayNameTwo->setSize(playerList[i].partnerDisplayNameTwo->getTextSize() * playerList[i].partnerNameTwo.length(), playerList[i].partnerDisplayNameTwo->getTextSize() + 2);
			playerList[i].partnerAvatarTwo.setScale((playerList[i].partnerDisplayNameTwo->getFullSize().y * 2) / playerList[i].partnerAvatarTwo.getTexture()->getSize().x, (playerList[i].partnerDisplayNameTwo->getFullSize().y * 2) / playerList[i].partnerAvatarTwo.getTexture()->getSize().y);
		}
		//Resize all of the Life Buttons.
		playerList[i].buttonLifePlusOne.scale((playerList[i].lifeCounter.getGlobalBounds().height / 2) / playerList[i].buttonLifePlusOne.getGlobalBounds().height,
			(playerList[i].lifeCounter.getGlobalBounds().height / 2) / playerList[i].buttonLifePlusOne.getGlobalBounds().height);
		playerList[i].buttonLifePlusFive.setScale(playerList[i].buttonLifePlusOne.getScale());
		playerList[i].buttonLifeMinusFive.setScale(playerList[i].buttonLifePlusOne.getScale());
		playerList[i].buttonLifeMinusTen.setScale(playerList[i].buttonLifePlusOne.getScale());
		//Resize each of the Poison Counters.
		playerList[i].poisonCounter.setCharacterSize((int)(playerList[i].lifeCounter.getCharacterSize() / 2));
		//Resize all of the Poison Buttons.
		playerList[i].buttonPoisonMinusOne.scale(playerList[i].poisonCounter.getGlobalBounds().height / playerList[i].buttonPoisonMinusOne.getGlobalBounds().height,
			playerList[i].poisonCounter.getGlobalBounds().height / playerList[i].buttonPoisonMinusOne.getGlobalBounds().height);
		playerList[i].buttonPoisonPlusOne.setScale(playerList[i].buttonPoisonMinusOne.getScale());
		//Resize all of the Commander Damage objects and set their origins, if this is a Commander game.
		if (gameTypeID == 1)
		{
			for (int j = 0; j < (int)playerList[i].cmdrDmgCounters.size(); j++)
			{
				playerList[i].cmdrDmgCounters[j].setCharacterSize(playerList[i].poisonCounter.getCharacterSize());
				playerList[i].cmdrDmgCounters[j].setOrigin(playerList[i].cmdrDmgCounters[j].getLocalBounds().left + (playerList[i].cmdrDmgCounters[j].getGlobalBounds().width / 2),
					playerList[i].cmdrDmgCounters[j].getLocalBounds().top + (playerList[i].cmdrDmgCounters[j].getGlobalBounds().height / 2));
				playerList[i].cmdrAvatars[j].scale((playerList[i].cmdrDmgCounters[j].getGlobalBounds().height * .75f) / playerList[i].cmdrAvatars[j].getGlobalBounds().width,
					(playerList[i].cmdrDmgCounters[j].getGlobalBounds().height * .75f) / playerList[i].cmdrAvatars[j].getGlobalBounds().height);
				playerList[i].cmdrAvatars[j].setOrigin(playerList[i].cmdrAvatars[j].getGlobalBounds().width / 2, playerList[i].cmdrAvatars[j].getGlobalBounds().height / 2);
				playerList[i].cmdrButtons[j].setScale(playerList[i].buttonLifePlusOne.getScale() * .75f);
				playerList[i].cmdrButtons[j].setOrigin(playerList[i].cmdrButtons[j].getGlobalBounds().width / 2, playerList[i].cmdrButtons[j].getGlobalBounds().height / 2);
			}
			playerList[i].cmdrCloseButton.setOrigin(playerList[i].cmdrCloseButton.getGlobalBounds().width / 2, playerList[i].cmdrCloseButton.getGlobalBounds().height / 2);
			playerList[i].cmdrCloseButton.setScale(playerList[i].cmdrDmgCounters[0].getScale() * .8f);
		}
		//Set the origin points for each of the visual elements (except the Display Names) to their centers.
		playerList[i].lifeCounter.setOrigin(playerList[i].lifeCounter.getLocalBounds().left + (playerList[i].lifeCounter.getGlobalBounds().width / 2),
			playerList[i].lifeCounter.getLocalBounds().top + (playerList[i].lifeCounter.getGlobalBounds().height / 2));
		playerList[i].buttonLifePlusOne.setOrigin(playerList[i].buttonLifePlusOne.getGlobalBounds().width / 2, playerList[i].buttonLifePlusOne.getGlobalBounds().height / 2);
		playerList[i].buttonLifePlusFive.setOrigin(playerList[i].buttonLifePlusFive.getGlobalBounds().width / 2, playerList[i].buttonLifePlusFive.getGlobalBounds().height / 2);
		playerList[i].buttonLifeMinusFive.setOrigin(playerList[i].buttonLifeMinusFive.getGlobalBounds().width / 2, playerList[i].buttonLifeMinusFive.getGlobalBounds().height / 2);
		playerList[i].buttonLifeMinusTen.setOrigin(playerList[i].buttonLifeMinusTen.getGlobalBounds().width / 2, playerList[i].buttonLifeMinusTen.getGlobalBounds().height / 2);
		playerList[i].poisonCounter.setOrigin(playerList[i].poisonCounter.getLocalBounds().left + (playerList[i].poisonCounter.getGlobalBounds().width / 2),
			playerList[i].poisonCounter.getLocalBounds().top + (playerList[i].poisonCounter.getGlobalBounds().height / 2));
		playerList[i].buttonPoisonPlusOne.setOrigin(playerList[i].buttonPoisonPlusOne.getGlobalBounds().width / 2, playerList[i].buttonPoisonPlusOne.getGlobalBounds().height / 2);
		playerList[i].buttonPoisonMinusOne.setOrigin(playerList[i].buttonPoisonMinusOne.getGlobalBounds().width / 2, playerList[i].buttonPoisonMinusOne.getGlobalBounds().height / 2);
	}
	/*//Change the width and height values.
	//First, check if the background width is greater than its target.
	if (targetSize.x < background.getGlobalBounds().width)
	{
		if (background.getGlobalBounds().width - targetSize.x > 20) { width -= 20; }
		else { width -= (background.getGlobalBounds().width - targetSize.x); }
	}
	//Second, check if the background width is less than its target.
	else if (background.getGlobalBounds().width < targetSize.x)
	{
		if (targetSize.x - background.getGlobalBounds().width > 20) { width += 20; }
		else { width += (targetSize.x - background.getGlobalBounds().width); }
	}
	//Third, check if the background height is greater than its target.
	if (targetSize.y < background.getGlobalBounds().height)
	{
		if (background.getGlobalBounds().height - targetSize.y > 20) { height -= 20; }
		else { height -= (background.getGlobalBounds().height - targetSize.y); }
	}
	//Fourth, check if the background width is less than its target.
	else if (background.getGlobalBounds().height < targetSize.y)
	{
		if (targetSize.y - background.getGlobalBounds().height > 20) { height += 20; }
		else { height += (targetSize.y - background.getGlobalBounds().height); }
	}
	std::this_thread::yield();
	std::this_thread::sleep_for(tick);*/
	//}
}

//Check the Panel for user input.
void gamePanel::getUserInput(sf::Event event)
{
	gui->handleEvent(event);
	//Check for Left Mouse Clicks.
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			//Get the Mouse position.
			mousePos = sf::Vector2f(sf::Mouse::getPosition());
			//Check the buttons for all of the Players.
			for (int i = 0; i < (int)playerList.size(); i++)
			{
				//Check the Life Counter.
				if (playerList[i].lifeCounter.getGlobalBounds().contains(mousePos) && remoteGameID == -1)
				{
					UpdateLife(i, -1);
				}
				//Check the "Life Plus One" Button.
				else if (playerList[i].buttonLifePlusOne.getGlobalBounds().contains(mousePos))
				{
					UpdateLife(i, 1);
				}
				//Check the "Life Plus Five" Button.
				else if (playerList[i].buttonLifePlusFive.getGlobalBounds().contains(mousePos))
				{
					UpdateLife(i, 5);
				}
				//Check the "Life Minus Five" Button.
				else if (playerList[i].buttonLifeMinusFive.getGlobalBounds().contains(mousePos))
				{
					UpdateLife(i, -5);
				}
				//Check the "Life Minus Ten" Button.
				else if (playerList[i].buttonLifeMinusTen.getGlobalBounds().contains(mousePos))
				{
					UpdateLife(i, -10);
				}
				//Check the "Poison Plus One" Button.
				else if (playerList[i].buttonPoisonPlusOne.getGlobalBounds().contains(mousePos))
				{
					UpdatePoison(i, 1);
				}
				//Check the "Poison Minus One" Button.
				else if (playerList[i].buttonPoisonMinusOne.getGlobalBounds().contains(mousePos))
				{
					UpdatePoison(i, -1);
				}
				//Check the "Commander Close" Button.
				else if (playerList[i].cmdrCloseButton.getGlobalBounds().contains(mousePos) && remoteGameID == -1)
				{
					//If this player's Damage Counters are expanded, collapse them.
					if (playerList[i].cmdrCountersExpanded)
					{
						if (playerExpandThread.joinable()) { playerExpandThread.join(); }
						playerExpandThread = std::thread(std::bind(&gamePanel::PlayerCollapseCoroutine, this, i));
					}
				}
				for (int j = 0; j < (int)playerList[i].cmdrDmgCounters.size(); j++)
				{
					//Check all of the "Commander Damage Counters" objects.
					if (playerList[i].cmdrDmgCounters[j].getGlobalBounds().contains(mousePos) && remoteGameID == -1)
					{
						//If the Counters are already fully expanded, change the Commander Damage total.
						if (playerList[i].cmdrCountersExpanded && playerList[i].cmdrCloseButton.getColor().a == 255)
						{
							UpdateCommander(i, j, 1);
						}
						//Otherwise, expand them.
						else if (!playerList[i].cmdrCountersExpanded)
						{
							if (playerExpandThread.joinable()) { playerExpandThread.join(); }
							playerExpandThread = std::thread(std::bind(&gamePanel::PlayerExpandCoroutine, this, i));
							playerList[i].cmdrCountersExpanded = true;
						}
					}
					//Check all of the "Commander Minus One" buttons.
					if (playerList[i].cmdrButtons[j].getGlobalBounds().contains(mousePos) && playerList[i].cmdrCountersExpanded)
					{
						UpdateCommander(i, j, -1);
					}
				}
			}
			//Check the "Add Player" Button.
			if (addPlayerButton.getGlobalBounds().contains(mousePos))
			{
				//Only do something if there are less than 10 Players.
				if ((int)playerList.size() < MAX_PLAYERS)
				{
					AddPlayer();
				}
			}
		}
	}
	//Check the Commander Damage Counters for Mouse-hover.
	else if (event.type == sf::Event::MouseMoved)
	{
		if (gameTypeID == 1)
		{
			//Reset the "Mouse hover" flag.
			hoverTextActive = false;
			//Get the Mouse position.
			mousePos = sf::Vector2f(sf::Mouse::getPosition());
			for (int i = 0; i < (int)playerList.size(); i++)
			{
				for (int j = 0; j < (int)playerList[i].cmdrDmgCounters.size(); j++)
				{
					//Only acknowledge the Counters that are visible, for when they are collapsed.
					if (playerList[i].cmdrAvatars[j].getGlobalBounds().contains(mousePos) && playerList[i].cmdrDmgCounters[j].getFillColor().a > 0)
					{
						//If the hover text's text already matches the player's, it doesn't need to be set up again.
						if (cmdrHoverText->getText() != playerList[j].displayName->getText())
						{
							//Pull all of the style values from the display name of the player.
							cmdrHoverText->getRenderer()->setBorders(tgui::Borders(0));
							cmdrHoverText->setFont(playerList[j].displayName->getFont());
							cmdrHoverText->getRenderer()->setTextColor(playerList[j].displayName->getRenderer()->getProperty("TextColor").getColor());
							cmdrHoverText->getRenderer()->setBackgroundColor(playerList[j].displayName->getRenderer()->getProperty("BackgroundColor").getColor());
							cmdrHoverText->setText(playerList[j].displayName->getText());
							cmdrHoverText->setTextSize(playerList[j].displayName->getTextSize());
						}
						//Make the hover text follow the mouse cursor.
						cmdrHoverText->setPosition(mousePos);
						hoverTextActive = true;
					}
				}
			}
			//If none of the Counters are being hovered over, hide the Hover Text.
			if (!hoverTextActive)
			{
				cmdrHoverText->setText("");
				cmdrHoverText->getRenderer()->setBackgroundColor(sf::Color::Transparent);
				cmdrHoverText->getRenderer()->setTextColor(sf::Color::Transparent);
			}
		}
	}
}

//Add a new Player to a Local Game.
void gamePanel::AddPlayer()
{
	//Create a new Player object.
	player newPlayer;
	playerList.push_back(newPlayer);
	playerList[playerList.size() - 1].id = -1;
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
	playerList[playerList.size() - 1].startingLife = playerList[0].startingLife;
	playerList[playerList.size() - 1].life = playerList[playerList.size() - 1].startingLife;
	playerList[playerList.size() - 1].lifeCounter.setString(std::to_string(playerList[playerList.size() - 1].life));
	playerList[playerList.size() - 1].handshake = true;
	//Set up the Poison Counter (except size and position).
	playerList[playerList.size() - 1].poisonCounter.setFont(*font_dejavu);
	playerList[playerList.size() - 1].poisonCounter.setFillColor(sf::Color::Black);
	playerList[playerList.size() - 1].startingPoison = 0;
	playerList[playerList.size() - 1].poison = playerList[playerList.size() - 1].startingPoison;
	playerList[playerList.size() - 1].poisonCounter.setString(std::to_string(playerList[playerList.size() - 1].poison));
	//Set up the Avatar.
	sf::Texture* tempTex = new sf::Texture();
	tempTex->loadFromFile(avatarPath + "player_images/default.jpg");
	playerList[playerList.size() - 1].avatar.setTexture(*tempTex);
	//Set up the Commander Damage Objects (except size and position).
	playerList[playerList.size() - 1].cmdrCountersExpanded = false;
	playerList[playerList.size() - 1].cmdrCloseButton = sf::Sprite(*buttonCommanderCloseTexture);
	playerList[playerList.size() - 1].cmdrCloseButton.setColor(sf::Color(255, 255, 255, 0));
	playerList[playerList.size() - 1].startingCmdrDmg = 0;
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		playerList[playerList.size() - 1].cmdrDmg.push_back(playerList[playerList.size() - 1].startingCmdrDmg);
		playerList[playerList.size() - 1].cmdrDmgCounters.push_back(sf::Text(std::to_string(playerList[playerList.size() - 1].cmdrDmg[i]), *font_dejavu));
		playerList[playerList.size() - 1].cmdrDmgCounters[i].setFillColor(sf::Color::Black);
		//If this is a player from the database, load their Avatar.
		if (playerList[i].id >= 0)
		{
			playerList[playerList.size() - 1].cmdrAvatars.push_back(sf::Sprite(*avatarTextures[playerList[i].id]));
		}
		//Otherwise, load the default.
		else
		{
			playerList[playerList.size() - 1].cmdrAvatars.push_back(sf::Sprite(*tempTex));
		}
		playerList[playerList.size() - 1].cmdrButtons.push_back(sf::Sprite(*buttonCommanderMinusOneTexture));
		//Make these Buttons transparent.
		playerList[playerList.size() - 1].cmdrButtons[i].setColor(sf::Color(255, 255, 255, 0));
		//Add an extra Commander Damage counter to the rest of the Players.
		if (i != (int)playerList.size() - 1)
		{
			playerList[i].cmdrDmg.push_back(playerList[i].startingCmdrDmg);
			playerList[i].cmdrDmgCounters.push_back(sf::Text(std::to_string(playerList[i].cmdrDmg[playerList.size() - 1]), *font_dejavu));
			playerList[i].cmdrDmgCounters[playerList.size() - 1].setFillColor(sf::Color::Black);
			//If this is a player from the database, load their Avatar.
			if (playerList[playerList.size() - 1].id >= 0)
			{
				playerList[i].cmdrAvatars.push_back(sf::Sprite(*avatarTextures[playerList[playerList.size() - 1].id]));
			}
			//Otherwise, load the default.
			else
			{
				playerList[i].cmdrAvatars.push_back(sf::Sprite(*tempTex));
			}
			playerList[i].cmdrButtons.push_back(sf::Sprite(*buttonCommanderMinusOneTexture));
			playerList[i].cmdrButtons[playerList.size() - 1].setColor(sf::Color(255, 255, 255, 0));
			//Only make these Buttons transparent if the counters aren't expanded.
			if (!playerList[i].cmdrCountersExpanded) { playerList[i].cmdrButtons[playerList.size() - 1].setColor(sf::Color::Transparent); }
		}
	}
	//Set up the Name Text (except size and position).
	playerList[playerList.size() - 1].displayName = tgui::TextBox::create();
	gui->add(playerList[playerList.size() - 1].displayName);
	playerList[playerList.size() - 1].displayName->setFont(*font_mtg);
	playerList[playerList.size() - 1].displayName->setReadOnly(false);
	playerList[playerList.size() - 1].displayName->getRenderer()->setBackgroundColor(sf::Color::Transparent);
	playerList[playerList.size() - 1].displayName->getRenderer()->setBorderColor(sf::Color::Transparent);
	playerList[playerList.size() - 1].displayName->getRenderer()->setTextColor(sf::Color::Black);
	playerList[playerList.size() - 1].name = "Player " + std::to_string(playerList.size());
	playerList[playerList.size() - 1].displayName->setText(playerList[playerList.size() - 1].name);
	//If this is a Two- or Three-Headed Giant game, set up the 1st Partner Name Text.
	if (teamSizeID > 0)
	{
		playerList[playerList.size() - 1].partnerDisplayNameOne = tgui::TextBox::create();
		gui->add(playerList[playerList.size() - 1].partnerDisplayNameOne);
		playerList[playerList.size() - 1].partnerDisplayNameOne->setFont(*font_mtg);
		playerList[playerList.size() - 1].partnerDisplayNameOne->setReadOnly(false);
		playerList[playerList.size() - 1].partnerDisplayNameOne->getRenderer()->setBackgroundColor(sf::Color::Transparent);
		playerList[playerList.size() - 1].partnerDisplayNameOne->getRenderer()->setBorderColor(sf::Color::Transparent);
		playerList[playerList.size() - 1].partnerDisplayNameOne->getRenderer()->setTextColor(sf::Color::Black);
		playerList[playerList.size() - 1].partnerNameOne = "Partner 1";
		playerList[playerList.size() - 1].partnerDisplayNameOne->setText(playerList[playerList.size() - 1].partnerNameOne);
		playerList[playerList.size() - 1].partnerAvatarOne.setTexture(*tempTex);
	}
	//If this is a Three-Headed Giant game, set up the 2nd Partner Name Text.
	if (teamSizeID == 2)
	{
		playerList[playerList.size() - 1].partnerDisplayNameTwo = tgui::TextBox::create();
		gui->add(playerList[playerList.size() - 1].partnerDisplayNameTwo);
		playerList[playerList.size() - 1].partnerDisplayNameTwo->setFont(*font_mtg);
		playerList[playerList.size() - 1].partnerDisplayNameTwo->setReadOnly(false);
		playerList[playerList.size() - 1].partnerDisplayNameTwo->getRenderer()->setBackgroundColor(sf::Color::Transparent);
		playerList[playerList.size() - 1].partnerDisplayNameTwo->getRenderer()->setBorderColor(sf::Color::Transparent);
		playerList[playerList.size() - 1].partnerDisplayNameTwo->getRenderer()->setTextColor(sf::Color::Black);
		playerList[playerList.size() - 1].partnerNameTwo = "Partner 2";
		playerList[playerList.size() - 1].partnerDisplayNameTwo->setText(playerList[playerList.size() - 1].partnerNameTwo);
		playerList[playerList.size() - 1].partnerAvatarTwo.setTexture(*tempTex);
	}
	//Resize the Panel.
	resize((int)background.getGlobalBounds().width, (int)background.getGlobalBounds().height);
	//Set the Panel's Position.
	setPosition((int)background.getPosition().x, (int)background.getPosition().y);
}

//Add a new Player to a remote Game.
void gamePanel::AddPlayer(std::string name, int life)
{
	//Create a new Player object.
	player newPlayer;
	playerList.push_back(newPlayer);
	//Set up the Life Counter (except size and position).
	playerList[playerList.size() - 1].lifeCounter.setFont(*font_dejavu);
	playerList[playerList.size() - 1].lifeCounter.setFillColor(sf::Color::Black);
	playerList[playerList.size() - 1].startingLife = life;
	playerList[playerList.size() - 1].life = playerList[playerList.size() - 1].startingLife;
	playerList[playerList.size() - 1].lifeCounter.setString(std::to_string(playerList[playerList.size() - 1].life));
	playerList[playerList.size() - 1].handshake = true;
	//Set up the Poison Counter (except size and position).
	playerList[playerList.size() - 1].poisonCounter.setFont(*font_dejavu);
	playerList[playerList.size() - 1].poisonCounter.setFillColor(sf::Color::Black);
	playerList[playerList.size() - 1].startingPoison = 0;
	playerList[playerList.size() - 1].poison = playerList[playerList.size() - 1].startingPoison;
	playerList[playerList.size() - 1].poisonCounter.setString(std::to_string(playerList[playerList.size() - 1].poison));
	//Set up the Commander Damage Counters (except size and position).
	playerList[playerList.size() - 1].startingCmdrDmg = 0;
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		playerList[playerList.size() - 1].cmdrDmg.push_back(playerList[playerList.size() - 1].startingCmdrDmg);
		playerList[playerList.size() - 1].cmdrDmgCounters.push_back(sf::Text(std::to_string(playerList[playerList.size() - 1].cmdrDmg[i]), *font_dejavu));
		playerList[playerList.size() - 1].cmdrDmgCounters[i].setFillColor(sf::Color::Black);
		playerList[playerList.size() - 1].cmdrAvatars.push_back(sf::Sprite(*avatarTextures[playerList[i].id]));
		//Add an extra Commander Damage counter to the rest of the Players.
		if (i != (int)playerList.size() - 1)
		{
			playerList[i].cmdrDmg.push_back(playerList[i].startingCmdrDmg);
			playerList[i].cmdrDmgCounters.push_back(sf::Text(std::to_string(playerList[i].cmdrDmg[playerList.size() - 1]), *font_dejavu));
			playerList[i].cmdrDmgCounters[playerList.size() - 1].setFillColor(sf::Color::Black);
			playerList[i].cmdrAvatars.push_back(sf::Sprite(*avatarTextures[playerList[playerList.size() - 1].id]));
		}
	}
	//Set up the Name Text (except size and position).
	playerList[playerList.size() - 1].displayName = tgui::TextBox::create();
	gui->add(playerList[playerList.size() - 1].displayName);
	playerList[playerList.size() - 1].displayName->setFont(*font_mtg);
	playerList[playerList.size() - 1].displayName->setReadOnly(false);
	playerList[playerList.size() - 1].displayName->getRenderer()->setBackgroundColor(sf::Color::Transparent);
	playerList[playerList.size() - 1].displayName->getRenderer()->setBorderColor(sf::Color::Transparent);
	playerList[playerList.size() - 1].displayName->getRenderer()->setTextColor(sf::Color::Black);
	playerList[playerList.size() - 1].name = name;
	playerList[playerList.size() - 1].displayName->setText(playerList[playerList.size() - 1].name);
	//Resize the Panel.
	resize((int)background.getGlobalBounds().width, (int)background.getGlobalBounds().height);
	//Set the Panel's Position.
	setPosition((int)background.getPosition().x, (int)background.getPosition().y);
}

//Draw the Game Panel's visual elements.
void gamePanel::draw()
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
		//Partner's Avatars:
		if (teamSizeID > 0)
		{
			window->draw(playerList[i].partnerAvatarOne);
		}
		if (teamSizeID == 2)
		{
			window->draw(playerList[i].partnerAvatarTwo);
		}
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
		//Commander Objects:
		if (gameTypeID == 1)
		{
			//If the Counters are not expanded, only draw the highest one.
			if (!playerList[i].cmdrCountersExpanded)
			{
				window->draw(playerList[i].cmdrDmgCounters[getHighestCmdrDmg(i)]);
				window->draw(playerList[i].cmdrAvatars[getHighestCmdrDmg(i)]);
			}
			//Otherwise, draw all of them.
			else
			{
				window->draw(playerList[i].cmdrCloseButton);
				for (int j = 0; j < (int)playerList[i].cmdrDmgCounters.size(); j++)
				{
					window->draw(playerList[i].cmdrAvatars[j]);
					window->draw(playerList[i].cmdrDmgCounters[j]);
					window->draw(playerList[i].cmdrButtons[j]);
				}
			}
		}
	}
	//Draw the "Add Player" Button (if there are fewer than 10 Players).
	if ((int)playerList.size() < MAX_PLAYERS)
	{
		window->draw(addPlayerButton);
	}
	//Draw the Gui objects (the Players' names)
	gui->draw();
}

void gamePanel::update()
{
	//Set each Player's "handshake" flag to false.
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		playerList[i].handshake = false;
	}
	//Pull the list of Teams in this game.
	res = stmt->executeQuery(sql::SQLString("SELECT team_id, name, life, player_id, partner1_id, partner2_id, emperor, poison, isAlive FROM teams WHERE game_id=").append(std::to_string(remoteGameID)).append(" ORDER BY team_id ASC"));
	sql::ResultSet *currentPlayer;
	sql::ResultSet *commanderDamage;
	while (res->next())
	{
		//Reset the flag that says whether this result has been used.
		currentPlayerUsed = false;
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
				if (!playerList[i].isAlive && playerList[i].lifeCounter.getFillColor().a == 255)
				{
					if (playerFadeThread.joinable()) { playerFadeThread.join(); }
					playerFadeThread = std::thread(std::bind(&gamePanel::PlayerFadeOutCoroutine, this, i));
				}
				else if (playerList[i].isAlive && playerList[i].lifeCounter.getFillColor().a == 128)
				{
					if (playerFadeThread.joinable()) { playerFadeThread.join(); }
					playerFadeThread = std::thread(std::bind(&gamePanel::PlayerFadeInCoroutine, this, i));
				}
				//If this is a Commander game, update the Commander Damage counters as well.
				if (gameTypeID == 1)
				{
					commanderDamage = stmt->executeQuery(sql::SQLString("SELECT from_id, to_id, life FROM commander_dmg WHERE game_id=").append(std::to_string(remoteGameID)).append(sql::SQLString(" AND from_id=")).append(std::to_string(res->getInt("player_id"))).append(sql::SQLString(" ORDER BY to_id ASC")));
					while (commanderDamage->next())
					{
						for (int j = 0; j < (int)playerList[i].cmdrDmgCounters.size(); j++)
						{
							if (playerList[j].id == commanderDamage->getInt("to_id"))
							{
								playerList[i].cmdrDmg[j] = commanderDamage->getInt("life");
								playerList[i].cmdrDmgCounters[j].setString(std::to_string(playerList[i].cmdrDmg[j]));
							}
						}
					}
				}
				//Set the Player's "handshake" to true when their stats get updated.
				playerList[i].handshake = true;
				//Set the flag that says this result has been used.
				currentPlayerUsed = true;
			}
		}
		//If this result hasn't updated a Player's stats, make a new Player using it.
		if (!currentPlayerUsed)
		{
			AddPlayer(currentPlayer->getString("name"), res->getInt("life"));
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
	if (gameTypeID == 1)
	{
		delete commanderDamage;
	}
}

//Determine which of a given Player's Commander Counters to draw (if they are collapsed) or to *not* fade out (if they are expanded).
int gamePanel::getHighestCmdrDmg(int player)
{
	//Create placeholder values to hold the highest counter's value and index.
	int temp = -1;
	int tempIndex = -1;
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		//If any counter's value is higher than the highest saved value, save its value and index.
		if (playerList[player].cmdrDmg[i] > temp)
		{
			temp = playerList[player].cmdrDmg[i];
			tempIndex = i;
		}
	}
	//Change the alpha values of all of the counters, for when they are expanded/collapsed.
	for (int i = 0; i < (int)playerList[player].cmdrDmgCounters.size(); i++)
	{
		//If this is the highest counter, make it fully opaque.
		if (i == tempIndex)
		{
			playerList[player].cmdrDmgCounters[i].setFillColor(sf::Color(0, 0, 0, 255));
		}
		//Otherwise, make it fully transparent.
		else if (!playerList[player].cmdrCountersExpanded)
		{
			playerList[player].cmdrDmgCounters[i].setFillColor(sf::Color(0, 0, 0, 0));
		}
	}
	return tempIndex;
}

//Set the "boundingBox" FloatRect for a given Player.
void gamePanel::createBoundingBox(int player)
{
	//If this is a Remote Game, only account for the Display Name and Counters.
	if (remoteGameID >= 0)
	{
		playerList[player].boundingBox.left = playerList[player].avatar.getGlobalBounds().left;
		playerList[player].boundingBox.width = (playerList[player].lifeCounter.getGlobalBounds().left + playerList[player].lifeCounter.getGlobalBounds().width) - playerList[player].boundingBox.left;
		playerList[player].boundingBox.top = playerList[player].avatar.getGlobalBounds().top;
		playerList[player].boundingBox.height = (playerList[player].poisonCounter.getGlobalBounds().top + playerList[player].poisonCounter.getGlobalBounds().height) - playerList[player].boundingBox.top;
	}
	//Otherwise, account for the Player Widget Buttons.
	else
	{
		playerList[player].boundingBox.left = playerList[player].buttonLifeMinusTen.getGlobalBounds().left;
		playerList[player].boundingBox.width = (playerList[player].buttonLifePlusFive.getGlobalBounds().left + playerList[player].buttonLifePlusFive.getGlobalBounds().width) - playerList[player].boundingBox.left;
		playerList[player].boundingBox.top = playerList[player].avatar.getGlobalBounds().top;
		//If this is a Commander game, add extra space to expand the counters.
		if (gameTypeID == 1)
		{
			playerList[player].boundingBox.height = (playerList[player].poisonCounter.getGlobalBounds().top + (playerList[player].poisonCounter.getGlobalBounds().height * 2.8f)) - playerList[player].boundingBox.top;
		}
		else
		{
			playerList[player].boundingBox.height = (playerList[player].poisonCounter.getGlobalBounds().top + playerList[player].poisonCounter.getGlobalBounds().height) - playerList[player].boundingBox.top;
		}
	}
}

//Fade all of a Player's visual objects to semi-transparency when they lose.
void gamePanel::PlayerFadeOutCoroutine(int playerID)
{
	//As long as the alpha value for the Player's objects is greater than 131, reduce it by 4 (132 - 4 = 128 = 50% opacity).
	while (playerList[playerID].lifeCounter.getFillColor().a > 131)
	{
		playerList[playerID].lifeCounter.setFillColor(sf::Color(playerList[playerID].lifeCounter.getFillColor().r, playerList[playerID].lifeCounter.getFillColor().g, playerList[playerID].lifeCounter.getFillColor().b, playerList[playerID].lifeCounter.getFillColor().a - 4));
		playerList[playerID].poisonCounter.setFillColor(sf::Color(playerList[playerID].poisonCounter.getFillColor().r, playerList[playerID].poisonCounter.getFillColor().g, playerList[playerID].poisonCounter.getFillColor().b, playerList[playerID].poisonCounter.getFillColor().a - 4));
		playerList[playerID].displayName->getRenderer()->setTextColor(playerList[playerID].lifeCounter.getFillColor());
		//Yield the thread for one frame, so the fade is visible.
		std::this_thread::yield();
		std::this_thread::sleep_for(tick);
	}
	//Set the objects' alpha values to exactly 128, in case there was any overshoot.
	playerList[playerID].lifeCounter.setFillColor(sf::Color(playerList[playerID].lifeCounter.getFillColor().r, playerList[playerID].lifeCounter.getFillColor().g, playerList[playerID].lifeCounter.getFillColor().b, 128));
	playerList[playerID].poisonCounter.setFillColor(sf::Color(playerList[playerID].poisonCounter.getFillColor().r, playerList[playerID].poisonCounter.getFillColor().g, playerList[playerID].poisonCounter.getFillColor().b, 128));
	playerList[playerID].displayName->getRenderer()->setTextColor(playerList[playerID].lifeCounter.getFillColor());
}

//Fade all of a Player's visual objects to full opacity, if their "loss" is undone.
void gamePanel::PlayerFadeInCoroutine(int playerID)
{
	//As long as the alpha value for the Player's objects is less than 252, increase it by 4 (251 + 4 = 255 = 100% opacity).
	while (playerList[playerID].lifeCounter.getFillColor().a < 252)
	{
		playerList[playerID].lifeCounter.setFillColor(sf::Color(playerList[playerID].lifeCounter.getFillColor().r, playerList[playerID].lifeCounter.getFillColor().g, playerList[playerID].lifeCounter.getFillColor().b, playerList[playerID].lifeCounter.getFillColor().a + 4));
		playerList[playerID].poisonCounter.setFillColor(sf::Color(playerList[playerID].poisonCounter.getFillColor().r, playerList[playerID].poisonCounter.getFillColor().g, playerList[playerID].poisonCounter.getFillColor().b, playerList[playerID].poisonCounter.getFillColor().a + 4));
		playerList[playerID].displayName->getRenderer()->setTextColor(playerList[playerID].lifeCounter.getFillColor());
		//Yield the thread for one frame, so the fade is visible.
		std::this_thread::yield();
		std::this_thread::sleep_for(tick);
	}
	//Set the objects' alpha values to exactly 255, in case there was any overshoot.
	playerList[playerID].lifeCounter.setFillColor(sf::Color(playerList[playerID].lifeCounter.getFillColor().r, playerList[playerID].lifeCounter.getFillColor().g, playerList[playerID].lifeCounter.getFillColor().b, 255));
	playerList[playerID].poisonCounter.setFillColor(sf::Color(playerList[playerID].poisonCounter.getFillColor().r, playerList[playerID].poisonCounter.getFillColor().g, playerList[playerID].poisonCounter.getFillColor().b, 255));
	playerList[playerID].displayName->getRenderer()->setTextColor(playerList[playerID].lifeCounter.getFillColor());
}

//Move and fade in a given Player's Commander Damage Counters, to make them all visible.
void gamePanel::PlayerExpandCoroutine(int playerID)
{
	//Create vectors to hold where the counters should end up, where the avatars should end up, and the distances between where they start and where their targets are.
	std::vector<Vector2f> targetCounterPositions;
	std::vector<Vector2f> targetAvatarPositions;
	std::vector<Vector2f> counterDistances;
	std::vector<Vector2f> avatarDistances;
	//Populate the position and distance vectors.
	for (int i = 0; i < (int)playerList[playerID].cmdrDmgCounters.size(); i++)
	{
		//All the counters should be arranged side-to-side below where they started, directly underneath their "close" button.
		targetCounterPositions.push_back(Vector2f((playerList[playerID].cmdrCloseButton.getPosition().x - (playerList[playerID].cmdrDmgCounters[i].getGlobalBounds().width * ((playerList[playerID].cmdrDmg.size() - 1) / 2.0f)) + (playerList[playerID].cmdrDmgCounters[i].getGlobalBounds().width * i * 1.2f)),
			(playerList[playerID].cmdrCloseButton.getPosition().y + playerList[playerID].cmdrCloseButton.getGlobalBounds().height) - playerList[playerID].cmdrDmgCounters[i].getLocalBounds().top));
		//All the avatars should be positioned the same way as the counters, so they appear behind them.
		targetAvatarPositions.push_back(targetCounterPositions[i] - Vector2f(playerList[playerID].cmdrAvatars[i].getGlobalBounds().width / 2, playerList[playerID].cmdrAvatars[i].getGlobalBounds().height / 2));
		//Get their distances, so the objects can be moved asymptotically.
		counterDistances.push_back(Vector2f(targetCounterPositions[i] - playerList[playerID].cmdrDmgCounters[i].getPosition()));
		avatarDistances.push_back(Vector2f(targetAvatarPositions[i] - playerList[playerID].cmdrAvatars[i].getPosition()));
	}
	//Instead of flooring the distance traveled per frame, just move the objects until they are "close enough".
	while (targetCounterPositions[0].y - playerList[playerID].cmdrDmgCounters[0].getPosition().y > 2)
	{
		for (int i = 0; i < (int)playerList[playerID].cmdrDmgCounters.size(); i++)
		{
			//Move the counters ~8% of their remaining distance.
			playerList[playerID].cmdrDmgCounters[i].move(counterDistances[i] / 12.5f);
			//Also increase the counters' alphas by ~8%.
			playerList[playerID].cmdrDmgCounters[i].setFillColor(playerList[playerID].cmdrDmgCounters[i].getFillColor() + sf::Color(0, 0, 0, 20));
			//Move the avatars ~8% of their remaining distance.
			playerList[playerID].cmdrAvatars[i].move(avatarDistances[i] / 12.5f);
			//Also increase the avatars' alphas by ~8%.
			playerList[playerID].cmdrAvatars[i].setColor(playerList[playerID].cmdrAvatars[i].getColor() + sf::Color(0, 0, 0, 20));
			//Move the buttons ~8% of their remaining distance.
			playerList[playerID].cmdrButtons[i].move(counterDistances[i] / 12.5f);
			//Also increase the buttons' alphas by ~8%.
			playerList[playerID].cmdrButtons[i].setColor(playerList[playerID].cmdrButtons[i].getColor() + sf::Color(0, 0, 0, 20));
			//Get the remaining distances, to decrease the distance traveled next frame.
			counterDistances[i] = Vector2f(targetCounterPositions[i] - playerList[playerID].cmdrDmgCounters[i].getPosition());
			avatarDistances[i] = Vector2f(targetAvatarPositions[i] - playerList[playerID].cmdrAvatars[i].getPosition());
		}
		//Also increase the close button's alpha by ~8%.
		playerList[playerID].cmdrCloseButton.setColor(playerList[playerID].cmdrCloseButton.getColor() + sf::Color(0, 0, 0, 20));
		//Yield the thread for one frame, so the movement is visible.
		std::this_thread::yield();
		std::this_thread::sleep_for(tick);
	}
	//Ensure that all of the positions and alpha values are correct.
	for (int i = 0; i < (int)playerList[playerID].cmdrDmgCounters.size(); i++)
	{
		playerList[playerID].cmdrDmgCounters[i].setPosition(targetCounterPositions[i]);
		playerList[playerID].cmdrAvatars[i].setPosition(targetAvatarPositions[i]);
		playerList[playerID].cmdrDmgCounters[i].setFillColor(sf::Color::Black);
		playerList[playerID].cmdrAvatars[i].setColor(sf::Color::White);
		playerList[playerID].cmdrButtons[i].setColor(sf::Color::White);
	}
	playerList[playerID].cmdrCloseButton.setColor(sf::Color::White);
}

//Move and fade out a given Player's Commander Damage Counters, until only the highest one is visible.
void gamePanel::PlayerCollapseCoroutine(int playerID)
{
	//Create vectors to hold where the counters should end up, where the avatars should end up, and the distances between where they start and where their targets are.
	std::vector<Vector2f> targetCounterPositions;
	std::vector<Vector2f> targetAvatarPositions;
	std::vector<Vector2f> counterDistances;
	std::vector<Vector2f> avatarDistances;
	//Populate the position and distance vectors.
	for (int i = 0; i < (int)playerList[playerID].cmdrDmgCounters.size(); i++)
	{
		//All the counters should be positioned on top of the "close" button.
		targetCounterPositions.push_back(Vector2f(playerList[playerID].cmdrCloseButton.getPosition()));
		//All the avatars should be positioned to the immediate left of the counters.
		targetAvatarPositions.push_back(targetCounterPositions[i] - Vector2f(playerList[playerID].cmdrAvatars[i].getGlobalBounds().width * 1.5f, playerList[playerID].cmdrDmgCounters[i].getLocalBounds().top));
		//Get their distances, so the objects can be moved asymptotically.
		counterDistances.push_back(Vector2f(targetCounterPositions[i] - playerList[playerID].cmdrDmgCounters[i].getPosition()));
		avatarDistances.push_back(Vector2f(targetAvatarPositions[i] - playerList[playerID].cmdrAvatars[i].getPosition()));
	}
	//Determine which counter to keep visible and which ones to fade out.
	int counterToKeep = getHighestCmdrDmg(playerID);
	while (targetCounterPositions[0].y - playerList[playerID].cmdrDmgCounters[0].getPosition().y < -2)
	{
		for (int i = 0; i < (int)playerList[playerID].cmdrDmgCounters.size(); i++)
		{
			//Move the counters ~8% of their remaining distance.
			playerList[playerID].cmdrDmgCounters[i].move(counterDistances[i] / 12.5f);
			//Move the avatars ~8% of their remaining distance.
			playerList[playerID].cmdrAvatars[i].move(avatarDistances[i] / 12.5f);
			//Move the buttons ~8% of their remaining distance.
			playerList[playerID].cmdrButtons[i].move(counterDistances[i] / 12.5f);
			//Also reduce the buttons' alphas by ~8%.
			playerList[playerID].cmdrButtons[i].setColor(playerList[playerID].cmdrButtons[i].getColor() - sf::Color(0, 0, 0, 20));
			//Get the remaining distances, to decrease the distance traveled next frame.
			counterDistances[i] = Vector2f(targetCounterPositions[i] - playerList[playerID].cmdrDmgCounters[i].getPosition());
			avatarDistances[i] = Vector2f(targetAvatarPositions[i] - playerList[playerID].cmdrAvatars[i].getPosition());
			//If this isn't the highest Counter, fade it and the corresponding Avatar out.
			if (i != counterToKeep)
			{
				playerList[playerID].cmdrAvatars[i].setColor(playerList[playerID].cmdrAvatars[i].getColor() - sf::Color(0, 0, 0, 20));
				playerList[playerID].cmdrDmgCounters[i].setFillColor(playerList[playerID].cmdrDmgCounters[i].getFillColor() - sf::Color(0, 0, 0, 20));
			}
		}
		//Also reduce the close button's alpha by ~8%.
		playerList[playerID].cmdrCloseButton.setColor(playerList[playerID].cmdrCloseButton.getColor() - sf::Color(0, 0, 0, 20));
		//Yield the thread for one frame, so the movement is visible.
		std::this_thread::yield();
		std::this_thread::sleep_for(tick);
	}
	//Reset the flag to allow the counters to be expanded again.
	playerList[playerID].cmdrCountersExpanded = false;
}

//Update the Life Counter for one Player.
	//playerID: The array index for the Player whose Life total is being affected.
	//change: The amount to increase the Player's life by (can be negative).
void gamePanel::UpdateLife(int playerID, int change)
{
	playerList[playerID].life += change;
	playerList[playerID].lifeCounter.setString(std::to_string(playerList[playerID].life));
	//Check if the Player's life is 0.
	if (playerList[playerID].life <= 0)
	{
	}
}

//Update the Poison Counter for one Player.
	//playerID: The array index for the Player whose Poison total is being affected.
	//change: The amount to increase the Player's poison by (can be negative).
void gamePanel::UpdatePoison(int playerID, int change)
{
	playerList[playerID].poison += change;
	playerList[playerID].poisonCounter.setString(std::to_string(playerList[playerID].poison));
	//Check if the Player's poison level is 10.
	if (playerList[playerID].poison >= 10)
	{
	}
}

//Update one of the Commander Damage Counters for one Player.
	//playerID: The array index for the Player whose Commander Damage total is being affected.
	//enemyID: The array index for the Damage Counter being affected.
	//change: The amount to increase the Player's commander damage by (can be negative).
void gamePanel::UpdateCommander(int playerID, int enemyID, int change)
{
	playerList[playerID].cmdrDmg[enemyID] += change;
	//If the value on this counter is now less than 0, keep it at 0 and don't affect the Player's Life counter.
	if (playerList[playerID].cmdrDmg[enemyID] < 0)
	{
		playerList[playerID].cmdrDmg[enemyID] = 0;
	}
	else
	{
		//Update the Life of the given Player to reflect the damage dealt.
		UpdateLife(playerID, -change);
	}
	playerList[playerID].cmdrDmgCounters[enemyID].setString(std::to_string(playerList[playerID].cmdrDmg[enemyID]));
	//Check if the Player's commander damage is 21.
	if (playerList[playerID].cmdrDmg[enemyID] >= 21)
	{

	}
}

//Disable the Panel's GUI objects when the Game Setup Panel is open.
void gamePanel::disableGUI()
{
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		playerList[i].displayName->hide();
		if (teamSizeID > 0)
		{
			playerList[i].partnerDisplayNameOne->hide();
		}
		if (teamSizeID == 2)
		{
			playerList[i].partnerDisplayNameTwo->hide();
		}
	}
}

//Re-enable the Panel's GUI objects when the Game Setup Panel is closed.
void gamePanel::enableGUI()
{
	for (int i = 0; i < (int)playerList.size(); i++)
	{
		playerList[i].displayName->show();
		if (teamSizeID > 0)
		{
			playerList[i].partnerDisplayNameOne->show();
		}
		if (teamSizeID == 2)
		{
			playerList[i].partnerDisplayNameTwo->show();
		}
	}
}
