#pragma once

#include <array>
#include <memory>
#include "Position.h"
#include "Piece.h"

constexpr int BOARD_SIZE = 8;

class Board {
public:
	Board();

	Piece& getPieceAt(int& col, int& row);
	Piece& getPieceAt(Col& col, Row& row);
	Piece& getPieceAt(Position& position);
	
private:
	void init();

	std::array<std::array<std::unique_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> board;
};


