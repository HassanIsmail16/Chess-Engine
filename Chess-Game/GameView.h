#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameModel.h"
#include <map>

enum class SquareColor {
	Black,
	White,
	Active,
	Highlight,
	Check,
	TakeOver
};

extern std::map<SquareColor, sf::Color> square_color_match;

class GameView {
public:
	GameView(sf::RenderWindow* window, GameModel* model);

	void renderMainMenuStateView(const float& dt);

	void renderGameStateView(const float& dt);

	sf::RenderWindow& getWindow();

private: 
	void drawGameStateBoard(const float& dt);
	void drawSquare(const float& dt, Position& position);
	void drawPiece(const float& dt, Position& position);

	void updateSquareColors();
	void setSquareColor(Position position, SquareColor new_color);
	void resetSquareColor(Position position);
	SquareColor getSquareColor(const Position& position);

	sf::RenderWindow* window;
	GameModel* model;

	std::array<std::array<SquareColor, BOARD_SIZE>, BOARD_SIZE> square_colors;
};

/*
	TODO: 
	- add an asset manager to manager the sprites of the pieces
	- figure out a way to draw the pieces without adding the piece type thing
	- place some focus on comments
*/