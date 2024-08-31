#include "Board.h"
#include <iostream>

Board::Board() {
	this->init();
}

std::unique_ptr<Piece>& Board::getPieceAt(int col, int row) {
	if (!this->isInBound(Position(col, row))) {
		throw "ERROR::Board: invoked getPieceAt() with an out-of-bounds position";
	}

	// NOTE: 0-indexed
	return this->board[row][col];
}

std::unique_ptr<Piece>& Board::getPieceAt(Col col, Row row) {
	int icol = col, irow = row;
	return this->getPieceAt(icol, irow);
}

std::unique_ptr<Piece>& Board::getPieceAt(Position position) {
	return this->getPieceAt(position.col, position.row);
}

void Board::setPieceAt(Position& position, std::unique_ptr<Piece> piece) {
	this->board[position.row][position.col] = std::move(piece);
}

const bool Board::isInBound(Position position) const {
	bool is_row_in_bound = position.row >= 0 && position.row <= 7;
	bool is_col_in_bound = position.col >= 0 && position.col <= 7;

	return is_row_in_bound && is_col_in_bound;
}

const bool Board::isInBound(int col, int row) const {
	return col >= 0 && col <= 7 && row >= 0 && row <= 7;
}

const bool Board::hasPieceAt(Position position) {
	return this->getPieceAt(position).get();
}

std::unique_ptr<Piece>& Board::getKing(ChessColor king_color) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			if (!this->hasPieceAt(Position(col, row))) {
				continue;
			}

			if (this->board[row][col]->getPieceType() != PieceType::King) {
				continue;
			}

			if (this->board[row][col]->getColor() != king_color) {
				continue;
			}

			return this->board[row][col];
		}
	}
}

void Board::printBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!this->board[i][j]) {
				std::cout << "# ";
			} else {
				Piece* piece = this->board[i][j].get();
				if (piece->getPieceType() == PieceType::Pawn) std::cout << "P ";
				else if (piece->getPieceType() == PieceType::Rook) std::cout << "R ";
				else if (piece->getPieceType() == PieceType::Knight) std::cout << "N ";
				else if (piece->getPieceType() == PieceType::Bishop) std::cout << "B ";
				else if (piece->getPieceType() == PieceType::King) std::cout << "K ";
				else if (piece->getPieceType() == PieceType::Queen) std::cout << "Q ";
			}
		}
		std::cout << std::endl;
	}
}

void Board::init() {
	// pawn initialization
	for (int col = 0; col < 8; col++) {
		this->board[1][col] = std::make_unique<Pawn>(ChessColor::White, Position(col, 1));
		this->board[6][col] = std::make_unique<Pawn>(ChessColor::Black, Position(col, 6));
	}

	// white pieces
	this->board[0][0] = std::make_unique<Rook>(ChessColor::White, Position(0, 0));
	this->board[0][1] = std::make_unique<Knight>(ChessColor::White, Position(1, 0));
	this->board[0][2] = std::make_unique<Bishop>(ChessColor::White, Position(2, 0));
	this->board[0][3] = std::make_unique<Queen>(ChessColor::White, Position(3, 0));
	this->board[0][4] = std::make_unique<King>(ChessColor::White, Position(4, 0));
	this->board[0][5] = std::make_unique<Bishop>(ChessColor::White, Position(5, 0));
	this->board[0][6] = std::make_unique<Knight>(ChessColor::White, Position(6, 0));
	this->board[0][7] = std::make_unique<Rook>(ChessColor::White, Position(7, 0));

	// black pieces
	this->board[7][0] = std::make_unique<Rook>(ChessColor::Black, Position(0, 7));
	this->board[7][1] = std::make_unique<Knight>(ChessColor::Black, Position(1, 7));
	this->board[7][2] = std::make_unique<Bishop>(ChessColor::Black, Position(2, 7));
	this->board[7][3] = std::make_unique<Queen>(ChessColor::Black, Position(3, 7));
	this->board[7][4] = std::make_unique<King>(ChessColor::Black, Position(4, 7));
	this->board[7][5] = std::make_unique<Bishop>(ChessColor::Black, Position(5, 7));
	this->board[7][6] = std::make_unique<Knight>(ChessColor::Black, Position(6, 7));
	this->board[7][7] = std::make_unique<Rook>(ChessColor::Black, Position(7, 7));
}



