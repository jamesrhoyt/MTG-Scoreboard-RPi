/*
 * constants.cpp
 *
 * Extern objects from "constants.h" are declared here as non-extern objects,
 * to make them publicly accessible to the entire project.
 *
 */

#include "constants.h"

 //SQL Objects:
sql::mysql::MySQL_Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::PreparedStatement *prep_stmt;
sql::ResultSet *res;

//Program Window:
sf::RenderWindow* window;

//GUI (used to draw TGUI Text Boxes):
tgui::Gui* gui;

//Fonts:
sf::Font* font_mtg;
sf::Font* font_dejavu;

//Player Widget Textures:
sf::Texture* buttonCommanderMinusOneTexture;
sf::Texture* buttonLifePlusOneTexture;
sf::Texture* buttonLifePlusFiveTexture;
sf::Texture* buttonLifeMinusFiveTexture;
sf::Texture* buttonLifeMinusTenTexture;
sf::Texture* buttonPoisonPlusOneTexture;
sf::Texture* buttonPoisonMinusOneTexture;
sf::Texture* buttonCommanderCloseTexture;

//Other Button Textures:
sf::Texture* addPlayerPanelButtonTexture;

//Backgrounds:
sf::Texture* panelTexture;

//Player Avatar Textures:
std::vector<Texture*> avatarTextures;
