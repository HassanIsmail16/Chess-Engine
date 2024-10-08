#pragma once

#include <SFML/Window.hpp>
#include "TextureManager.h"
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

	void updateView();

	float getSquareSize() const;

private: 
	void drawGameStateBoard(const float& dt);
	void drawSquare(const float& dt, Position& position);
	void drawPiece(const float& dt, Position& position);
	void drawPlan(const float& dt);
	void drawArrow(const float& dt, ArrowData& arrow_data);
	void drawCircle(const float& dt, ArrowData& circle_data);

	bool isCircle(ArrowData& arrow_data);

	void updateSquareColors();
	void setSquareColor(Position position, SquareColor new_color);
	void resetSquareColor(Position position);
	SquareColor getSquareColor(const Position& position);

	sf::RenderWindow* window;
	GameModel* model;

	std::array<std::array<SquareColor, BOARD_SIZE>, BOARD_SIZE> square_colors;

	TextureManager texture_manager;

	float square_size = 50.f;
};

// TODO: add an asset manager to manager the sprites of the pieces