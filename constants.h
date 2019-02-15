/*
 * constants.h
 */

#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <chrono>
#include <connection.h>
#include <driver.h>
#include <prepared_statement.h>
#include <resultset.h>
#include <statement.h>
#include <functional>
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <Graphics.hpp>
#include <Gui.hpp>
#include <Widget.hpp>
#include <Widgets/ClickableWidget.hpp>
#include <Widgets/Label.hpp>
#include <Widgets/Scrollbar.hpp>
#include <Widgets/TextBox.hpp>
#include <thread>

using namespace sf;

#pragma once

//The file paths for the Art assets and the Player Avatars.
const std::string resourcePath = "./Resources/";
const std::string avatarPath = resourcePath;

//The maximum number of players, per game.
const int MAX_PLAYERS = 10;

//The amount of time between frames (1/60th of a second); used in threads.
const std::chrono::milliseconds tick = std::chrono::milliseconds(16);

//SQL Objects:
extern sql::mysql::MySQL_Driver *driver;
extern sql::Connection *con;
extern sql::Statement *stmt;				//The basic statement object, used in retrieving data from the database.
extern sql::PreparedStatement *prep_stmt;	//An object for advanced statements, used to add Players to the database.
extern sql::ResultSet *res;					//An object used to hold result tables from SQL queries.

//Program Window:
extern RenderWindow* window;

//GUI (used to draw TGUI Text Boxes):
extern tgui::Gui* gui;

//Fonts:
extern Font* font_mtg;
extern Font* font_dejavu;

//Player Widget Textures:
extern Texture* buttonCommanderMinusOneTexture;
extern Texture* buttonLifePlusOneTexture;
extern Texture* buttonLifePlusFiveTexture;
extern Texture* buttonLifeMinusFiveTexture;
extern Texture* buttonLifeMinusTenTexture;
extern Texture* buttonPoisonPlusOneTexture;
extern Texture* buttonPoisonMinusOneTexture;
extern Texture* buttonCommanderCloseTexture;

//Other Button Textures:
extern Texture* addPlayerPanelButtonTexture;

//Backgrounds:
extern Texture* panelTexture;

//Player Avatar Textures:
extern std::vector<Texture*> avatarTextures;

//Holds the UI objects and underlying variables for each Player instance.
struct player
{
	//Player Objects:
	int id;								//The ID number for the Player as listed in the database.
	tgui::TextBox::Ptr displayName;		//The TextBox to display the Player's name.
	std::string name;					//The Player's name, used to set displayName's text.
	Sprite avatar;						//The Player's avatar, set using the database and "avatarPath".
	Text lifeCounter;					//The Text object to display the Player's life total.
	int life;							//The Player's current life total, used to set lifeCounter's text.
	int startingLife;					//The amount of life each Player starts the game with, based on game type.
	bool isAlive;						//Whether or not this Player has been defeated.
	bool handshake;						//The flag for whether this Player is still part of the game (used in Remote Games).
	Sprite buttonLifePlusOne;			//The Sprite to add one to the Player's life total (used in Local Games).
	Sprite buttonLifePlusFive;			//The Sprite to add five to the Player's life total (used in Local Games).
	Sprite buttonLifeMinusFive;			//The Sprite to subtract five from the Player's life total (used in Local Games).
	Sprite buttonLifeMinusTen;			//The Sprite to subtract ten from the Player's life total (used in Local Games).
	Text poisonCounter;					//The Text object to display the Player's poison total.
	int poison;							//The Player's current poison total, used to set poisonCounter's text.
	int startingPoison;					//The amount of poison each Player starts the game with; used to reset the Counter.
	Sprite buttonPoisonPlusOne;			//The Sprite to add one to the Player's poison total (used in Local Games).
	Sprite buttonPoisonMinusOne;		//The Sprite to subtract one from the Player's life total (used in Local Games).
	FloatRect boundingBox;				//The bounding box that contains all of the Player widget objects.

	//Commander Damage objects:
	int startingCmdrDmg;				//The amount of Commander Damage each Counter starts the game with; used to reset the Counters.
	std::vector<int> cmdrDmg;			//The amount of Commander Damage each Player has done to this one, used to set cmdrDmgCounters' text.
	std::vector<Text> cmdrDmgCounters;	//The Text objects to display each of this Player's Commander Damage totals.
	std::vector<Sprite> cmdrAvatars;	//The avatars used to associate each Commander Damage Counter with the Player it represents.
	std::vector<Sprite> cmdrButtons;	//The Sprite to subtract one from the Counter's Damage total (used in Local Games).
	Sprite cmdrCloseButton;				//The button to collapse the Commander Damage Counters back into place, after they've been expanded.
	bool cmdrCountersExpanded;			//Whether or not this Player's Commander Damage Counters are expanded (i.e., they can all be seen).

	//Partner #1 Objects:
	tgui::TextBox::Ptr partnerDisplayNameOne;	//The TextBox to display the name of this Player's 1st Partner (if they have one).
	std::string partnerNameOne;					//The 1st partner's name, used to set partnerDisplayNameOne's text.
	Sprite partnerAvatarOne;					//The 1st partner's avatar, set using the database and "avatarPath".

	//Partner #2 Objects:
	tgui::TextBox::Ptr partnerDisplayNameTwo;	//The TextBox to display the name of this Player's 2nd Partner (if they have one).
	std::string partnerNameTwo;					//The 2nd partner's name, used to set partnerDisplayNameTwo's text.
	Sprite partnerAvatarTwo;					//The 2nd partner's avatar, set using the database and "avatarPath".

	//Team Objects:
	tgui::TextBox::Ptr teamDisplayName;	//The TextBox to display the name of this Player's Team.
	std::string teamName;				//The Player's team name, used to set teamDisplayName's text.
};

#endif // CONSTANTS_H_INCLUDED
