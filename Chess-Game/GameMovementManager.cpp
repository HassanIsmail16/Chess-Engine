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

	model.status_manager.registerBoardState(model.getBoard());
}

void GameMovementManager::undoLastMove(GameModel& model) {
	model.status_manager.undoBoardStateRegistery(model.getBoard());

	auto& last_move_data = this->move_data.getLastMove();

	if (!this->board->hasPieceAt(last_move_data.move.end_position)) {
		throw "ERROR::GameModel: attempted to invoke undoMove() with an empty end_position";
	}

	auto& moving_piece = this->board->getPieceAt(last_move_data.move.end_position);

	moving_piece->moveTo(last_move_data.move.start_position);

	this->board->setPieceAt(last_move_data.move.start_position, std::move(moving_piece));

	auto& taken_over = last_move_data.taken_over;

	if (taken_over) {
		//taken_over->moveTo(last_move_data.move.end_position);
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

	bool is_king_checked = model.status_manager.isKingChecked(model, king_color);

	this->undoLastMove(model);
	
	this->board->getPieceAt(move.start_position)->setIsMoved(initial_move_status);

	return is_king_checked;
}

GameMoveData& GameMovementManager::getMoveData() {
	return move_data;
}

bool GameMovementManager::isPromotionPending() {
	if (this->move_data.noMoves()) {
		return false;
	}

	auto& last_moving_piece(this->getLastMovingPiece());

	if (last_moving_piece->getPieceType() != PieceType::Pawn) {
		return false;
	} // last piece not a pawn

	Position last_moving_piece_position(last_moving_piece->getPosition());

	if (last_moving_piece_position.row != 0 && last_moving_piece_position.row != 7) {
		return false;
	} // pawn was not on edges

	return true;
}

void GameMovementManager::handlePawnPromotion(PieceType new_piece_type) {
	if (!this->isPromotionPending()) {
		return;
	} // no promotions are pending

	auto& promotion_pawn(this->getLastMovingPiece());

	Position promotion_position(promotion_pawn->getPosition());

	ChessColor promotion_color(promotion_pawn->getColor());

	switch (new_piece_type) {
		case (PieceType::Rook): 
		this->board->setPieceAt(promotion_position, std::make_unique<Rook>(promotion_color, promotion_position));
		break;

		case (PieceType::Knight):
		this->board->setPieceAt(promotion_position, std::make_unique<Knight>(promotion_color, promotion_position));
		break;

		case (PieceType::Queen):
		this->board->setPieceAt(promotion_position, std::make_unique<Queen>(promotion_color, promotion_position));
		break;

		case (PieceType::Bishop):
		this->board->setPieceAt(promotion_position, std::make_unique<Bishop>(promotion_color, promotion_position));
		break;

		default:
		break;
	}
}

// WARNING: will crash if no moves were done
// TODO: always returns a fucking white piece for some reason
std::unique_ptr<Piece>& GameMovementManager::getLastMovingPiece() {
	Move& last_move(this->move_data.getLastMove().move);

	auto& last_moving_piece(this->board->getPieceAt(last_move.end_position));

	return last_moving_piece;
}
