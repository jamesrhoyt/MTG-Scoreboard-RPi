/*
 * main.cpp
 */

#include <cstdio>
#include <iostream>
#include "startupScreen.h"

int main()
{
	//Set up the SQL connection.
	driver = sql::mysql::get_mysql_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306/mtg", "root", "mtgscoreboard");
	stmt = con->createStatement();
	//Create the Window object.
	window = new sf::RenderWindow(VideoMode::getDesktopMode(), "MTG Scoreboard", Style::Fullscreen);
	//window = new sf::RenderWindow(sf::VideoMode(640,480)/*VideoMode::getDesktopMode()*/, "MTG Scoreboard"/*, Style:: Fullscreen*/);
	window->setVerticalSyncEnabled(true);
	window->setMouseCursorVisible(true);
	window->setActive(true);
	//window->setPosition(sf::Vector2i(200,200));
	//Create the Gui object.
	gui = new tgui::Gui(*window);
	//Create the Font objects.
	font_mtg = new sf::Font();
	if(!font_mtg->loadFromFile(resourcePath + "MAGIC.TTF"))
	{
        font_mtg->loadFromFile(altResourcePath + "MAGIC.TTF");
	}
	font_dejavu = new sf::Font();
	if(!font_dejavu->loadFromFile(resourcePath + "DejaVuSans.ttf"))
	{
        font_dejavu->loadFromFile(altResourcePath + "DejaVuSans.ttf");
	}
	//Create and load the Texture objects.
	//Load the Player Button Textures.
	buttonCommanderMinusOneTexture = new sf::Texture();
	if(!buttonCommanderMinusOneTexture->loadFromFile(resourcePath + "buttonCommanderMinusOne.png"))
	{
        buttonCommanderMinusOneTexture->loadFromFile(altResourcePath + "buttonCommanderMinusOne.png");
	}
	buttonLifePlusOneTexture = new sf::Texture();
	if(!buttonLifePlusOneTexture->loadFromFile(resourcePath + "buttonLifePlusOne.png"))
	{
        buttonLifePlusOneTexture->loadFromFile(altResourcePath + "buttonLifePlusOne.png");
	}
	buttonLifePlusFiveTexture = new sf::Texture();
	if(!buttonLifePlusFiveTexture->loadFromFile(resourcePath + "buttonLifePlusFive.png"))
	{
        buttonLifePlusFiveTexture->loadFromFile(altResourcePath + "buttonLifePlusFive.png");
	}
	buttonLifeMinusFiveTexture = new sf::Texture();
	if(!buttonLifeMinusFiveTexture->loadFromFile(resourcePath + "buttonLifeMinusFive.png"))
	{
        buttonLifeMinusFiveTexture->loadFromFile(altResourcePath + "buttonLifeMinusFive.png");
	}
	buttonLifeMinusTenTexture = new sf::Texture();
	if(!buttonLifeMinusTenTexture->loadFromFile(resourcePath + "buttonLifeMinusTen.png"))
	{
        buttonLifeMinusTenTexture->loadFromFile(altResourcePath + "buttonLifeMinusTen.png");
	}
	buttonPoisonPlusOneTexture = new sf::Texture();
	if(!buttonPoisonPlusOneTexture->loadFromFile(resourcePath + "buttonPoisonPlusOne.png"))
	{
        buttonPoisonPlusOneTexture->loadFromFile(altResourcePath + "buttonPoisonPlusOne.png");
	}
	buttonPoisonMinusOneTexture = new sf::Texture();
	if(!buttonPoisonMinusOneTexture->loadFromFile(resourcePath + "buttonPoisonMinusOne.png"))
	{
        buttonPoisonMinusOneTexture->loadFromFile(altResourcePath + "buttonPoisonMinusOne.png");
	}
	//Load the Texture for the Button to collapse the Commander Counters.
	buttonCommanderCloseTexture = new sf::Texture();
	if(!buttonCommanderCloseTexture->loadFromFile(resourcePath + "buttonCommanderClose.png"))
	{
        buttonCommanderCloseTexture->loadFromFile(altResourcePath + "buttonCommanderClose.png");
	}
	//Load the Texture for the Button to add Players to the game.
	addPlayerPanelButtonTexture = new sf::Texture();
	if(!addPlayerPanelButtonTexture->loadFromFile(resourcePath + "gameSetupButtonAdd.png"))
	{
        addPlayerPanelButtonTexture->loadFromFile(altResourcePath + "gameSetupButtonAdd.png");
	}
	//Load the Texture for the background.
	panelTexture = new sf::Texture();
	if(!panelTexture->loadFromFile(resourcePath + "startupBackground.png"))
	{
        panelTexture->loadFromFile(altResourcePath + "startupBackground.png");
	}
	//Create the Avatar Texture Objects.
	//Get the file path for each of the avatar images.
	res = stmt->executeQuery("SELECT id, avatar FROM players ORDER BY id ASC");
	while (res->next())
	{
		//Create an empty Texture object.
		avatarTextures.push_back(new sf::Texture());
		//If a Player does not have an Avatar, give them a copy of the default one.
		if (!avatarTextures[res->getInt("id")]->loadFromFile(avatarPath + res->getString("avatar")))
		{
            if (!avatarTextures[res->getInt("id")]->loadFromFile(altAvatarPath + res->getString("avatar")))
            {
                if(!avatarTextures[res->getInt("id")]->loadFromFile(avatarPath + "player_images/default.jpg"))
                {
                    avatarTextures[res->getInt("id")]->loadFromFile(altAvatarPath + "player_images/default.jpg");
                }
            }
		}
	}
	//Create and run the startup screen.
	startupScreen startup;
	return startup.runStartup();
}
