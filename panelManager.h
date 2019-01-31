/*
 * panelManager.h
 */

#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include "gamePanel.h"
#include "gamePanelEmperor.h"
#include "gameSetupPanel.h"

#ifndef PANELMANAGER_H_INCLUDED
#define PANELMANAGER_H_INCLUDED

class panelManager
{
private:

	//Mouse Position:
	Vector2f mousePos;

	//Screen Background:
	Texture backgroundTexture;	//The texture used to create the "background" Sprite.
	Sprite background;			//The object to use as this Screen's background.

	//"Waiting" Text:
	std::string waitingString;	//The "Awaiting Network Connection" string; used to set "waitingText"'s text.
	Text waitingText;			//The SFML Text object that gets drawn while there are 0 active games.
	Clock waitingTextClock;		//The Clock that adds ellipsis dots to "waitingString" every half-second; used to simulate text animation.
	int waitingTextEllipsisDots;//How many ellipsis dots are currently appended to the end of "waitingString"; resets after 3.

	//"Create Game" Tab:
	Texture createGameTabClosedTexture;	//The texture applied to "createGameTab" when "setupPanel" is closed (arrows pointing left).
	Texture createGameTabOpenTexture;	//The texture applied to "createGameTab" when "setupPanel" is open (arrows pointing right).
	Sprite createGameTab;				//The Sprite that slides "setupPanel" on- or off-screen when it is clicked.

	//"Game Setup" Panel:
	gameSetupPanel setupPanel;	//The sliding panel on the right side of the screen; used for setting up Local games.
	bool setupPanelOpen;		//Whether or not "setupPanel" is visible.

	int windowWidth;				//The width of this screen in pixels. Should match the width of the window, and the monitor it's displayed on.
	int windowHeight;				//The height of this screen in pixels. Should match the height of the window, and the monitor it's displayed on.
	std::vector<std::unique_ptr<gamePanel>> panels;	//The collection of every active gamePanel in the program, both Local and Remote.
	void checkForPanels();			//Queries the "games" SQL table every frame, and creates a gamePanel for every active Remote Game that doesn't already have one.
	Clock panelUpdateTimer;			//The Clock that keeps time for the "update" method in Remote GamePanels, updating them every second.

public:
	int runPanelManager();		//The main loop for the panelManager; runs as long as the Program is running and isn't on the Startup Screen.
	void addPanel(int gameID);	//Used to create new gamePanels, both Local (through the Game Setup Panel) and Remote (through the "checkForPanels" method).
};

#endif // PANELMANAGER_H_INCLUDED
