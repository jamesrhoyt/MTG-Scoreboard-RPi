/*
 * startupScreen.h
 */

#include <cmath>
#include <Graphics.hpp>
#include "panelManager.h"

#ifndef STARTUPSCREEN_H_INCLUDED
#define STARTUPSCREEN_H_INCLUDED

class startupScreen
{
private:

	//Visual Components
	//-----------------

	//Screen Background:
	Texture backgroundTexture;
	Sprite background;

	//Title Screen Logo:
	Texture titleLogoTexture;
	Sprite titleLogo;

	//"Press Start"/"Click To Start" Text:
	Text startText;

	//"Click To Start" Alpha Clock:
	Clock startTextClock;

	//"Click To Start" Alpha Value:
	int startTextAlpha;

	//Current Version Number:
	Text versionNumberText;

	//GamePanel Manager:
	panelManager pManager;

public:
	int runStartup();
};

#endif // STARTUPSCREEN_H_INCLUDED
