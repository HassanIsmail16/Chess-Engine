#include <iostream>
#include "GameMovementManager.h"
#include "GameModel.h"

GameMovementManager::GameMovementManager() {
	this->selected_piece = nullptr;
	this->move_data = GameMoveData();
	this->board = nullptr;
}

GameMovementManager::GameMovementManager(Board* board) {
	this->selected_piece = nullptr;
	this->move_data = GameMoveData();
	this->board = board;
}

void GameMovementManager::selectPiece(std::unique_ptr<Piece>& piece) {
	this->selected_piece = piece.get();
}

void GameMovementManager::selectPiece(Position position) {
	this->selected_piece = this->board->getPieceAt(position).get();
}

void GameMovementManager::selectPiece(int col, int row) {
	this->selectPiece(Position(col, row));
}

void GameMovementManager::unselectPiece() {
	this->selected_piece = nullptr;
}

bool GameMovementManager::hasSelectedPiece() {
	return this->selected_piece;
}

Piece* GameMovementManager::getSelectedPiece() {
	return this->selected_piece;
}

void GameMovementManager::makeMove(Move move, GameModel& model) {
	if (!this->board->hasPieceAt(move.start_position)) {
		throw "ERROR::GameModel: attempted to invoke makeMove() with an empty start_position";
	}

	this->move_data.pushMove(move, model);

	auto& moving_piece = this->board->getPieceAt(move.start_position);

	moving_piece->moveTo(move.end_position);

	this->board->setPieceAt(move.end_position, std::move(moving_piece));

	// TODO: implement takeover data addition
}

void GameMovementManager::undoLastMove() {
	auto& last_move_data = this->move_data.getLastMove();

	if (!this->board->hasPieceAt(last_move_data.move.end_position)) {
		throw "ERROR::GameModel: attempted to invoke undoMove() with an empty end_position";
	}

	auto& moving_piece = this->board->getPieceAt(last_move_data.move.end_position);

	moving_piece->moveTo(last_move_data.move.start_position);

	this->board->setPieceAt(last_move_data.move.start_position, std::move(moving_piece));

	auto& taken_over = last_move_data.taken_over;

	if (taken_over) {
		taken_over->moveTo(last_move_data.move.end_position);
		this->board->setPieceAt(last_move_data.move.end_position, std::move(taken_over));
	}

	this->move_data.popMove();
}

bool GameMovementManager::willKingBeChecked(ChessColor king_color, Move move, GameModel& model) {
 	auto& king = this->board->getKing(king_color);

	if (king->getPosition() == move.end_position) {
		return false;
	}

	auto& moving_piece = this->board->getPieceAt(move.start_position);

	bool initial_move_status = this->board->getPieceAt(move.start_position)->isMoved();

	this->makeMove(move, model);

	bool is_king_checked = model.isKingChecked(king_color);

	this->undoLastMove();

	this->board->getPieceAt(move.start_position)->setIsMoved(initial_move_status);

	return is_king_checked;
}

GameMoveData& GameMovementManager::getMoveData() {
	return move_data;
}
