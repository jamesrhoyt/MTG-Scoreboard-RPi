/*
 * startupScreen.cpp
 */

#include "startupScreen.h"

int startupScreen::runStartup()
{
	//Limit the framerate to reduce the CPU load.
	window->setFramerateLimit(60);

	//Load the Background Texture, and set its Sprite.
	if (!backgroundTexture.loadFromFile(resourcePath + "startupBackground.png"))
	{
        if (!backgroundTexture.loadFromFile(altResourcePath + "startupBackground.png"))
        {
            return 1;
        }
	}
	background = Sprite(backgroundTexture);
	//Position and scale the background to fill the window.
	background.setPosition(0, 0);
	background.setScale(window->getSize().x / background.getGlobalBounds().width, window->getSize().y / background.getGlobalBounds().height);

	//Load the Title Logo Texture, and set its Sprite.
	if (!titleLogoTexture.loadFromFile(resourcePath + "titleLogo.png"))
	{
        if (!titleLogoTexture.loadFromFile(altResourcePath + "titleLogo.png"))
        {
            return 1;
		}
	}
	titleLogo = Sprite(titleLogoTexture);
	//Scale the logo so it is 2/3 the width of the screen, and 1/5 the height.
	titleLogo.setScale((window->getSize().x * (.67f)) / titleLogo.getGlobalBounds().width, (window->getSize().y / 5.0f) / titleLogo.getGlobalBounds().height);
	//Center the logo on the screen, and position it 1/4 of the way down the screen.
	titleLogo.setPosition((window->getSize().x / 2) - (titleLogo.getGlobalBounds().width / 2), (window->getSize().y * .25f) - (titleLogo.getGlobalBounds().height / 2));

	//Set up the Start Text.
	startText.setFont(*font_mtg);
	startText.setCharacterSize(32);
	startText.setFillColor(Color::Transparent);
	startText.setString("Click/Connect To Start");
	//Center the Text, and position it two-thirds of the way down the window.
	startText.setPosition((window->getSize().x / 2) - (startText.getGlobalBounds().width / 2), window->getSize().y * .67f);

	//Create the Clock for the Start Text.
	startTextClock = Clock();

	//Set up the Version Number Text.
	versionNumberText.setFont(*font_mtg);
	versionNumberText.setCharacterSize(24);
	versionNumberText.setFillColor(Color::Black);
	versionNumberText.setString("version 1.0.1");
	//Position the Text in the lower-right corner of the window.
	versionNumberText.setPosition(window->getSize().x - versionNumberText.getGlobalBounds().width, window->getSize().y - (versionNumberText.getGlobalBounds().height * 2));

	//Set up the Connection URL Text.
	connectURLText.setFont(*font_dejavu);
	connectURLText.setCharacterSize(24);
	connectURLText.setFillColor(Color::Black);
	connectURLText.setString("Connect to: 192.168.1.201/mtg_lobby.php");
	//Position the Text above the Version Number.
    connectURLText.setPosition(window->getSize().x - connectURLText.getGlobalBounds().width, versionNumberText.getPosition().y - (versionNumberText.getGlobalBounds().height * 2));

	//Run the main Window loop.
	while (window->isOpen())
	{
        //Check if the program already has any active games.
        res = stmt->executeQuery("SELECT COUNT(*) FROM games WHERE active=1");
        res->next();
        //If it does, skip to the panelManager.
        //(This is in case the program has no local input devices.)
        if(res->getInt(1) > 0)
        {
            //Fade the "Start" Text and Title Graphic out before continuing.
			//First, record the Start Text's current alpha value.
			startTextAlpha = startText.getFillColor().a;
			//Second, have a loop run until the Start Text is faded out.
			while (startTextAlpha > 0)
			{
				//Change the "Start Text" alpha value.
				startTextAlpha -= 4;
				if (startTextAlpha < 0) startTextAlpha = 0;
				startText.setFillColor(Color(startText.getFillColor().r, startText.getFillColor().g, startText.getFillColor().b, startTextAlpha));
				//Draw Code:
				window->clear();
				window->draw(background);
				window->draw(titleLogo);
				window->draw(startText);
				window->draw(versionNumberText);
				window->draw(connectURLText);
				window->display();
			}
			//Run the main GamePanel Manager.
            pManager.runPanelManager();
        }

		sf::Event event;
		while (window->pollEvent(event))
		{
			//"Close Requested" Event: Close the Window.
			if (event.type == sf::Event::Closed)
			{
				//Delete all of the SQL objects.
				delete res;
				delete prep_stmt;
				delete stmt;
				delete con;
				window->close();
			}

			//Escape Key pressed: Close the Window.
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
                //Delete all of the SQL objects.
				delete res;
				delete prep_stmt;
				delete stmt;
				delete con;
				window->close();
			}

			//If the user clicks with any Mouse Button.
			if (event.type == sf::Event::MouseButtonReleased)
			{
				//Fade the "Start" Text and Title Graphic out before continuing.
				//First, record the Start Text's current alpha value.
				startTextAlpha = startText.getFillColor().a;
				//Second, have a loop run until the Start Text is faded out.
				while (startTextAlpha > 0)
				{
					//Change the "Start Text" alpha value.
					startTextAlpha -= 4;
					if (startTextAlpha < 0) startTextAlpha = 0;
					startText.setFillColor(Color(startText.getFillColor().r, startText.getFillColor().g, startText.getFillColor().b, startTextAlpha));
					//Draw Code:
					window->clear();
					window->draw(background);
					window->draw(titleLogo);
					window->draw(startText);
					window->draw(versionNumberText);
					window->draw(connectURLText);
					window->display();
				}
				//Run the main GamePanel Manager.
				pManager.runPanelManager();
			}
		}

		//Change the "Start Text" alpha value.
		startTextAlpha = (int)((1.5f - abs(startTextClock.getElapsedTime().asSeconds() - 1.5f)) * 255);
		//Because "a" is a UInt8, the value needs to be clamped between 0 and 255.
		if (startTextAlpha > 255)
		{
			startTextAlpha = 255;
		}
		else if (startTextAlpha < 0)
		{
			startTextAlpha = 0;
		}
		//Update the "Start Text"'s Color using startTextAlpha.
		startText.setFillColor(Color(startText.getFillColor().r, startText.getFillColor().g, startText.getFillColor().b, startTextAlpha));
		//After three seconds, restart the Clock to reset the cycle.
		if (startTextClock.getElapsedTime().asSeconds() > 3.0f)
		{
			startTextClock.restart();
		}

		//Draw Code:
		window->clear();
		window->draw(background);
		window->draw(titleLogo);
		window->draw(startText);
		window->draw(versionNumberText);
		window->draw(connectURLText);
		window->display();
	}
	return 0;
}
