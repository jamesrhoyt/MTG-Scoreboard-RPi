/*
 * panelManager.cpp
 */

#include "panelManager.h"

int panelManager::runPanelManager()
{
	//Save the Window's width and height values locally.
	windowWidth = window->getSize().x;
	windowHeight = window->getSize().y;

	//Load the Background Texture, and set its Sprite.
	if (!backgroundTexture.loadFromFile(resourcePath + "startupBackground.png"))
	{
        if (!backgroundTexture.loadFromFile(altResourcePath + "startupBackground.png"))
        {
            return 1;
		}
	}
	background = Sprite(backgroundTexture);
	//Resize and reposition the background to fill the window.
	background.setPosition(0, 0);
	background.setScale(windowWidth / background.getGlobalBounds().width, windowHeight / background.getGlobalBounds().height);

	//Set up the "Waiting" Text object.
	waitingString = "Awaiting Remote Connection";
	waitingText.setString(waitingString);
	waitingText.setFont(*font_mtg);
	waitingText.setCharacterSize(40);
	waitingText.setFillColor(Color::Black);
	//Position the Text in the very center of the screen.
	waitingText.setPosition(windowWidth / 2 - waitingText.getGlobalBounds().width / 2, windowHeight / 2 - waitingText.getGlobalBounds().height / 2);
	//Start the timer to "animate" the Waiting Text.
	waitingTextClock = Clock();
	waitingTextEllipsisDots = 0;

	//Start the timer to update the Remote Games.
	panelUpdateTimer = Clock();

	//Load the Textures for the "Create Game" tab, then set up its Sprite using the default one.
	if (!createGameTabClosedTexture.loadFromFile(resourcePath + "createGameTabClosed.png"))
	{
        if (!createGameTabClosedTexture.loadFromFile(altResourcePath + "createGameTabClosed.png"))
        {
            return 1;
		}
	}
	if (!createGameTabOpenTexture.loadFromFile(resourcePath + "createGameTabOpen.png"))
	{
        if (!createGameTabOpenTexture.loadFromFile(altResourcePath + "createGameTabOpen.png"))
        {
            return 1;
		}
	}
	createGameTab = Sprite(createGameTabClosedTexture);
	//Position the Tab in the upper-right corner of the screen.
	createGameTab.setPosition(windowWidth - createGameTab.getGlobalBounds().width, 0);

	//Set up the variables for the "Create Game" setup Panel.
	setupPanelOpen = false;

	//Create the click event for the Game Setup Panel's "Create Game" button.
	setupPanel.createGameButton->connect("clicked", &panelManager::addPanel, this, -1);

	//Run the PanelManager's window loop.
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			//Check the Game Setup Panel for user input.
			if (setupPanelOpen)
			{
				setupPanel.getUserInput(event);
			}

			//Check each of the Game Panels for user input.
			else
			{
				for (int i = 0; i < (int)panels.size(); i++)
				{
					panels[i]->getUserInput(event);
				}
			}

			//"Close Requested" Event: Close the Window.
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}

			//Escape Key: Go back to the Startup Screen.
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				return 0;
			}

			//Tab Key: Open/Close the Game Setup Panel.
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
			{
                //If the Game Setup Panel isn't open, open it.
                if (!setupPanelOpen)
                {
                    //Join the thread from the last time it was called, if it was.
                    if (setupPanel.slideInThread.joinable()) setupPanel.slideInThread.join();
                    //Set the flag to check the Setup Panel for input.
                    setupPanelOpen = true;
                    //Switch the Texture for the tab.
                    createGameTab.setTexture(createGameTabOpenTexture);
                    //Start sliding the Panel out.
                    setupPanel.slideOut(createGameTab);
                    //Hide the TGUI objects on the Game Panels, to prevent accidental clicks.
                    for (int i = 0; i < (int)panels.size(); i++)
                    {
                        panels[i]->disableGUI();
                    }
                }
                //If the Panel is open, close it.
                /*else
                {
                    //Join the thread from the last time it was called, if it was.
                    if (setupPanel.slideOutThread.joinable()) setupPanel.slideOutThread.join();
                    //Set the flag to stop checking the Setup Panel for input.
                    setupPanelOpen = false;
                    //Switch the Texture for the tab.
                    createGameTab.setTexture(createGameTabClosedTexture);
                    //Start sliding the Panel back in.
                    setupPanel.slideIn(createGameTab);
                    //Show the TGUI objects on the Game Panels again.
                    for (int i = 0; i < (int)panels.size(); i++)
                    {
                        panels[i]->enableGUI();
                    }
                }*/
			}

			//Q Key: Open/Close the Game Setup Panel.
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
                //If the Game Setup Panel isn't open, open it.
                /*if (!setupPanelOpen)
                {
                    //Join the thread from the last time it was called, if it was.
                    if (setupPanel.slideInThread.joinable()) setupPanel.slideInThread.join();
                    //Set the flag to check the Setup Panel for input.
                    setupPanelOpen = true;
                    //Switch the Texture for the tab.
                    createGameTab.setTexture(createGameTabOpenTexture);
                    //Start sliding the Panel out.
                    setupPanel.slideOut(createGameTab);
                    //Hide the TGUI objects on the Game Panels, to prevent accidental clicks.
                    for (int i = 0; i < (int)panels.size(); i++)
                    {
                        panels[i]->disableGUI();
                    }
                }*/
                //If the Panel is open, close it.
                //else
                //{
                    //Join the thread from the last time it was called, if it was.
                    if (setupPanel.slideOutThread.joinable()) setupPanel.slideOutThread.join();
                    //Set the flag to stop checking the Setup Panel for input.
                    setupPanelOpen = false;
                    //Switch the Texture for the tab.
                    createGameTab.setTexture(createGameTabClosedTexture);
                    //Start sliding the Panel back in.
                    setupPanel.slideIn(createGameTab);
                    //Show the TGUI objects on the Game Panels again.
                    for (int i = 0; i < (int)panels.size(); i++)
                    {
                        panels[i]->enableGUI();
                    }
                //}
			}

			//Left Mouse Clicks
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				mousePos = sf::Vector2f(sf::Mouse::getPosition());
				//Check the "Create Game" tab.
				if (createGameTab.getGlobalBounds().contains(mousePos))
				{
					//If the Game Setup Panel isn't open, open it.
					if (!setupPanelOpen)
					{
						//Join the thread from the last time it was called, if it was.
						if (setupPanel.slideInThread.joinable()) setupPanel.slideInThread.join();
						//Set the flag to check the Setup Panel for input.
						setupPanelOpen = true;
						//Switch the Texture for the tab.
						createGameTab.setTexture(createGameTabOpenTexture);
						//Start sliding the Panel out.
						setupPanel.slideOut(createGameTab);
						//Hide the TGUI objects on the Game Panels, to prevent accidental clicks.
						for (int i = 0; i < (int)panels.size(); i++)
						{
							panels[i]->disableGUI();
						}
					}
					//If the Panel is open, close it.
					else
					{
						//Join the thread from the last time it was called, if it was.
						if (setupPanel.slideOutThread.joinable()) setupPanel.slideOutThread.join();
						//Set the flag to stop checking the Setup Panel for input.
						setupPanelOpen = false;
						//Switch the Texture for the tab.
						createGameTab.setTexture(createGameTabClosedTexture);
						//Start sliding the Panel back in.
						setupPanel.slideIn(createGameTab);
						//Show the TGUI objects on the Game Panels again.
						for (int i = 0; i < (int)panels.size(); i++)
						{
							panels[i]->enableGUI();
						}
					}
				}
			}
		}

		//Every half-second, cycle the number of dots "waitingText" ends in.
		if (waitingTextClock.getElapsedTime().asSeconds() > .5)
		{
			//If there is already a full ellipsis, remove it.
			if (waitingTextEllipsisDots >= 3)
			{
				waitingString = waitingString.substr(0, waitingString.size() - 3);
				waitingTextEllipsisDots = 0;
			}
			//Otherwise, append a new dot to the string.
			else
			{
				waitingString += ".";
				waitingTextEllipsisDots++;
			}
			waitingText.setString(waitingString);
			//Reset the timer.
			waitingTextClock.restart();
		}

		//Update all of the Remote Game Panels every second.
		if (panelUpdateTimer.getElapsedTime().asSeconds() > 1)
		{
			//Iterate through all of the Panels, both Local and Remote.
			for (int i = 0; i < (int)panels.size(); i++)
			{
				//All of the Local Game Panels have IDs of -1.
				if (panels[i]->remoteGameID >= 0)
				{
					panels[i]->update();
				}
			}
			//Reset the timer.
			panelUpdateTimer.restart();
		}

		//Draw Code:
		window->clear();
		window->draw(background);
		//Check for remotely-created Game Panels.
		checkForPanels();
		//If there are no Game Panels, draw the Waiting Text.
		if ((int)panels.size() == 0)
		{
			window->draw(waitingText);
		}
		//Otherwise, draw each of the Panels.
		else
		{
			for (int i = 0; i < (int)panels.size(); i++)
			{
				panels[i]->draw();
			}
		}
		//Draw the Tab for the Setup Panel.
		window->draw(createGameTab);
		//Draw the Setup Panel, even if it isn't open.
		setupPanel.draw();
		//Display all of the drawn objects.
		window->display();
	}
	return 0;
}

//Query the "games" SQL Table in the database for any Remote Games.
void panelManager::checkForPanels()
{
	//Find any games in the table that are currently active, and sort them by ID.
	res = stmt->executeQuery("SELECT id FROM games WHERE active=1 ORDER BY id ASC");
	//Get the next game in the list, if there is one.
	while (res->next())
	{
		//Pull the ID for the next game in the list.
		int gameID = res->getInt(1);
		//Set the flag to check that this game does not already have a Panel.
		bool gameExists = false;
		//Check every Panel that currently exists.
		for (int i = 0; i < (int)panels.size(); i++)
		{
			//If any of their IDs match this game's, this game already has a Panel.
			if (panels[i]->remoteGameID == gameID)
			{
				gameExists = true;
			}
		}
		//If this game does not have a Panel, create one.
		if (!gameExists)
		{
			addPanel(gameID);
		}
	}
}

//Create a new gamePanel and add it to the "panels" vector, either locally
//(via the Game Setup Panel) or remotely (via the "checkForPanels" method).
void panelManager::addPanel(int gameID)
{
	//Check if this is a manually-created gamePanel.
	if (gameID == -1)
	{
		//Slide the Game Setup Panel back offscreen.
		if (setupPanel.slideOutThread.joinable()) setupPanel.slideOutThread.join();
		setupPanelOpen = false;
		createGameTab.setTexture(createGameTabClosedTexture);
		setupPanel.slideIn(createGameTab);
		//Re-enable all of the TGUI objects for the rest of the Panels.
		for (int i = 0; i < (int)panels.size(); i++)
		{
			panels[i]->enableGUI();
		}
		//Add this panel to the vector of panels, using the Player Names, their respective IDs, and the Game Type and Team Size chosen in the Game Setup Panel.
		//panels.push_back(gamePanel(setupPanel.getNames(), setupPanel.getIDs(), setupPanel.getGameType(), setupPanel.getTeamSize()));
		//If this is a non-Emperor game, use the generic Panel constructor.
		if (setupPanel.getGameType() != 2)
		{
			//std::unique_ptr<gamePanel> newPanel(new gamePanel(setupPanel.getNames(), setupPanel.getIDs(), setupPanel.getGameType(), setupPanel.getTeamSize()));
			panels.push_back(std::unique_ptr<gamePanel>(new gamePanel(setupPanel.getNames(), setupPanel.getIDs(), setupPanel.getGameType(), setupPanel.getTeamSize())));
		}
		//Otherwise, use the Emperor Panel constructor.
		else
		{
			//std::unique_ptr<gamePanel> newPanel(new gamePanelEmperor(setupPanel.getNames(), setupPanel.getIDs(), setupPanel.getTeamSize()));
			panels.push_back(std::unique_ptr<gamePanel>(new gamePanelEmperor(setupPanel.getNames(), setupPanel.getIDs(), setupPanel.getTeamSize())));
		}
	}
	//If not, use the "remote" constructor.
	else
	{
		//Create an object to hold the entered "name" of this game, which will distinguish it from other Remote Games.
		sql::SQLString gameName;
		//Pull the name of the game whose ID matches the one passed in via "gameID".
		res = stmt->executeQuery(sql::SQLString("SELECT name, game_type FROM games WHERE id=").append(std::to_string(gameID)));
		while (res->next())
		{
			gameName = res->getString(1);
            //Add a new panel to the vector of panels, using the game's ID and its "name".
            //panels.push_back(gamePanel(gameID, gameName));
            //If this is a non-Emperor game, use the generic Panel constructor.
            if (res->getInt(2) != 2)
            {
                //std::unique_ptr<gamePanel> newPanel(new gamePanel(gameID, gameName));
                panels.push_back(std::unique_ptr<gamePanel>(new gamePanel(gameID, gameName)));
            }
            //Otherwise, use the Emperor Panel constructor.
            else
            {
                //std::unique_ptr<gamePanel> newPanel(new gamePanelEmperor(gameID, gameName));
                panels.push_back(std::unique_ptr<gamePanel>(new gamePanelEmperor(gameID, gameName)));
            }
        }
	}
	//Resize and reposition the rest of the GamePanels to make room for this one.
	//Get the current number of panels on screen.
	switch ((int)panels.size() - 1)
	{
		//Only one Panel:
	case 0:
		panels[0]->setTargetSize(windowWidth, windowHeight);
		panels[0]->setTargetPosition(0, 0);
		break;
		//Two Panels:
	case 1:
		//Portrait:
		if (windowHeight >= windowWidth)
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth, windowHeight / 2);
			panels[0]->setTargetPosition(0, 0);
			//Set up 2nd Panel
			panels[1]->setTargetSize(windowWidth, windowHeight / 2);
			panels[1]->setTargetPosition(0, windowHeight / 2);
		}
		//Landscape:
		else
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth / 2, windowHeight);
			panels[0]->setTargetPosition(0, 0);
			//Set up 2nd Panel
			panels[1]->setTargetSize(windowWidth / 2, windowHeight);
			panels[1]->setTargetPosition(windowWidth / 2, 0);
		}
		break;
		//Three Panels:
	case 2:
		//Portrait:
		if (windowHeight >= windowWidth)
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth, windowHeight / 3);
			panels[0]->setTargetPosition(0, 0);
			//Resize 2nd Panel
			panels[1]->setTargetSize(windowWidth, windowHeight / 3);
			panels[1]->setTargetPosition(0, windowHeight / 3);
			//Set up 3rd Panel
			panels[2]->setTargetSize(windowWidth, windowHeight / 3);
			panels[2]->setTargetPosition(0, windowHeight * 2 / 3);
		}
		//Landscape:
		else
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth * 2 / 3, windowHeight / 2);
			panels[0]->setTargetPosition(windowWidth / 6, 0);
			//Resize 2nd Panel
			panels[1]->setTargetSize(windowWidth / 2, windowHeight / 2);
			panels[1]->setTargetPosition(0, windowHeight / 2);
			//Set up 3rd Panel
			panels[2]->setTargetSize(windowWidth / 2, windowHeight / 2);
			panels[2]->setTargetPosition(windowWidth / 2, windowHeight / 2);

		}
		break;
		//Four Panels:
	case 3:
		//Resize 1st Panel
		panels[0]->setTargetSize(windowWidth / 2, windowHeight / 2);
		panels[0]->setTargetPosition(0, 0);
		//Resize 2nd Panel
		panels[1]->setTargetSize(windowWidth / 2, windowHeight / 2);
		panels[1]->setTargetPosition(windowWidth / 2, 0);
		//Resize 3rd Panel
		panels[2]->setTargetSize(windowWidth / 2, windowHeight / 2);
		panels[2]->setTargetPosition(0, windowHeight / 2);
		//Set 4th Panel
		panels[3]->setTargetSize(windowWidth / 2, windowHeight / 2);
		panels[3]->setTargetPosition(windowWidth / 2, windowHeight / 2);
		break;
		//Five Panels:
	case 4:
		//Portrait:
		if (windowHeight >= windowWidth)
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[0]->setTargetPosition(0, 0);
			//Resize 2nd Panel
			panels[1]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[1]->setTargetPosition(windowWidth / 2, 0);
			//Resize 3rd Panel
			panels[2]->setTargetSize(windowWidth, windowHeight / 3);
			panels[2]->setTargetPosition(0, windowHeight / 3);
			//Resize 4th Panel
			panels[3]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[3]->setTargetPosition(0, windowHeight * 2 / 3);
			//Set 5th Panel
			panels[4]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[4]->setTargetPosition(windowWidth / 2, windowHeight * 2 / 3);
		}
		//Landscape:
		else
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth / 2, windowHeight / 2);
			panels[0]->setTargetPosition(0, 0);
			//Resize 2nd Panel
			panels[1]->setTargetSize(windowWidth / 2, windowHeight / 2);
			panels[1]->setTargetPosition(0, windowHeight / 2);
			//Resize 3rd Panel
			panels[2]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[2]->setTargetPosition(windowWidth / 2, 0);
			//Resize 4th Panel
			panels[3]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[3]->setTargetPosition(windowWidth / 2, windowHeight / 3);
			//Set 5th Panel
			panels[4]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[4]->setTargetPosition(windowWidth / 2, windowHeight * 2 / 3);
		}
		break;
		//Six Panels:
	case 5:
		//Resize 1st Panel
		panels[0]->setTargetSize(windowWidth / 2, windowHeight / 3);
		panels[0]->setTargetPosition(0, 0);
		//Resize 2nd Panel
		panels[1]->setTargetSize(windowWidth / 2, windowHeight / 3);
		panels[1]->setTargetPosition(0, windowHeight / 3);
		//Resize 3rd Panel
		panels[2]->setTargetSize(windowWidth / 2, windowHeight / 3);
		panels[2]->setTargetPosition(0, windowHeight * 2 / 3);
		//Resize 4th Panel
		panels[3]->setTargetSize(windowWidth / 2, windowHeight / 3);
		panels[3]->setTargetPosition(windowWidth / 2, 0);
		//Resize 5th Panel
		panels[4]->setTargetSize(windowWidth / 2, windowHeight / 3);
		panels[4]->setTargetPosition(windowWidth / 2, windowHeight / 3);
		//Set 6th Panel
		panels[5]->setTargetSize(windowWidth / 2, windowHeight / 3);
		panels[5]->setTargetPosition(windowWidth / 2, windowHeight * 2 / 3);
		break;
		//Seven Panels:
	case 6:
		//Portrait:
		if (windowHeight >= windowWidth)
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[0]->setTargetPosition(0, 0);
			//Resize 2nd Panel
			panels[1]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[1]->setTargetPosition(windowWidth / 2, 0);
			//Resize 3rd Panel
			panels[2]->setTargetSize(windowWidth / 3, windowHeight / 3);
			panels[2]->setTargetPosition(0, windowHeight / 3);
			//Resize 4th Panel
			panels[3]->setTargetSize(windowWidth / 3, windowHeight / 3);
			panels[3]->setTargetPosition(windowWidth / 3, windowHeight / 3);
			//Resize 5th Panel
			panels[4]->setTargetSize(windowWidth / 3, windowHeight / 3);
			panels[4]->setTargetPosition(windowWidth * 2 / 3, windowHeight / 3);
			//Resize 6th Panel
			panels[5]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[5]->setTargetPosition(0, windowHeight * 2 / 3);
			//Set 7th Panel
			panels[6]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[6]->setTargetPosition(windowWidth / 2, windowHeight * 2 / 3);
		}
		//Landscape:
		else
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[0]->setTargetPosition(0, 0);
			//Resize 2nd Panel
			panels[1]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[1]->setTargetPosition(windowWidth / 4, 0);
			//Resize 3rd Panel
			panels[2]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[2]->setTargetPosition(0, windowHeight / 2);
			//Resize 4th Panel
			panels[3]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[3]->setTargetPosition(windowWidth / 4, windowHeight / 2);
			//Resize 5th Panel
			panels[4]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[4]->setTargetPosition(windowWidth / 2, 0);
			//Resize 6th Panel
			panels[5]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[5]->setTargetPosition(windowWidth / 2, windowHeight / 3);
			//Set 7th Panel
			panels[6]->setTargetSize(windowWidth / 2, windowHeight / 3);
			panels[6]->setTargetPosition(windowWidth / 2, windowHeight * 2 / 3);
		}
		break;
		//Eight Panels:
	case 7:
		//Portrait:
		if (windowHeight >= windowWidth)
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[0]->setTargetPosition(0, 0);
			//Resize 2nd Panel
			panels[1]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[1]->setTargetPosition(windowWidth / 2, 0);
			//Resize 3rd Panel
			panels[2]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[2]->setTargetPosition(0, windowHeight / 4);
			//Resize 4th Panel
			panels[3]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[3]->setTargetPosition(windowWidth / 2, windowHeight / 4);
			//Resize 5th Panel
			panels[4]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[4]->setTargetPosition(0, windowHeight / 2);
			//Resize 6th Panel
			panels[5]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[5]->setTargetPosition(windowWidth / 2, windowHeight / 2);
			//Resize 7th Panel
			panels[6]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[6]->setTargetPosition(0, windowHeight * 3 / 4);
			//Set 8th Panel
			panels[7]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[7]->setTargetPosition(windowWidth / 2, windowHeight * 3 / 4);
		}
		//Landscape:
		else
		{
			//Resize 1st Panel
			panels[0]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[0]->setTargetPosition(0, 0);
			//Resize 2nd Panel
			panels[1]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[1]->setTargetPosition(windowWidth / 4, 0);
			//Resize 3rd Panel
			panels[2]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[2]->setTargetPosition(windowWidth / 2, 0);
			//Resize 4th Panel
			panels[3]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[3]->setTargetPosition(windowWidth * 3 / 4, 0);
			//Resize 5th Panel
			panels[4]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[4]->setTargetPosition(0, windowHeight / 2);
			//Resize 6th Panel
			panels[5]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[5]->setTargetPosition(windowWidth / 4, windowHeight / 2);
			//Resize 7th Panel
			panels[6]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[6]->setTargetPosition(windowWidth / 2, windowHeight / 2);
			//Set 8th Panel
			panels[7]->setTargetSize(windowWidth / 4, windowHeight / 2);
			panels[7]->setTargetPosition(windowWidth * 3 / 4, windowHeight / 2);
		}
		break;
	}
}
