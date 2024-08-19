#include "Piece.h"
#include "GameModel.h"

// <<<<<<<<<< Piece >>>>>>>>>>

Piece::Piece(ChessColor color) {
	this->color = color;
}

ChessColor Piece::getColor() {
	return this->color;
}

// <<<<<<<<<< Pawn >>>>>>>>>>

Pawn::Pawn(ChessColor color) : Piece(color) {}

std::vector<Position>& Pawn::getValidPositions(const GameModel& model) {
	std::vector<Position> valid_positions;
	return valid_positions;
}

bool Pawn::isMoved() {
	return this->is_moved;
}

// <<<<<<<<<< Rook >>>>>>>>>>

Rook::Rook(ChessColor color) : Piece(color) {}

std::vector<Position>& Rook::getValidPositions(const GameModel& model) {
	std::vector<Position> valid_positions;
	return valid_positions;
}

bool Rook::isMoved() {
	return this->is_moved;
}

// <<<<<<<<<< Bishop >>>>>>>>>>

Bishop::Bishop(ChessColor color) : Piece(color) {}

std::vector<Position>& Bishop::getValidPositions(const GameModel& model) {
	std::vector<Position> valid_positions;
	return valid_positions;
}

// <<<<<<<<<< Knight >>>>>>>>>>

Knight::Knight(ChessColor color) : Piece(color) {}

std::vector<Position>& Knight::getValidPositions(const GameModel& model) {
	std::vector<Position> valid_positions;
	return valid_positions;
}

// <<<<<<<<<< Queen >>>>>>>>>>

Queen::Queen(ChessColor color) : Piece(color) {}

std::vector<Position>& Queen::getValidPositions(const GameModel& model) {
	std::vector<Position> valid_positions;
	return valid_positions;
}

// <<<<<<<<<< King >>>>>>>>>>

King::King(ChessColor color) : Piece(color) {}

std::vector<Position>& King::getValidPositions(const GameModel& model) {
	std::vector<Position> valid_positions;
	return valid_positions;
}

bool King::isChecked() {
	return this->is_checked;
}

bool King::isMoved() {
	return this->is_moved;
}


