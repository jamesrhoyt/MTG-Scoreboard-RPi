/*
 * gamePanel.h
 */

#include <cmath>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <Graphics.hpp>
#include <Gui.hpp>
#include "constants.h"

#ifndef GAMEPANEL_H_INCLUDED
#define GAMEPANEL_H_INCLUDED

class gamePanel
{
protected:
	//Game-related variables:
	int gameTypeID;
	int teamSizeID;
	//Mouse Position:
	Vector2f mousePos;

	Vector2i targetSize;				//The target size of this Panel.
	std::thread panelResizeThread;		//The thread to resize the Panel over time.
	Vector2i targetPosition;			//The target position of this Panel.
	std::thread panelRepositionThread;	//The thread to reposition the Panel over time.

	sf::Texture backgroundTexture;		//The texture for the Panel's background.
	sf::Sprite background;				//The background object itself, which fills the entire Panel.

	sf::Text gameDisplayName;			//The "name" given to this Game Panel (currently only used in Remote Games).
	std::vector<player> playerList;		//The list of all Players in this game, organized by team.
	sf::Sprite addPlayerButton;			//The Button to add another Player to this game (Local Games only).

	bool currentPlayerUsed;				//A flag that gets used when adding Players to Remote Games.
	sf::Image resetButton;				//A Button to reset the game to its initial state (Local Games only).

	tgui::TextBox::Ptr cmdrHoverText;	//The "hover text" for Commander Damage Avatars, to display the name of the Player
										//associated with that Avatar (helpful in the case of duplicate Avatars).
	bool hoverTextActive;				//Whether or not a Commander Damage Avatar is currently being hovered over.

	//Player-related Threads:
	std::thread playerFadeThread;		//The thread to fade a Player's objects out (if they've lost this game) or back in (if they were marked as losing accidentally).
	std::thread playerExpandThread;		//The thread to expand/collapse a Player's Commander Damage Counters, so they can all be viewed/interacted with.

	//Internal Methods:
	virtual void setPosition(int x, int y);									//Set this Panel's target position, to reposition it over time.
	virtual void resize(int width, int height);								//Set this Panel's target size, to resize it over time.
	void PlayerFadeOutCoroutine(int playerID);						//Fade a given Player's objects out to semi-transparency over time.
	void PlayerFadeInCoroutine(int playerID);						//Fade a given Player's objects in to full opacity over time.
	void PlayerExpandCoroutine(int playerID);						//Expand a given Player's Commander Damage Counters over time.
	void PlayerCollapseCoroutine(int playerID);						//Collapse a given Player's Commander Damage Counters over time.
	void UpdateLife(int playerID, int change);						//Update a given Player's Life Counter.
	void UpdatePoison(int playerID, int change);					//Update a given Player's Poison Counter.
	void UpdateCommander(int playerID, int enemyID, int change);	//Update one of a given Player's Commander Damage Counters.
	virtual void AddPlayer();												//Add a new Player to this game (Local Games).
	void AddPlayer(std::string name, int life);						//Add a new Player to this game (Remote Games).
	int getHighestCmdrDmg(int player);								//Determine which of a given Player's Commander Damage Counters to display.
	void createBoundingBox(int player);								//Set the "boundingBox" FloatRect for a given Player.
	void shiftPlayers();											//Rearrange the Player widgets until none of them overlap.
	void movePlayer(int player, float xDiff, float yDiff);			//Move all of the UI objects for a single Player; called by "shiftPlayers".

public:
    gamePanel();                                                                                    //The Default constructor (shouldn't ever be used).
	gamePanel(std::vector<std::string> names, std::vector<int> ids, int gameType, int teamSize);	//The Local Game Panel constructor.
	gamePanel(int gameID, std::string gameName);													//The Remote Game Panel constructor.
	int remoteGameID;																				//The ID number for this Game Panel (Remote Games have unique IDs, Local Games' are always -1).
	int numPlayers;																					//The number of players/teams in this game.

	//External Methods:
	virtual void setTargetSize(int width, int height);	//Set this Panel's target size, to resize it over time.
	virtual void setTargetPosition(int x, int y);		//Set this Panel's target position, to reposition it over time.
	void getUserInput(sf::Event event);			//Get any user input for this Panel (typically only used for Local Games).
	virtual void draw();								//Draw the visual objects for the game and all of its Players.
	virtual void update();								//Update the Panel's counter values (Remote Games only).
	virtual void disableGUI();							//Hide the Panel's TGUI objects (used to prevent accidental clicks while the Setup Panel is open).
	virtual void enableGUI();							//Show the Panel's TGUI objects again (after the Setup Panel has been closed).
};

#endif // GAMEPANEL_H_INCLUDED
