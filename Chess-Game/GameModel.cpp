#include "GameModel.h"
#include <iostream>

GameModel::GameModel() {
	this->initBoard();
	this->initPlayers();
	this->movement_manager = GameMovementManager(&(this->board));
}

void GameModel::reset() {
	// TODO: implement
}

void GameModel::update() {
	return;
}

void GameModel::updateTurn() {
	this->status_manager.updateCurrentPlayer();
}

void GameModel::updateBoard() {
	// TODO: implement;
}

const std::vector<Position> GameModel::getValidPositions(Piece* piece) {
	return piece->getValidPositions(*this);
}

Board& GameModel::getBoard() {
	return this->board;
}

const bool GameModel::isMainPlayerTurn() const {
	return this->current_player == this->main_player_color;
}

const bool GameModel::mainPlayerWon() {
	// TODO: implement
	return true;
}

const bool GameModel::mainPlayerLost() {
	// TODO: implement
	return true;
}

const bool GameModel::isStaleMate() {
	// TODO: implement
	return true;
}

void GameModel::handlePromotions() {
	// TODO: implement;
}

void GameModel::initBoard() {
	this->board = Board();
}

void GameModel::initPlayers() {
	this->main_player_color = ChessColor::White;
	this->current_player = ChessColor::White;
}

bool GameModel::isKingChecked(ChessColor king_color) {
	auto& king = this->board.getKing(king_color);
	
	if (!king) {
		std::cerr << "error no king" << std::endl;
	}

	Position king_position = king->getPosition();

	bool straight_line_check = this->searchStraightLineCheck(king_position, king_color);
	bool diagonal_check = this->searchDiagonalCheck(king_position, king_color);
	bool knight_check = this->searchKnightCheck(king_position, king_color);

	return straight_line_check || diagonal_check || knight_check;
}

bool GameModel::searchDiagonalCheck(Position& king_position, ChessColor& king_color) {
	bool north_east_check = this->searchLineCheck(king_position, king_color, 1, 1);
	bool north_west_check = this->searchLineCheck(king_position, king_color, -1, 1);
	bool south_east_check = this->searchLineCheck(king_position, king_color, 1, -1);
	bool south_west_check = this->searchLineCheck(king_position, king_color, -1, -1);

	return north_east_check || north_west_check || south_east_check || south_west_check;
}

bool GameModel::searchStraightLineCheck(Position& king_position, ChessColor& king_color) {
	bool north_check = this->searchLineCheck(king_position, king_color, 0, 1);
	bool south_check = this->searchLineCheck(king_position, king_color, 0, -1);
	bool east_check = this->searchLineCheck(king_position, king_color, 1, 0);
	bool west_check = this->searchLineCheck(king_position, king_color, -1, 0);

	return north_check || south_check || east_check || west_check;
}

bool GameModel::searchLineCheck(Position& king_position, ChessColor& king_color, const int dx, const int dy) {
	for (int row = king_position.row + dy, col = king_position.col + dx, cnt = 0; true; row += dy, col += dx, cnt++) {
		if (!this->board.isInBound(col, row)) {
			return false;
		}

		Position current_position(col, row);

		if (!this->board.hasPieceAt(current_position)) {
			continue;
		}

		auto& found_piece = this->board.getPieceAt(current_position);

		if (found_piece->getColor() == king_color) {
			return false;
		}

		// handle pawns and king
		if (cnt == 0) {
			if (found_piece->getPieceType() == PieceType::King) {
				return true;
			} else if (found_piece->getPieceType() == PieceType::Pawn) {
				if (king_color == ChessColor::White) {
					return (dy == 1 && abs(dx) == 1);
				} else {
					return (dy == -1 && abs(dx) == 1);
				}
			}
		}

		if (found_piece->getPieceType() == PieceType::Queen) {
			return true;
		}

		if (found_piece->getPieceType() == PieceType::Rook) {
			return (abs(dx) + abs(dy) == 1);
		}

		if (found_piece->getPieceType() == PieceType::Bishop) {
			return (abs(dx) + abs(dy) == 2);
		}

		return false;
	}
}

bool GameModel::searchKnightCheck(Position& king_position, ChessColor& king_color) {
	std::array<Position, 8> attacking_knight_positions = {
		Position(king_position.col - 1, king_position.row - 2),
		Position(king_position.col - 2, king_position.row - 1),
		Position(king_position.col - 2, king_position.row + 1),
		Position(king_position.col - 1, king_position.row + 2),
		Position(king_position.col + 1, king_position.row + 2),
		Position(king_position.col + 2, king_position.row + 1),
		Position(king_position.col + 2, king_position.row - 1),
		Position(king_position.col + 1, king_position.row - 2)
	};

	for (auto position : attacking_knight_positions) {
		if (!this->board.isInBound(position)) {
			continue;
		}

		if (!this->board.hasPieceAt(position)) {
			continue;
		}

		auto& found_piece = this->board.getPieceAt(position);

		if (found_piece->getColor() == king_color || found_piece->getPieceType() != PieceType::Knight) {
			continue;
		}

		return true;
	}

	return false;
}
