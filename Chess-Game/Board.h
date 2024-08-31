#pragma once

#include <array>
#include <memory>
#include "Position.h"
#include "Piece.h"
#include "Move.h"

constexpr int BOARD_SIZE = 8;

class Board {
public:
	Board();

	std::unique_ptr<Piece>& getPieceAt(int col, int row);
	std::unique_ptr<Piece>& getPieceAt(Col col, Row row);
	std::unique_ptr<Piece>& getPieceAt(Position position);

	void setPieceAt(Position& position, std::unique_ptr<Piece> piece);

	const bool isInBound(Position position) const;
	const bool isInBound(int col, int row) const;
	
	const bool hasPieceAt(Position position);

	std::unique_ptr<Piece>& getKing(ChessColor king_color);

	void printBoard();
	
private:
	void init();

	std::array<std::array<std::unique_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> board;
};


