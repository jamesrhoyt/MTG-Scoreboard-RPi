/*
 * gameSetupPanel.h
 */

#include <chrono>
#include <functional>	//std::bind
#include <Graphics.hpp>
#include <Gui.hpp>
#include <Signal.hpp>
#include <TGUI.hpp>
#include <Widget.hpp>
#include <Widgets/Button.hpp>
#include <Widgets/ClickableWidget.hpp>
#include <Widgets/ComboBox.hpp>
#include <Widgets/Label.hpp>
#include <Widgets/ListBox.hpp>
#include <Widgets/Scrollbar.hpp>
#include <Widgets/TextBox.hpp>
#include <thread>		//std::thread
#include "constants.h"

#ifndef GAMESETUPPANEL_H_INCLUDED
#define GAMESETUPPANEL_H_INCLUDED

class gameSetupPanel
{
private:
	sf::RectangleShape background;	//The semi-transparent black background.

	//Mouse Position:
	Vector2f mousePos;

	sf::Text header;	//The "Create Game" text at the top of this Panel.

	tgui::TextBox::Ptr addPlayerTextbox;	//Names can be entered into this TextBox to add them as Players to the SQL database.
	tgui::Button::Ptr addPlayerButton;		//Click this to save the current string in "addPlayerTextbox" as a new Player.

	sf::Text playerListHeader;								//The "Players:" text before the list of Player TextBoxes.
	std::vector<sf::Text> playerNumbers;					//The ordered list of numbers that denotes each of the teams represented.
	std::vector<tgui::ComboBox::Ptr> playerEntryFields;		//The column of players (in a solo game) or team leaders (in a Two- or Three-Headed Giant game).
	std::vector<tgui::ComboBox::Ptr> playerEntryFields2;	//The column of 1st partners in a team; only appears if "Two-Headed" or "Three-Headed" are the selected team size.
	std::vector<tgui::ComboBox::Ptr> playerEntryFields3;	//The column of 2nd partners in a team; only appears if "Three-Headed" is the selected team size.
	void addPlayerEntryField(int index);					//Adds another team entry row to the end of the list of Players/Teams.
	void removePlayerEntryField(int index);					//Removes a team entry row from the list of Players/Teams.
	void addPlayerEntryColumn();							//Enables "playerEntryFields2" or "playerEntryFields3", and resizes the Entry Fields as needed.
	void removePlayerEntryColumn();							//Disables "playerEntryFields2" or "playerEntryFields3", and resizes the Entry Fields as needed.

	sf::Texture gameSetupButtonAddTexture;			//The Texture for the Button that adds Player/Team rows to the Setup Panel.
	sf::Sprite gameSetupButtonAdd;					//Click this to call "addPlayerEntryField".
	sf::Texture gameSetupButtonRemoveTexture;		//The Texture for the Button that removes Player/Team rows from the Setup Panel.
	std::vector<sf::Sprite> gameSetupButtonsRemove;	//Clicking any of these will remove (in a sense) the corresponding Player/Team row.

	sf::Text gameTypeHeader;			//The "Game Type:" header that precedes the "game type" dropdown box.
	tgui::ComboBox::Ptr gameTypeList;	//The dropdown box to select the game type (current lineup: Standard, Commander, Emperor, or Brawl).

	sf::Text teamSizeHeader;			//The "Team Size:" header that precedes the "team size" dropdown box.
	tgui::ComboBox::Ptr teamSizeList;	//The dropdown box to select the team size (current lineup: Solo, Two-Headed, or Three-Headed).

	void addPlayerToTable();	//Add a new name to the "players" SQL table; this gets called when "addPlayerButton" is clicked.

	void slideOutCoroutine(sf::Sprite& tab);	//Slides this Panel (and the "createGameTab" from panelManager) into view over time, using a Thread.
	void slideInCoroutine(sf::Sprite& tab);		//Slides this Panel (and the "createGameTab" from panelManager) out of view over time, using a Thread.

public:
	gameSetupPanel();						//The constructor for the Game Setup Panel.
	std::thread slideOutThread;				//The thread that lets this Panel slide into view.
	std::thread slideInThread;				//The thread that lets this Panel slide out of view.
	tgui::Button::Ptr createGameButton;		//Click this to create a new gamePanel, using the information entered.
	void slideOut(sf::Sprite& tab);			//Calls "slideOutCoroutine" and passes it a reference to panelManager's "createGameTab".
	void getUserInput(sf::Event event);		//Checks this Panel for any user input.
	std::vector<std::string> getNames();	//Pulls the names from the Player Entry Fields to create a gamePanel, first by column, then by row.
	std::vector<int> getIDs();				//Pulls the ID numbers for the Players, first by column, then by row.
	int getGameType();						//Pulls the ID of the selected entry in "gameTypeList".
	int getTeamSize();						//Pulls the ID of the selected entry in "teamSizeList".
	void slideIn(sf::Sprite& tab);			//Calls "slideInCoroutine" and passes it a reference to panelManager's "createGameTab".
	void draw();							//Draws all of this Panel's visual elements.
};

#endif // GAMESETUPPANEL_H_INCLUDED
