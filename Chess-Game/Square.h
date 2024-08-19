#pragma once

#include "Shape.h"
#include "Piece.h"
#include "Position.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

enum class SquareColor {
	Black,
	White, 
	Highlight,
	Active,
	Alert
};

extern std::map<SquareColor, sf::Color> shape_color_match;

class Square : public Shape {
public:
	Square(std::shared_ptr<GameData> game_data, Position position, float size);
	
	sf::RectangleShape& getShape();

	void initPiece();
	void setPiece(std::shared_ptr<Piece> new_piece);
	void removePiece();
	Piece& getPiece();

	const SquareColor getDefaultColor() const;
	
	void setColor(SquareColor new_color);
	void setSize(float new_size);
	
	void activate();
	void highlight();

	const bool isEmpty() const;

private:
	Position position;
	std::shared_ptr<Piece> current_piece;
	float size;
	SquareColor current_color;
};

