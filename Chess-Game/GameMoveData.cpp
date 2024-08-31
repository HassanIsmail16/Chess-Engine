#include <iostream>
#include "GameMoveData.h"
#include "GameModel.h"

MoveDataPair& GameMoveData::getLastMove() {
	if (this->wasBlackLast()) {
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

void GameMoveData::pushMove(Move& new_move, GameModel& model) {
	ChessColor moving_player = model.getBoard().getPieceAt(new_move.start_position)->getColor();

	auto taken_over_piece = model.getBoard().getPieceAt(new_move.end_position).release();

	if (moving_player == ChessColor::White) {
		this->white_move_data.emplace_back(new_move, std::unique_ptr<Piece>(taken_over_piece));
	} else {
		this->black_move_data.emplace_back(new_move, std::unique_ptr<Piece>(taken_over_piece));
	}
}

void GameMoveData::popMove() {
	// ALERT: THIS WILL REMOVE THE TAKEN OVER PIECE IF NOT HANDLED CORRECTLY
	if (this->wasBlackLast()) {
		if (this->black_move_data.back().taken_over) {
			std::cout << "ALERT: GameMoveData::PopMove() was invoked and a taken_over piece was destructed" << std::endl;
		}

		this->black_move_data.pop_back();
	} else {
		if (this->white_move_data.back().taken_over) {
			std::cout << "ALERT: GameMoveData::PopMove() was invoked and a taken_over piece was destructed" << std::endl;
		}

		this->white_move_data.pop_back();
	}
}

MoveDataPair& GameMoveData::getLastMove(ChessColor player_color) {
	return (player_color == ChessColor::Black) ? this->black_move_data.back() : this->white_move_data.back();
}

bool GameMoveData::wasBlackLast() {
	return this->black_move_data.size() == this->white_move_data.size();
}
