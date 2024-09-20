#include <iostream>
#include "GameMoveData.h"
#include "GameModel.h"

MoveDataPair& GameMoveData::getLastMove() {
	if (this->was_black_last) {
		return this->getLastBlackMove();
	} else {
		return this->getLastWhiteMove();
	}
}

MoveDataPair& GameMoveData::getLastWhiteMove() {
	return this->getLastMove(ChessColor::White);
}

MoveDataPair& GameMoveData::getLastBlackMove() {
	return this->getLastMove(ChessColor::Black);
}

bool GameMoveData::noMoves() {
	return this->noWhiteMoves() && this->noBlackMoves();
}

bool GameMoveData::noWhiteMoves() {
	return this->white_move_data.empty();
}

bool GameMoveData::noBlackMoves() {
	return this->black_move_data.empty();
}

void GameMoveData::pushMove(Move& new_move, GameModel& model) {
	std::cout << (was_black_last ? "TRUE" : "FALSE") << std::endl;
	ChessColor moving_player = model.getBoard().getPieceAt(new_move.start_position)->getColor();

	auto taken_over_piece = model.getBoard().getPieceAt(new_move.end_position).release();

	if (moving_player == ChessColor::White) {
		this->white_move_data.emplace_back(new_move, std::unique_ptr<Piece>(taken_over_piece));
		this->was_black_last = false;
	} else {
		this->black_move_data.emplace_back(new_move, std::unique_ptr<Piece>(taken_over_piece));
		this->was_black_last = true;
	}
}

void GameMoveData::popMove() {
	std::cout << (was_black_last ? "TRUE" : "FALSE") << std::endl;
	// ALERT: THIS WILL REMOVE THE TAKEN OVER PIECE IF NOT HANDLED CORRECTLY
	// TODO: clean this up
	if (this->was_black_last) {
		if (this->black_move_data.back().taken_over) {
			std::cout << "ALERT: GameMoveData::PopMove() was invoked and a taken_over piece was destructed" << std::endl;
		}

		Position last_move_end_position = this->getLastBlackMove().move.end_position;

		if (last_move_end_position.col == Col::KingSideCastle || last_move_end_position.col == Col::QueenSideCasle) {
			this->black_move_data.pop_back();
		}

		this->black_move_data.pop_back();

		this->was_black_last = false;
	} else {
		if (this->white_move_data.back().taken_over) {
			std::cout << "ALERT: GameMoveData::PopMove() was invoked and a taken_over piece was destructed" << std::endl;
		}

		Position last_move_end_position = this->getLastWhiteMove().move.end_position;

		if (last_move_end_position.col == Col::KingSideCastle || last_move_end_position.col == Col::QueenSideCasle) {
			this->white_move_data.pop_back();
		}

		this->white_move_data.pop_back();

		this->was_black_last = true;
	}
}

MoveDataPair& GameMoveData::getLastMove(ChessColor player_color) {
	return (player_color == ChessColor::Black) ? this->black_move_data.back() : this->white_move_data.back();
}
