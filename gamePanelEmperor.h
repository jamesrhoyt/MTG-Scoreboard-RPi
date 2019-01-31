/*
 * gamePanelEmperor.h
 */

#include "gamePanel.h"

#pragma once

#ifndef GAMEPANELEMPEROR_H_INCLUDED
#define GAMEPANELEMPEROR_H_INCLUDED

class gamePanelEmperor : public gamePanel
{
private:
	virtual void setPosition(int x, int y) override;
	virtual void resize(int width, int height) override;
	virtual void AddPlayer() override;
	void AddPlayer(int i, int id, int life, int poison);
public:
	gamePanelEmperor(std::vector<std::string> names, std::vector<int> ids, int teamSize);	//The Local Game Panel Constructor
	gamePanelEmperor(int gameID, std::string gameName);	//The Remote Game Panel Constructor
	virtual void setTargetSize(int width, int height) override;
	virtual void setTargetPosition(int x, int y) override;
	virtual void draw() override;
	virtual void update() override;
	virtual void disableGUI() override;
	virtual void enableGUI() override;
	~gamePanelEmperor();
};

#endif // GAMEPANELEMPEROR_H_INCLUDED
