/*
 * gameSetupPanel.cpp
 */

#include "gameSetupPanel.h"

gameSetupPanel::gameSetupPanel()
{
	//Set up the Background rectangle.
	//Make it 1/3rd of the screen.
	background = sf::RectangleShape(sf::Vector2f((float)window->getSize().x / 3, (float)window->getSize().y));
	//Make the background black and ~25% transparent.
	background.setFillColor(sf::Color(0, 0, 0, 204));
	//Set it just off the right edge of the screen, so it can slide into view later.
	background.setPosition((float)window->getSize().x, 0);

	//Set up the "Add Player" button.
	if (!gameSetupButtonAddTexture.loadFromFile(resourcePath + "gameSetupButtonAdd.png"))
	{
		return;
	}
	gameSetupButtonAdd = sf::Sprite(gameSetupButtonAddTexture);

	//Load the "Remove Player" button Texture.
	if (!gameSetupButtonRemoveTexture.loadFromFile(resourcePath + "gameSetupButtonRemove.png"))
	{
		return;
	}

	//Set up the header text for the Panel.
	header.setFont(*font_dejavu);
	header.setStyle(sf::Text::Italic | sf::Text::Underlined);
	header.setFillColor(sf::Color::White);
	header.setCharacterSize(window->getSize().y / 22.5);
	header.setString("CREATE GAME");
	//Center the text on the Setup Panel, and position it near the top of the screen.
	header.setPosition(background.getPosition().x + (background.getGlobalBounds().width / 2 - header.getGlobalBounds().width / 2), background.getPosition().y + 5);

	//Set up the TextBox to add Players to the Database.
	addPlayerTextbox = tgui::TextBox::create();
	gui->add(addPlayerTextbox);
	addPlayerTextbox->getRenderer()->setFont(*font_dejavu);
	addPlayerTextbox->setReadOnly(false);
	addPlayerTextbox->setMaximumCharacters(14);
	addPlayerTextbox->setTextSize(header.getCharacterSize() / 2);
	//Make the TextBox background light grey.
	addPlayerTextbox->getRenderer()->setBackgroundColor(sf::Color(192, 192, 192, 255));
	addPlayerTextbox->getRenderer()->setTextColor(sf::Color::Black);
	addPlayerTextbox->getRenderer()->setBorders(tgui::Borders(3));
	//Make the TextBox borders dark grey.
	addPlayerTextbox->getRenderer()->setBorderColor(sf::Color(64, 64, 64, 255));
	//Position the TextBox just underneath the "header".
	addPlayerTextbox->setPosition(background.getPosition().x + 36, background.getGlobalBounds().height * .09f);
	//Remove the TextBox's drop-down arrow.
	addPlayerTextbox->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);

	//Set up the Button to add Players to the Database.
	addPlayerButton = tgui::Button::create();
	gui->add(addPlayerButton);
	addPlayerButton->getRenderer()->setFont(*font_dejavu);
	addPlayerButton->setTextSize(addPlayerTextbox->getTextSize() * .9);
	addPlayerButton->getRenderer()->setTextColor(sf::Color::Black);
	addPlayerButton->getRenderer()->setBorders(tgui::Borders(3));
	addPlayerButton->setText("ADD PLAYER");
	//Set addPlayerTextbox's size relative to addPlayerButton.
	addPlayerTextbox->setSize(addPlayerButton->getSize().x * 2.4f, addPlayerButton->getSize().y);
	//Position the Button to the immediate right of "addPlayerTextbox".
	addPlayerButton->setPosition(addPlayerTextbox->getPosition().x + addPlayerTextbox->getFullSize().x, addPlayerTextbox->getPosition().y);
	//Bind clicking this Button to the "addPlayerToTable" method.
	addPlayerButton->connect("clicked", &gameSetupPanel::addPlayerToTable, this);

	//Set up the header text for the Player List.
	playerListHeader.setFont(*font_dejavu);
	playerListHeader.setStyle(sf::Text::Underlined);
	playerListHeader.setFillColor(sf::Color::White);
	playerListHeader.setCharacterSize(window->getSize().y / 28.125);
	playerListHeader.setString("PLAYERS:");
	//Position the Text along the left edge of the Panel, underneath the "Add Player To Table" objects.
	playerListHeader.setPosition(background.getPosition().x + 5, background.getPosition().y + (background.getGlobalBounds().height * .15f));

	//Set the "Add Player" button's size.
	gameSetupButtonAdd.setScale(.4f, .4f);

	//Set up the header text for the Team Size Drop-down.
	teamSizeHeader.setFont(*font_dejavu);
	teamSizeHeader.setStyle(sf::Text::Underlined);
	teamSizeHeader.setFillColor(sf::Color::White);
	teamSizeHeader.setCharacterSize(playerListHeader.getCharacterSize());
	teamSizeHeader.setString("TEAM SIZE:");
	//Position it near the bottom of the screen, after the "Game Type" dropdown.
	teamSizeHeader.setPosition(background.getPosition().x + 5, background.getPosition().y + (background.getGlobalBounds().height * .8f));

	//Set up the Team Size Drop-down.
	teamSizeList = tgui::ComboBox::create();
	gui->add(teamSizeList);
	teamSizeList->getRenderer()->setFont(*font_dejavu);
	teamSizeList->setTextSize(20);
	//Make the TextBox background light grey.
	teamSizeList->getRenderer()->setBackgroundColor(tgui::Color(192, 192, 192, 255));
	teamSizeList->getRenderer()->setTextColor(sf::Color::Black);
	teamSizeList->getRenderer()->setBorders(tgui::Borders(3));
	//Make the TextBox borders dark grey.
	teamSizeList->getRenderer()->setBorderColor(tgui::Color(64, 64, 64, 255));
	//Position the TextBox to the immediate right of "teamSizeHeader".
	teamSizeList->setPosition(teamSizeHeader.getPosition().x + teamSizeHeader.getGlobalBounds().width + 16, teamSizeHeader.getPosition().y + 3);
	//Add each of the team size options to the list, and set "Solo" as the default option.
	teamSizeList->addItem("Solo", "0");
	teamSizeList->addItem("Two-Headed", "1");
	teamSizeList->addItem("Three-Headed", "2");
	teamSizeList->setSelectedItemById("0");

	//Add the first two Player Entry Fields.
	addPlayerEntryField(0);
	addPlayerEntryField(1);

	//Set the Team Size Drop-down's size relative to the 1st Player Entry Field.
	teamSizeList->setSize(playerEntryFields[0]->getSize().x * .85f, playerEntryFields[0]->getFullSize().y);

	//Move the "Add Player" Button to align with the right edge of the Player Entry Fields, just beneath the last one of them.
	gameSetupButtonAdd.setPosition((playerEntryFields[1]->getPosition().x + playerEntryFields[1]->getFullSize().x) - (gameSetupButtonAdd.getGlobalBounds().width + 3),
		playerEntryFields[1]->getPosition().y + playerEntryFields[1]->getFullSize().y + 3);

	//Set up the header text for the Game Type Drop-down.
	gameTypeHeader.setFont(*font_dejavu);
	gameTypeHeader.setStyle(sf::Text::Underlined);
	gameTypeHeader.setFillColor(sf::Color::White);
	gameTypeHeader.setCharacterSize(playerListHeader.getCharacterSize());
	gameTypeHeader.setString("GAME TYPE:");
	//Position the Text along the left edge of the Panel, leaving enough room for the maximum number of Player Entry Fields.
	gameTypeHeader.setPosition(background.getPosition().x + 5, background.getPosition().y + (background.getGlobalBounds().height * .67f));

	//Set up the Game Type Drop-down.
	gameTypeList = tgui::ComboBox::create();
	gui->add(gameTypeList);
	gameTypeList->getRenderer()->setFont(*font_dejavu);
	gameTypeList->setTextSize(20);
	//Make the TextBox background light grey.
	gameTypeList->getRenderer()->setBackgroundColor(tgui::Color(192, 192, 192, 255));
	gameTypeList->getRenderer()->setTextColor(sf::Color::Black);
	gameTypeList->getRenderer()->setBorders(tgui::Borders(3));
	//Make the TextBox borders dark grey.
	gameTypeList->getRenderer()->setBorderColor(tgui::Color(64, 64, 64, 255));
	//Position the TextBox to the immediate right of "gameTypeHeader".
	gameTypeList->setPosition(gameTypeHeader.getPosition().x + gameTypeHeader.getGlobalBounds().width + 16, gameTypeHeader.getPosition().y + 3);
	gameTypeList->setSize(playerEntryFields[0]->getSize().x * .85f, playerEntryFields[0]->getFullSize().y);
	//Add each of the game type options to the list, and set "Standard" as the default option.
	gameTypeList->addItem("Standard", "0");
	gameTypeList->addItem("Commander", "1");
	gameTypeList->addItem("Emperor", "2");
	gameTypeList->addItem("Brawl", "3");
	gameTypeList->setSelectedItemById("0");

	//Set up the "Create Game" Button.
	createGameButton = tgui::Button::create();
	gui->add(createGameButton);
	createGameButton->getRenderer()->setFont(*font_dejavu);
	createGameButton->setTextSize(18);
	createGameButton->getRenderer()->setTextColor(sf::Color::Black);
	createGameButton->setText("CREATE");
	//Position the Button in the lower-right corner of the Panel.
	createGameButton->setPosition((background.getPosition().x + background.getGlobalBounds().width) - createGameButton->getFullSize().x - 30, (background.getPosition().y + background.getGlobalBounds().height) - createGameButton->getFullSize().y - 30);
}

//Start the thread to slide out the Setup Panel, and move the "Create Game" tab with it.
void gameSetupPanel::slideOut(sf::Sprite& tab)
{
	slideOutThread = std::thread(std::bind(&gameSetupPanel::slideOutCoroutine, this, std::ref(tab)));
}

//Slide the Setup Panel into view, moving all of its visual objects with it.
void gameSetupPanel::slideOutCoroutine(sf::Sprite& tab)
{
	//Create a float to hold the distance the Panel will have to move per frame.
	float distance;
	//Move the Panel until it is fully visible.
	while (background.getPosition().x > window->getSize().x - background.getGlobalBounds().width)
	{
		//Move the Panel asymptotically (quickly to start, slower as it gets closer to its target).
		distance = (background.getPosition().x - (window->getSize().x - background.getGlobalBounds().width)) / (background.getGlobalBounds().width / 50);
		//Floor the distance at 2 pixels, so it does eventually reach its target.
		if (distance < 2) distance = 2;
		//Move all of the Panel's objects.
		tab.move(-distance, 0);
		background.move(-distance, 0);
		header.move(-distance, 0);
		addPlayerTextbox->setPosition(addPlayerTextbox->getPosition() + Vector2f(-distance, 0));
		addPlayerButton->setPosition(addPlayerButton->getPosition() + Vector2f(-distance, 0));
		playerListHeader.move(-distance, 0);
		gameTypeHeader.move(-distance, 0);
		gameSetupButtonAdd.move(-distance, 0);
		gameTypeList->setPosition(gameTypeList->getPosition() + Vector2f(-distance, 0));
		teamSizeHeader.move(-distance, 0);
		teamSizeList->setPosition(teamSizeList->getPosition() + Vector2f(-distance, 0));
		createGameButton->setPosition(createGameButton->getPosition() + Vector2f(-distance, 0));
		for (int i = 0; i < (int)playerNumbers.size(); i++)
		{
			playerNumbers[i].move(-distance, 0);
			gameSetupButtonsRemove[i].move(-distance, 0);
			playerEntryFields[i]->setPosition(playerEntryFields[i]->getPosition() + Vector2f(-distance, 0));
			if (teamSizeList->getSelectedItemIndex() > 0)
			{
				playerEntryFields2[i]->setPosition(playerEntryFields2[i]->getPosition() + Vector2f(-distance, 0));
			}
			if (teamSizeList->getSelectedItemIndex() == 2)
			{
				playerEntryFields3[i]->setPosition(playerEntryFields3[i]->getPosition() + Vector2f(-distance, 0));
			}
		}
		//Yield the thread for one frame, so the movement is actually visible.
		std::this_thread::yield();
		std::this_thread::sleep_for(tick);
	}
	//If the Panel overshoots its mark, move it back.
	if (background.getPosition().x <= window->getSize().x - background.getGlobalBounds().width)
	{
		//Get the distance between where the Panel ended up and where it should have ended up.
		float difference = (window->getSize().x - background.getGlobalBounds().width) - background.getPosition().x;
		//Move all of the Panel objects back into place.
		background.move(difference, 0);
		tab.move(difference, 0);
		header.move(difference, 0);
		addPlayerTextbox->setPosition(addPlayerTextbox->getPosition() + Vector2f(difference, 0));
		addPlayerButton->setPosition(addPlayerButton->getPosition() + Vector2f(difference, 0));
		playerListHeader.move(difference, 0);
		gameTypeHeader.move(difference, 0);
		gameSetupButtonAdd.move(difference, 0);
		gameTypeList->setPosition(gameTypeList->getPosition() + Vector2f(difference, 0));
		teamSizeHeader.move(difference, 0);
		teamSizeList->setPosition(teamSizeList->getPosition() + Vector2f(difference, 0));
		createGameButton->setPosition(createGameButton->getPosition() + Vector2f(difference, 0));
		for (int i = 0; i < (int)playerNumbers.size(); i++)
		{
			playerNumbers[i].move(difference, 0);
			gameSetupButtonsRemove[i].move(difference, 0);
			playerEntryFields[i]->setPosition(playerEntryFields[i]->getPosition() + Vector2f(difference, 0));
			if (teamSizeList->getSelectedItemIndex() > 0)
			{
				playerEntryFields2[i]->setPosition(playerEntryFields2[i]->getPosition() + Vector2f(difference, 0));
			}
			if (teamSizeList->getSelectedItemIndex() == 2)
			{
				playerEntryFields3[i]->setPosition(playerEntryFields3[i]->getPosition() + Vector2f(difference, 0));
			}
		}
	}
	return;
}

//Add a new Player name to the "players" SQL table.
void gameSetupPanel::addPlayerToTable()
{
	//Save "addPlayerTextbox"'s Text as an SQL object.
	sql::SQLString newPlayerName = sql::SQLString(addPlayerTextbox->getText());
	//Create an object to store the new Player's ID number.
	int newPlayerID;
	//Get the number of Players already in the table, to determine this new Player's ID.
	res = stmt->executeQuery("SELECT COUNT(id) FROM players");
	while (res->next())
	{
		newPlayerID = res->getInt(1);
	}
	//Start preparing the "Insert" statement to add the new Player.
	prep_stmt = con->prepareStatement("INSERT INTO players(id, name) VALUES (?,?)");
	//Set the new Player's ID number.
	prep_stmt->setInt(1, newPlayerID);
	//Set the new Player's name.
	prep_stmt->setString(2, newPlayerName);
	//Add this Player to the table.
	prep_stmt->execute();
	//Clear "addPlayerTextBox"'s Text.
	addPlayerTextbox->setText("");
	//Add this Player's name to the end of all active Player Entry Fields' lists, to make it selectable via each of their dropdowns.
	for (int i = 0; i < (int)playerEntryFields.size(); i++)
	{
		playerEntryFields[i]->addItem(sf::String(newPlayerName), std::to_string(newPlayerID));
		if (teamSizeList->getSelectedItemIndex() > 0)
		{
			playerEntryFields2[i]->addItem(sf::String(newPlayerName), std::to_string(newPlayerID));
		}
		if (teamSizeList->getSelectedItemIndex() == 2)
		{
			playerEntryFields3[i]->addItem(sf::String(newPlayerName), std::to_string(newPlayerID));
		}
	}
}

//Add another Player/Team Entry row to the Game Setup Panel.
void gameSetupPanel::addPlayerEntryField(int index)
{
	//Set up the new Player Number object.
	playerNumbers.push_back(sf::Text(std::to_string(index + 1) + ".", *font_dejavu, playerListHeader.getCharacterSize()));
	playerNumbers[index].setFillColor(sf::Color::White);
	//Position the new number along the left edge of the Panel, under the left edge of "playerListHeader".
	playerNumbers[index].setPosition(playerListHeader.getPosition().x + 8, playerListHeader.getPosition().y + playerListHeader.getGlobalBounds().height + 20 +
		((playerNumbers[index].getGlobalBounds().height + 12) * index));

	//Set up the new Player Entry Field object.
	playerEntryFields.push_back(tgui::ComboBox::create());
	gui->add(playerEntryFields[index]);
	playerEntryFields[index]->getRenderer()->setFont(*font_dejavu);
	//playerEntryFields[index]->setReadOnly(false);
	//playerEntryFields[index]->setMaximumCharacters(14);
	playerEntryFields[index]->setTextSize(playerNumbers[index].getCharacterSize() * .9);
	//Make the TextBox background light grey.
	playerEntryFields[index]->getRenderer()->setBackgroundColor(tgui::Color(192, 192, 192, 255));
	playerEntryFields[index]->getRenderer()->setTextColor(sf::Color::Black);
	playerEntryFields[index]->getRenderer()->setBorders(tgui::Borders(3));
	//Make the TextBox borders dark grey.
	playerEntryFields[index]->getRenderer()->setBorderColor(tgui::Color(64, 64, 64, 255));
	playerEntryFields[index]->setSize(playerNumbers[0].getGlobalBounds().width * 12, playerNumbers[index].getGlobalBounds().height + 6);
	//Position the TextBox to the immediate right of its corresponding number.
	playerEntryFields[index]->setPosition(playerNumbers[0].getPosition().x + playerNumbers[0].getGlobalBounds().width + 30,
		playerNumbers[index].getPosition().y + playerEntryFields[index]->getRenderer()->getBorders().getTop());

	//Add all of the Player names to the Player Entry Field.
	res = stmt->executeQuery("SELECT name FROM players ORDER BY id ASC");
	for (int i = 0; i < (int)res->rowsCount(); i++)
	{
		res->next();
		playerEntryFields[index]->addItem(sf::String(res->getString("name")), std::to_string(i));
	}

	//Set up the new "Remove Player" Button.
	gameSetupButtonsRemove.push_back(sf::Sprite(gameSetupButtonRemoveTexture));
	gameSetupButtonsRemove[index].setScale(.4f, .4f);
	//Position the Button to the immediate right of the Player Entry Field.
	gameSetupButtonsRemove[index].setPosition(playerEntryFields[index]->getPosition().x + playerEntryFields[index]->getFullSize().x + 12,
		(playerEntryFields[index]->getPosition().y + (playerEntryFields[index]->getSize().y / 2)) - (gameSetupButtonsRemove[index].getGlobalBounds().height / 2));

	//If necessary, make a Player Entry Field for the 2nd column.
	if (teamSizeList->getSelectedItemIndex() > 0)
	{
		//Copy the 1st Player Entry Field that was created, to get all of its style parameters.
		playerEntryFields2.push_back(playerEntryFields2[index]->copy(playerEntryFields[index]));
		gui->add(playerEntryFields2[index]);
		//Reduce the width of both Player Entry Fields to make them fit.
		playerEntryFields2[index]->setSize(playerNumbers[0].getGlobalBounds().width * 7, playerEntryFields[index]->getSize().y);
		playerEntryFields[index]->setSize(playerNumbers[0].getGlobalBounds().width * 7, playerEntryFields[index]->getSize().y);
		//The Selected Item will also be copied from the 1st Player Entry Field, so clear it.
		playerEntryFields2[index]->deselectItem();
		//Position this TextBox to the immediate right of the 1st Player Entry Field that was created here.
		playerEntryFields2[index]->setPosition(playerEntryFields[index]->getPosition().x + playerEntryFields[index]->getFullSize().x + 8, playerEntryFields2[index]->getPosition().y);
		//Move the Button to the immediate right of this TextBox.
		gameSetupButtonsRemove[index].setPosition(playerEntryFields2[index]->getPosition().x + playerEntryFields2[index]->getFullSize().x + 12, gameSetupButtonsRemove[index].getPosition().y);
	}

	//If necessary, make a Player Entry Field for the 3rd column.
	if (teamSizeList->getSelectedItemIndex() == 2)
	{
		//Copy the 1st Player Entry Field that was created, to get all of its style parameters.
		playerEntryFields3.push_back(playerEntryFields3[index]->copy(playerEntryFields[index]));
		gui->add(playerEntryFields3[index]);
		//Reduce the width of all 3 Player Entry Fields to make them fit.
		playerEntryFields3[index]->setSize(playerNumbers[0].getGlobalBounds().width * 4.6f, playerEntryFields[index]->getSize().y);
		playerEntryFields2[index]->setSize(playerNumbers[0].getGlobalBounds().width * 4.6f, playerEntryFields[index]->getSize().y);
		playerEntryFields[index]->setSize(playerNumbers[0].getGlobalBounds().width * 4.6f, playerEntryFields[index]->getSize().y);
		//The Selected Item will also be copied from the 1st Player Entry Field, so clear it.
		playerEntryFields3[index]->deselectItem();
		//Position this TextBox to the immediate right of the 1st Player Entry Field that was created here.
		playerEntryFields2[index]->setPosition(playerEntryFields[index]->getPosition().x + playerEntryFields[index]->getFullSize().x + 8, playerEntryFields2[index]->getPosition().y);
		//Position this TextBox to the immediate right of the 2nd Player Entry Field that was created here.
		playerEntryFields3[index]->setPosition(playerEntryFields2[index]->getPosition().x + playerEntryFields2[index]->getFullSize().x + 8, playerEntryFields2[index]->getPosition().y);
		//Move the Button to the immediate right of this TextBox.
		gameSetupButtonsRemove[index].setPosition(playerEntryFields3[index]->getPosition().x + playerEntryFields3[index]->getFullSize().x + 12, gameSetupButtonsRemove[index].getPosition().y);
	}

	//Move the "Add Player" Button down one row.
	gameSetupButtonAdd.setPosition(gameSetupButtonAdd.getPosition().x, playerEntryFields[index]->getPosition().y + playerEntryFields[index]->getFullSize().y + 3);
}

//Remove a Player/Team Entry row from the Game Setup Panel.
//(This method always removes the last row present on the Panel, but does so in a way
// to make it look like the row for the "Remove" Button that was clicked was removed).
void gameSetupPanel::removePlayerEntryField(int index)
{
	//Remove the last Player number.
	playerNumbers.pop_back();
	//Remove the last "Remove Player" button.
	gameSetupButtonsRemove.pop_back();
	//Move all of the Entry Fields' Text values down one.
	for (int i = index; i < (int)playerEntryFields.size() - 1; i++)
	{
		playerEntryFields[i]->setSelectedItemByIndex(playerEntryFields[i + 1]->getSelectedItemIndex());
		if (teamSizeList->getSelectedItemIndex() > 0)
		{
			playerEntryFields2[i]->setSelectedItemByIndex(playerEntryFields2[i + 1]->getSelectedItemIndex());
		}
		if (teamSizeList->getSelectedItemIndex() == 2)
		{
			playerEntryFields3[i]->setSelectedItemByIndex(playerEntryFields3[i + 1]->getSelectedItemIndex());
		}
	}
	//Remove the last row of Player Entry Fields.
	gui->remove(playerEntryFields[playerEntryFields.size() - 1]);
	playerEntryFields.pop_back();
	if (teamSizeList->getSelectedItemIndex() > 0)
	{
		gui->remove(playerEntryFields2[playerEntryFields2.size() - 1]);
		playerEntryFields2.pop_back();
	}
	if (teamSizeList->getSelectedItemIndex() == 2)
	{
		gui->remove(playerEntryFields3[playerEntryFields3.size() - 1]);
		playerEntryFields3.pop_back();
	}
	//Move the "Add Player" Button up one row.
	gameSetupButtonAdd.setPosition(gameSetupButtonAdd.getPosition().x, playerEntryFields[playerEntryFields.size() - 1]->getPosition().y + playerEntryFields[playerEntryFields.size() - 1]->getFullSize().y + 3);
}

//Add another column of Player Entry Fields to the Panel, to hold partner names for Two- or Three-Headed Giant games.
void gameSetupPanel::addPlayerEntryColumn()
{
	//If the selected team size is "Two-Headed" or "Three-Headed" and the 2nd Entry Column is not visible, add it.
	if (teamSizeList->getSelectedItemIndex() > 0 && playerEntryFields2.size() == 0)
	{
		for (int i = 0; i < (int)playerEntryFields.size(); i++)
		{
			//Copy the 1st Player Entry Field, to get all of its style parameters.
			playerEntryFields2.push_back(playerEntryFields2[i]->copy(playerEntryFields[i]));
			gui->add(playerEntryFields2[i]);
			//Reduce the width of both Player Entry Fields to make them fit.
			playerEntryFields2[i]->setSize(playerNumbers[0].getGlobalBounds().width * 7, playerEntryFields[i]->getSize().y);
			playerEntryFields[i]->setSize(playerNumbers[0].getGlobalBounds().width * 7, playerEntryFields[i]->getSize().y);
			//The Selected Item will also be copied from the 1st Player Entry Field, so clear it.
			playerEntryFields2[i]->deselectItem();
			//Position this TextBox to the immediate right of the 1st Player Entry Field that was created here.
			playerEntryFields2[i]->setPosition(playerEntryFields[i]->getPosition().x + playerEntryFields[i]->getFullSize().x + 8, playerEntryFields2[i]->getPosition().y);
			//Move the Button to the immediate right of this TextBox.
			gameSetupButtonsRemove[i].setPosition(playerEntryFields2[i]->getPosition().x + playerEntryFields2[i]->getFullSize().x + 12, gameSetupButtonsRemove[i].getPosition().y);
		}
		//Move the "Add Player" Button over to the right edge of the new TextBoxes.
		gameSetupButtonAdd.setPosition((playerEntryFields2[0]->getPosition().x + playerEntryFields2[0]->getFullSize().x) - (gameSetupButtonAdd.getGlobalBounds().width + 3), gameSetupButtonAdd.getPosition().y);
		//Change the "Player List" Header Text to say "Teams" instead of "Players".
		playerListHeader.setString("TEAMS:");
	}
	//If the selected team size is "Three-Headed" and the 3rd Entry Column is not visible, add it.
	if (teamSizeList->getSelectedItemIndex() == 2 && playerEntryFields3.size() == 0)
	{
		for (int i = 0; i < (int)playerEntryFields.size(); i++)
		{
			//Copy the 1st Player Entry Field, to get all of its style parameters.
			playerEntryFields3.push_back(playerEntryFields3[i]->copy(playerEntryFields[i]));
			gui->add(playerEntryFields3[i]);
			//Reduce the width of all 3 Player Entry Fields to make them fit.
			playerEntryFields3[i]->setSize(playerNumbers[0].getGlobalBounds().width * 4.6f, playerEntryFields[i]->getSize().y);
			playerEntryFields2[i]->setSize(playerNumbers[0].getGlobalBounds().width * 4.6f, playerEntryFields[i]->getSize().y);
			playerEntryFields[i]->setSize(playerNumbers[0].getGlobalBounds().width * 4.6f, playerEntryFields[i]->getSize().y);
			//The Selected Item will also be copied from the 1st Player Entry Field, so clear it.
			playerEntryFields3[i]->deselectItem();
			//Position this TextBox to the immediate right of the 1st Player Entry Field that was created here.
			playerEntryFields2[i]->setPosition(playerEntryFields[i]->getPosition().x + playerEntryFields[i]->getFullSize().x + 8, playerEntryFields2[i]->getPosition().y);
			//Position this TextBox to the immediate right of the 2nd Player Entry Field that was created here.
			playerEntryFields3[i]->setPosition(playerEntryFields2[i]->getPosition().x + playerEntryFields2[i]->getFullSize().x + 8, playerEntryFields2[i]->getPosition().y);
			//Move the Button to the immediate right of this TextBox.
			gameSetupButtonsRemove[i].setPosition(playerEntryFields3[i]->getPosition().x + playerEntryFields3[i]->getFullSize().x + 12, gameSetupButtonsRemove[i].getPosition().y);
		}
		//Move the "Add Player" Button over to the right edge of the new TextBoxes.
		gameSetupButtonAdd.setPosition((playerEntryFields3[0]->getPosition().x + playerEntryFields3[0]->getFullSize().x) - (gameSetupButtonAdd.getGlobalBounds().width + 3), gameSetupButtonAdd.getPosition().y);
	}
}

//Remove a column of Player Entry Fields from the Panel, if "Solo" or "Two-Headed" has been selected as a team size.
void gameSetupPanel::removePlayerEntryColumn()
{
	//If the selected team size is "Solo" or "Two-Headed" and the 3rd Entry Column is visible, remove it.
	if (teamSizeList->getSelectedItemIndex() < 2 && playerEntryFields3.size() > 0)
	{
		for (int i = 0; i < (int)playerEntryFields3.size(); i++)
		{
			//Remove these TextBoxes from the GUI, so they won't be drawn anymore.
			gui->remove(playerEntryFields3[i]);
			//Increase the width of the two remaining Player Entry Fields, to fill the extra space.
			playerEntryFields[i]->setSize(playerNumbers[0].getGlobalBounds().width * 7, playerEntryFields[i]->getSize().y);
			playerEntryFields2[i]->setSize(playerNumbers[0].getGlobalBounds().width * 7, playerEntryFields2[i]->getSize().y);
			//Position this TextBox to the immediate right of the 1st Player Entry Field.
			playerEntryFields2[i]->setPosition(playerEntryFields[i]->getPosition().x + playerEntryFields[i]->getFullSize().x + 8, playerEntryFields2[i]->getPosition().y);
			//Move the Button to the immediate right of the 2nd Player Entry Field.
			gameSetupButtonsRemove[i].setPosition(playerEntryFields2[i]->getPosition().x + playerEntryFields2[i]->getFullSize().x + 12, gameSetupButtonsRemove[i].getPosition().y);
		}
		//Empty the vector that held the objects in this column.
		playerEntryFields3.clear();
		//Move the "Add Player" Button over to the right edge of the 2nd Player Entry Fields.
		gameSetupButtonAdd.setPosition((playerEntryFields2[0]->getPosition().x + playerEntryFields2[0]->getFullSize().x) - (gameSetupButtonAdd.getGlobalBounds().width + 3), gameSetupButtonAdd.getPosition().y);
	}
	//If the selected team size is "Solo" and the 2nd Entry Column is visible, remove it.
	if (teamSizeList->getSelectedItemIndex() == 0 && (int)playerEntryFields2.size() > 0)
	{
		for (int i = 0; i < (int)playerEntryFields2.size(); i++)
		{
			//Remove these TextBoxes from the GUI, so they won't be drawn anymore.
			gui->remove(playerEntryFields2[i]);
			//Increase the width of the remaining Player Entry Field, to fill the extra space.
			playerEntryFields[i]->setSize(playerNumbers[0].getGlobalBounds().width * 12, playerEntryFields[i]->getSize().y);
			//Move the Button to the immediate right of the remaining Player Entry Field.
			gameSetupButtonsRemove[i].setPosition(playerEntryFields[i]->getPosition().x + playerEntryFields[i]->getFullSize().x + 12, gameSetupButtonsRemove[i].getPosition().y);
		}
		//Empty the vector that held the objects in this column.
		playerEntryFields2.clear();
		//Move the "Add Player" Button over to the right edge of the 2nd Player Entry Fields.
		gameSetupButtonAdd.setPosition((playerEntryFields[0]->getPosition().x + playerEntryFields[0]->getFullSize().x) - (gameSetupButtonAdd.getGlobalBounds().width + 3), gameSetupButtonAdd.getPosition().y);
		//Change the "Player List" Header Text to say "Players" again instead of "Teams".
		playerListHeader.setString("PLAYERS:");
	}
}

//Get any user input being done to the Setup Panel.
void gameSetupPanel::getUserInput(sf::Event event)
{
	//Handle the events for the GUI objects.
	//(This is what enables text entry for the "Add Player" TextBox, as well as being able to expand and
	// select from all of the dropdown boxes, and click both the "Add Player" and "Create Game" Buttons.)
	gui->handleEvent(event);
	//If "Two-Headed" or "Three-Headed" was selected as a team size, and 3 Player Entry columns aren't visible, add as many as needed.
	if (teamSizeList->getSelectedItemIndex() > 0 && playerEntryFields3.size() == 0)
	{
		addPlayerEntryColumn();
	}
	//If "Solo" or "Two-Headed" was selected as a team size, and 2 or more Player Entry columns are visible, remove as many as needed.
	if (teamSizeList->getSelectedItemIndex() < 2 && playerEntryFields2.size() > 0)
	{
		removePlayerEntryColumn();
	}

	//Left Mouse Clicks
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		mousePos = sf::Vector2f(sf::Mouse::getPosition());

		//Check the Player Entry Fields.
		for (int i = 0; i < (int)playerEntryFields.size(); i++)
		{
			//If a Field was clicked on, make its Border yellow.
			if (playerEntryFields[i]->mouseOnWidget(mousePos))
			{
				playerEntryFields[i]->getRenderer()->setBorderColor(sf::Color::Yellow);
			}
			//Otherwise, reset it back to grey.
			else
			{
				playerEntryFields[i]->getRenderer()->setBorderColor(tgui::Color(64, 64, 64, 255));
			}
			//Check the 2nd Player Entry Fields.
			if (teamSizeList->getSelectedItemIndex() > 0)
			{
				//If a Field was clicked on, make its Border yellow.
				if (playerEntryFields2[i]->mouseOnWidget(mousePos))
				{
					playerEntryFields2[i]->getRenderer()->setBorderColor(sf::Color::Yellow);
				}
				//Otherwise, reset it back to grey.
				else
				{
					playerEntryFields2[i]->getRenderer()->setBorderColor(tgui::Color(64, 64, 64, 255));
				}
				//Check the 3rd Player Entry Fields.
				if (teamSizeList->getSelectedItemIndex() == 2)
				{
					//If a Field was clicked on, make its Border yellow.
					if (playerEntryFields3[i]->mouseOnWidget(mousePos))
					{
						playerEntryFields3[i]->getRenderer()->setBorderColor(sf::Color::Yellow);
					}
					//Otherwise, reset it back to grey.
					else
					{
						playerEntryFields3[i]->getRenderer()->setBorderColor(tgui::Color(64, 64, 64, 255));
					}
				}
			}
		}

		//Check the "Add Player" Button.
		if (gameSetupButtonAdd.getGlobalBounds().contains(mousePos) && playerNumbers.size() < MAX_PLAYERS)
		{
			addPlayerEntryField(playerNumbers.size());
		}

		//Check each of the "Remove Player" Buttons.
		for (int i = 0; i < (int)gameSetupButtonsRemove.size(); i++)
		{
			//If there are only 2 Player/Team Entry rows, don't remove either.
			if (gameSetupButtonsRemove[i].getGlobalBounds().contains(mousePos) && (int)playerNumbers.size() > 2)
			{
				removePlayerEntryField(i);
			}
		}
	}
	//Tab Key Check
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Tab)
		{
			//Create a check as to whether focus has changed once already.
			bool focusChanged = false;
			//Check each of the Player Entry Fields.
			for (int i = 0; i < (int)playerEntryFields.size(); i++)
			{
				//If a field has focus, move that focus to the next field.
				if (playerEntryFields[i]->isFocused() && !focusChanged)
				{
					playerEntryFields[i]->getRenderer()->setBorderColor(sf::Color(64, 64, 64, 255));
					//If there is only one Player Entry Column, jump to the Field below this one.
					if (teamSizeList->getSelectedItemIndex() == 0)
					{
						playerEntryFields[(i + 1) % playerEntryFields.size()]->setFocused(true);
						playerEntryFields[(i + 1) % playerEntryFields.size()]->getRenderer()->setBorderColor(sf::Color::Yellow);
					}
					//Otherwise, jump one to the right.
					else if (teamSizeList->getSelectedItemIndex() > 0)
					{
						playerEntryFields2[i]->setFocused(true);
						playerEntryFields2[i]->getRenderer()->setBorderColor(sf::Color::Yellow);
					}
					focusChanged = true;
				}
				else if (teamSizeList->getSelectedItemIndex() > 0)
				{
					if (playerEntryFields2[i]->isFocused() && !focusChanged)
					{
						playerEntryFields2[i]->getRenderer()->setBorderColor(sf::Color(64, 64, 64, 255));
						//If there are only two Player Entry Columns, jump to the 1st Column and one Row down.
						if (teamSizeList->getSelectedItemIndex() == 1)
						{
							playerEntryFields[(i + 1) % playerEntryFields.size()]->setFocused(true);
							playerEntryFields[(i + 1) % playerEntryFields.size()]->getRenderer()->setBorderColor(sf::Color::Yellow);
						}
						//Otherwise, jump one to the right.
						else if (teamSizeList->getSelectedItemIndex() == 2)
						{
							playerEntryFields3[i]->setFocused(true);
							playerEntryFields3[i]->getRenderer()->setBorderColor(sf::Color::Yellow);
						}
						focusChanged = true;
					}
				}
				if (teamSizeList->getSelectedItemIndex() == 2)
				{
					if (playerEntryFields3[i]->isFocused() && !focusChanged)
					{
						//Jump to the next Player Entry Field in the 1st Column.
						playerEntryFields3[i]->getRenderer()->setBorderColor(sf::Color(64, 64, 64, 255));
						playerEntryFields[(i + 1) % playerEntryFields.size()]->setFocused(true);
						playerEntryFields[(i + 1) % playerEntryFields.size()]->getRenderer()->setBorderColor(sf::Color::Yellow);
						focusChanged = true;
					}
				}
			}
		}
	}
}

//Collect the data to send to the Panel Manager to create the Game Panel.
std::vector<std::string> gameSetupPanel::getNames()
{
	std::vector<std::string> names;
	for (int i = 0; i < (int)playerEntryFields.size(); i++)
	{
		names.push_back(playerEntryFields[i]->getSelectedItem());
		//If the team size is 2 or more, push the 2nd player on the team.
		if (teamSizeList->getSelectedItemIndex() > 0)
		{
			names.push_back(playerEntryFields2[i]->getSelectedItem());
			//If the team size is 3, push the 3rd player on the team.
			if (teamSizeList->getSelectedItemIndex() == 2)
			{
				names.push_back(playerEntryFields3[i]->getSelectedItem());
			}
		}
	}
	return names;
}

//Collect the data to send to the Panel Manager to create the Game Panel.
std::vector<int> gameSetupPanel::getIDs()
{
	std::vector<int> ids;
	for (int i = 0; i < (int)playerEntryFields.size(); i++)
	{
		ids.push_back(playerEntryFields[i]->getSelectedItemIndex());
		//If the team size is 2 or more, push the 2nd player on the team.
		if (teamSizeList->getSelectedItemIndex() > 0)
		{
			ids.push_back(playerEntryFields2[i]->getSelectedItemIndex());
			//If the team size is 3, push the 3rd player on the team.
			if (teamSizeList->getSelectedItemIndex() == 2)
			{
				ids.push_back(playerEntryFields3[i]->getSelectedItemIndex());
			}
		}
	}
	return ids;
}

//Return the Game Type Drop-Down's Selected Item Index.
int gameSetupPanel::getGameType()
{
	return gameTypeList->getSelectedItemIndex();
}

//Return the Team Size Drop-Down's Selected Item Index.
int gameSetupPanel::getTeamSize()
{
	return teamSizeList->getSelectedItemIndex();
}

//Start the thread to slide the Setup Panel offscreen, and move the "Create Game" tab with it.
void gameSetupPanel::slideIn(sf::Sprite& tab)
{
	slideInThread = std::thread(std::bind(&gameSetupPanel::slideInCoroutine, this, std::ref(tab)));
}

//Slide the Setup Panel out of view, moving all of its visual objects with it.
void gameSetupPanel::slideInCoroutine(sf::Sprite& tab)
{
	//Create a float to hold the distance the Panel will have to move per frame.
	float distance;
	//Move the Panel to the right by 5 pixels every frame, until it is offscreen.
	while (background.getPosition().x < window->getSize().x)
	{
		//Move the Panel asymptotically (quickly to start, slower as it gets closer to its target).
		distance = (window->getSize().x - background.getPosition().x) / (background.getGlobalBounds().width / 50);
		//Floor the distance at 2 pixels, so it does eventually reach its target.
		if (distance < 2) distance = 2;
		//Move all of the Panel's objects.
		tab.move(distance, 0);
		background.move(distance, 0);
		header.move(distance, 0);
		addPlayerTextbox->setPosition(addPlayerTextbox->getPosition() + Vector2f(distance, 0));
		addPlayerButton->setPosition(addPlayerButton->getPosition() + Vector2f(distance, 0));
		playerListHeader.move(distance, 0);
		gameTypeHeader.move(distance, 0);
		gameSetupButtonAdd.move(distance, 0);
		gameTypeList->setPosition(gameTypeList->getPosition() + Vector2f(distance, 0));
		teamSizeHeader.move(distance, 0);
		teamSizeList->setPosition(teamSizeList->getPosition() + Vector2f(distance, 0));
		createGameButton->setPosition(createGameButton->getPosition() + Vector2f(distance, 0));
		for (int i = 0; i < (int)playerNumbers.size(); i++)
		{
			playerNumbers[i].move(distance, 0);
			gameSetupButtonsRemove[i].move(distance, 0);
			playerEntryFields[i]->setPosition(playerEntryFields[i]->getPosition() + Vector2f(distance, 0));
			if (teamSizeList->getSelectedItemIndex() > 0)
			{
				playerEntryFields2[i]->setPosition(playerEntryFields2[i]->getPosition() + Vector2f(distance, 0));
			}
			if (teamSizeList->getSelectedItemIndex() == 2)
			{
				playerEntryFields3[i]->setPosition(playerEntryFields3[i]->getPosition() + Vector2f(distance, 0));
			}
		}
		//Yield the thread for one frame, so the movement is actually visible.
		std::this_thread::yield();
		std::this_thread::sleep_for(tick);
	}
	//It doesn't matter if the Panel overshoots its mark while it goes offscreen,
	// so there is no need for "difference" code like there is in "slideOutCoroutine".
	return;
}

//Draw all of the Panel's visual objects.
void gameSetupPanel::draw()
{
	//Draw the Background.
	window->draw(background);
	//Draw the Header.
	window->draw(header);
	//Draw the Player List Header.
	window->draw(playerListHeader);
	//Draw all of the Player Numbers.
	for (int i = 0; i < (int)playerNumbers.size(); i++)
	{
		window->draw(playerNumbers[i]);
		//Draw all of the "Remove Player" Buttons if there are 3 or more.
		if ((int)playerNumbers.size() > 2)
		{
			window->draw(gameSetupButtonsRemove[i]);
		}
	}
	//Draw the "Add Player" Button (if there are less than 10 Player Fields onscreen).
	if ((int)playerNumbers.size() < MAX_PLAYERS)
	{
		window->draw(gameSetupButtonAdd);
	}
	//Draw the Game Type Header.
	window->draw(gameTypeHeader);
	//Draw the Team Size Header.
	window->draw(teamSizeHeader);
	//Draw the GUI objects ("Add Player" TextBox, Player Entry Fields, Game Type List, Team Size List, and "Add Player" and "Create Game" Buttons).
	gui->draw();
}
