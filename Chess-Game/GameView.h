#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameModel.h"

class GameView {
public:
	GameView(sf::RenderWindow* window, GameModel* model);

	void renderMainMenuStateView(const float& dt);

	void renderGameStateView(const float& dt);

private: 
	void drawGameStateBoard(const float& dt);

	sf::RenderWindow* window;
	GameModel* model;
};

/*
	TODO: 
	- add an asset manager to manager the sprites of the pieces
	- figure out a way to draw the pieces without adding the piece type thing
*/