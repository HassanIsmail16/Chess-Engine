#include "Board.h"

Board::Board() {
	this->init();
}

Piece& Board::getPieceAt(int& col, int& row) {
	// NOTE: 0-indexed
	return *(this->board[row][col]);
}

Piece& Board::getPieceAt(Col& col, Row& row) {
	return *(this->board[static_cast<int>(row) - 1][static_cast<int>(col) - 1]);
}

Piece& Board::getPieceAt(Position& position) {
	return this->getPieceAt(position.col, position.row);
}

void Board::init() {
	// pawn initialization
	for (int col = 0; col < 8; col++) {
		this->board[1][col] = std::make_unique<Pawn>(ChessColor::White);
		this->board[6][col] = std::make_unique<Pawn>(ChessColor::Black);
	}

	// white pieces
	this->board[0][0] = std::make_unique<Rook>(ChessColor::White);
	this->board[0][1] = std::make_unique<Knight>(ChessColor::White);
	this->board[0][2] = std::make_unique<Bishop>(ChessColor::White);
	this->board[0][3] = std::make_unique<Queen>(ChessColor::White);
	this->board[0][4] = std::make_unique<King>(ChessColor::White);
	this->board[0][5] = std::make_unique<Bishop>(ChessColor::White);
	this->board[0][6] = std::make_unique<Knight>(ChessColor::White);
	this->board[0][7] = std::make_unique<Rook>(ChessColor::White);

	// black pieces
	this->board[7][0] = std::make_unique<Rook>(ChessColor::Black);
	this->board[7][1] = std::make_unique<Knight>(ChessColor::Black);
	this->board[7][2] = std::make_unique<Bishop>(ChessColor::Black);
	this->board[7][3] = std::make_unique<Queen>(ChessColor::Black);
	this->board[7][4] = std::make_unique<King>(ChessColor::Black);
	this->board[7][5] = std::make_unique<Bishop>(ChessColor::Black);
	this->board[7][6] = std::make_unique<Knight>(ChessColor::Black);
	this->board[7][7] = std::make_unique<Rook>(ChessColor::Black);
}



