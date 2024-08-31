#include "Piece.h"
#include "GameModel.h"
#include <iostream>

// <<<<<<<<<< Piece >>>>>>>>>>

Piece::Piece(ChessColor color, Position position) {
	this->color = color;
	this->position = position;
}

Piece::Piece(const Piece& other) {
	this->color = other.color;
	this->position = other.position;
	this->type = other.type;
}

Piece& Piece::operator=(const Piece& other) {
	if (this != &other) {
		this->color = other.color;
		this->position = other.position;
		this->type = other.type;
	}
	return *this;
}

ChessColor Piece::getColor() {
	return this->color;
}

PieceType& Piece::getPieceType() {
	return this->type;
}

void Piece::moveTo(Position& new_position) {
	this->position = new_position;
	this->is_moved = true;
}

Position Piece::getPosition() const {
	return this->position;
}

void Piece::printType() {
	if (this->type == PieceType::Pawn) {
		std::cout << "Pawn" << std::endl;
	} else if (this->type == PieceType::Rook) {
		std::cout << "Rook" << std::endl;
	} else if (this->type == PieceType::Bishop) {
		std::cout << "Bishop" << std::endl;
	} else if (this->type == PieceType::Knight) {
		std::cout << "Knight" << std::endl;
	} else if (this->type == PieceType::King) {
		std::cout << "King" << std::endl;
	} else if (this->type == PieceType::Queen) {
		std::cout << "Queen" << std::endl;
	} else {
		std::cout << "unknown" << std::endl;
	}
}

bool Piece::isMoved() {
	return this->is_moved;
}

void Piece::setIsMoved(bool is_moved) {
	this->is_moved = is_moved;
}

std::vector<Position> Piece::getValidStraightLinePositions(GameModel& model, Position current_position, ChessColor color) {
	std::vector<Position> north(this->getValidLinePositions(model, current_position, color, 0, 1));
	std::vector<Position> south(this->getValidLinePositions(model, current_position, color, 0, -1));
	std::vector<Position> east(this->getValidLinePositions(model, current_position, color, 1, 0));
	std::vector<Position> west(this->getValidLinePositions(model, current_position, color, -1, 0));

	std::vector<Position> valid_positions;

	valid_positions.insert(valid_positions.end(), north.begin(), north.end());
	valid_positions.insert(valid_positions.end(), south.begin(), south.end());
	valid_positions.insert(valid_positions.end(), east.begin(), east.end());
	valid_positions.insert(valid_positions.end(), west.begin(), west.end());

	return valid_positions;
}

std::vector<Position> Piece::getValidDiagonalPositions(GameModel& model, Position current_position, ChessColor color) {
	std::vector<Position> north_east(this->getValidLinePositions(model, current_position, color, 1, 1));
	std::vector<Position> north_west(this->getValidLinePositions(model, current_position, color, 1, -1));
	std::vector<Position> south_east(this->getValidLinePositions(model, current_position, color, -1, 1));
	std::vector<Position> south_west(this->getValidLinePositions(model, current_position, color, -1, -1));

	std::vector<Position> valid_positions;

	valid_positions.insert(valid_positions.end(), north_east.begin(), north_east.end());
	valid_positions.insert(valid_positions.end(), north_west.begin(), north_west.end());
	valid_positions.insert(valid_positions.end(), south_east.begin(), south_east.end());
	valid_positions.insert(valid_positions.end(), south_west.begin(), south_west.end());

	return valid_positions;
}

std::vector<Position> Piece::getValidLinePositions(GameModel& model, Position current_position, ChessColor color, const int& dx, const int& dy) {
	std::vector<Position> valid_positions;

	for (int row = current_position.row + dy, col = current_position.col + dx; true; row += dy, col += dx) {
		if (!model.getBoard().isInBound(Position(col, row))) {
			return valid_positions;
		}

		if (model.getBoard().hasPieceAt(Position(col, row))) {
			if (model.getBoard().getPieceAt(Position(col, row))->getColor() != color
				&& model.getBoard().getPieceAt(Position(col, row))->getPieceType() != PieceType::King) {
				valid_positions.emplace_back(Position(col, row));
			}

			return valid_positions;
		}

		if (model.movement_manager.willKingBeChecked(color, Move(current_position, Position(col, row)), model)) {
			return valid_positions;
		}

		valid_positions.emplace_back(Position(col, row));
	}

	return valid_positions;
}

// <<<<<<<<<< Pawn >>>>>>>>>>

Pawn::Pawn(ChessColor color, Position position) : Piece(color, position) {
	this->type = PieceType::Pawn;
}

std::vector<Position> Pawn::getValidPositions(GameModel& model) {
	std::vector<Position> valid_positions;

	int step = (this->color == ChessColor::White) ? 1 : -1;

	Position single_step(this->position.col, this->position.row + step);
	Position double_step(this->position.col, this->position.row + 2 * step);
	Position diagonal_takeover_left(this->position.col - 1, this->position.row + step);
	Position diagonal_takeover_right(this->position.col + 1, this->position.row + step);

	auto isValidMove = [&](Position& target_position) -> bool {
		if (!model.getBoard().isInBound(target_position)) return false;
		if (model.movement_manager.willKingBeChecked(color, Move(this->getPosition(), target_position), model)) {
			return false;
		}
		return true;
		};

	auto handleDiagonalMove = [&](Position& diagonal_position) {
		if (isValidMove(diagonal_position) && model.getBoard().hasPieceAt(diagonal_position) 
			&& model.getBoard().getPieceAt(diagonal_position)->getColor() != this->getColor()
			&& model.getBoard().getPieceAt(diagonal_position)->getPieceType() != PieceType::King) {
			valid_positions.emplace_back(diagonal_position);
		}
		};


	handleDiagonalMove(diagonal_takeover_left);
	handleDiagonalMove(diagonal_takeover_right);

	if (isValidMove(single_step) && !model.getBoard().hasPieceAt(single_step)) {
		valid_positions.emplace_back(single_step);

		if (!this->isMoved() && isValidMove(double_step) && !model.getBoard().hasPieceAt(double_step)) {
			valid_positions.emplace_back(double_step);
		}
	}

	return valid_positions;
}

// <<<<<<<<<< Rook >>>>>>>>>>

Rook::Rook(ChessColor color, Position position) : Piece(color, position) {
	this->type = PieceType::Rook;
}

std::vector<Position> Rook::getValidPositions(GameModel& model) {
	bool was_moved_initially = this->is_moved;

	std::vector<Position> valid_positions = this->getValidStraightLinePositions(model, this->getPosition(), this->getColor());

	this->is_moved = was_moved_initially;

	return valid_positions;
}

// <<<<<<<<<< Bishop >>>>>>>>>>

Bishop::Bishop(ChessColor color, Position position) : Piece(color, position) {
	this->type = PieceType::Bishop;
}

std::vector<Position> Bishop::getValidPositions(GameModel& model) {
	std::vector<Position> valid_positions = this->getValidDiagonalPositions(model, this->getPosition(), this->getColor());

	return valid_positions;
}

// <<<<<<<<<< Knight >>>>>>>>>>

Knight::Knight(ChessColor color, Position position) : Piece(color, position) {
	this->type = PieceType::Knight;
}

std::vector<Position> Knight::getValidPositions(GameModel& model) {
	std::vector<Position> valid_positions;

	Position current_position = this->getPosition();

	// compute positions
	std::array<Position, 8> possible_positions {
		Position(current_position.col - 1, current_position.row + 2),
		Position(current_position.col - 2, current_position.row + 1),
		Position(current_position.col + 1, current_position.row + 2),
		Position(current_position.col + 2, current_position.row + 1),
		Position(current_position.col - 2, current_position.row - 1),
		Position(current_position.col - 1, current_position.row - 2),
		Position(current_position.col + 2, current_position.row - 1),
		Position(current_position.col + 1, current_position.row - 2)
	};

	// validate positions
	auto isValidMove = [&](Position& position) -> bool {
		return model.getBoard().isInBound(position) && !model.movement_manager.willKingBeChecked(this->getColor(), Move(this->getPosition(), position), model);
		};

	for (Position& position : possible_positions) {
		if (isValidMove(position)) {
			if (model.getBoard().hasPieceAt(position) && 
				(model.getBoard().getPieceAt(position)->getColor() == this->getColor()
					|| model.getBoard().getPieceAt(position)->getPieceType() == PieceType::King)) {
				continue;
			}

			valid_positions.emplace_back(position);
		}
	}

	return valid_positions;
}

// <<<<<<<<<< Queen >>>>>>>>>>

Queen::Queen(ChessColor color, Position position) : Piece(color, position) {
	this->type = PieceType::Queen;
}

std::vector<Position> Queen::getValidPositions(GameModel& model) {
	std::vector<Position> rook_valid_positions = this->getValidStraightLinePositions(model, this->getPosition(), this->getColor());

	std::vector<Position> bishop_valid_positions = this->getValidDiagonalPositions(model, this->getPosition(), this->getColor());

	// TODO: create a merge function
	std::vector<Position> valid_positions(rook_valid_positions);
	valid_positions.insert(valid_positions.end(), std::make_move_iterator(bishop_valid_positions.begin()), std::make_move_iterator(bishop_valid_positions.end()));

	return valid_positions;
}

// <<<<<<<<<< King >>>>>>>>>>

King::King(ChessColor color, Position position) : Piece(color, position) {
	this->type = PieceType::King;
}

std::vector<Position> King::getValidPositions(GameModel& model) {
	std::vector<Position> valid_positions;

	Position current_position = this->getPosition();

	std::array<Position, 8> potential_positions{
		Position(current_position.col, current_position.row + 1),
		Position(current_position.col, current_position.row - 1),
		Position(current_position.col + 1, current_position.row + 1),
		Position(current_position.col + 1, current_position.row),
		Position(current_position.col + 1, current_position.row - 1),
		Position(current_position.col - 1, current_position.row + 1),
		Position(current_position.col - 1, current_position.row),
		Position(current_position.col - 1, current_position.row - 1),
	};

	for (auto position : potential_positions) {
		if (!model.getBoard().isInBound(position)) {
			continue;
		}

		if (model.getBoard().hasPieceAt(position) && model.getBoard().getPieceAt(position)->getColor() == this->getColor()) {
			continue;
		}

		if (model.movement_manager.willKingBeChecked(this->getColor(), Move(this->getPosition(), position), model)) {
			continue;
		}

		valid_positions.emplace_back(position);
 	}
	
	this->handleCastling(model, valid_positions);

	return valid_positions;
}

bool King::isChecked() {
	return this->is_checked;
}

void King::handleCastling(GameModel& model, std::vector<Position>& valid_positions) {
	if (this->canCastleQueenSide(model)) {
		valid_positions.emplace_back(Col::QueenSideCasle, this->getPosition().row);
	}
	if (this->canCastleKingSide(model)) {
		valid_positions.emplace_back(Col::KingSideCastle, this->getPosition().row);
	}

	return;
}

bool King::canCastleQueenSide(GameModel& model) {
	return this->canCastle(model, false);
}

bool King::canCastleKingSide(GameModel& model) {
	return this->canCastle(model, true);
}

bool King::canCastle(GameModel& model, const bool is_king_side) {
	Position king_position(this->getPosition());

	if (model.isKingChecked(this->getColor())) {
		return false;
	}

	if (this->isMoved()) {
		return false;
	}

	if (!this->rookCanCastle(model, this->getColor(), is_king_side)) {
		return false;
	}

	if (!this->isCastlingPathClearAndSafe(model, this->getColor(), is_king_side)) {
		return false;
	}

	return true;
}

bool King::rookCanCastle(GameModel& model, ChessColor king_color, const bool is_king_side) {
	Position king_position(model.getBoard().getKing(king_color)->getPosition());

	int target_rook_col = (is_king_side) ? 7 : 0;

	Position target_rook_position(target_rook_col, king_position.row);

	if (!model.getBoard().hasPieceAt(target_rook_position)) {
		return false;
	}

	if (model.getBoard().getPieceAt(target_rook_position)->getPieceType() != PieceType::Rook) {
		return false;
	}

	Rook* rook = dynamic_cast<Rook*>(model.getBoard().getPieceAt(target_rook_position).get());
 
	if (rook->getColor() != king_color || rook->isMoved()) {
		return false;
	}

	return true;
}

bool King::isCastlingPathClearAndSafe(GameModel& model, ChessColor king_color, const bool is_king_side) {
	Position king_position(model.getBoard().getKing(king_color)->getPosition());

	int movement_direction = (is_king_side) ? 1 : -1;

	int target_rook_col = (is_king_side) ? 7 : 0;

	Position king_destination(target_rook_col - movement_direction, king_position.row);

	for (int col = king_position.col + movement_direction; col != target_rook_col; col += movement_direction) {
		Position current_position(col, king_position.row);

		// check if path betweeen rook and king is clear
		if (model.getBoard().hasPieceAt(current_position)) {
			return false;
		}

		// check if path from king_position to king_destination is safe
		if (col <= king_destination.col) {
			if (model.movement_manager.willKingBeChecked(king_color, Move(king_position, Position(col, king_position.row)), model)) {
				return false;
			}
		}
	}

	return true;
}


