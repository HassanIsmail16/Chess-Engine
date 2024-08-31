#pragma once
#include <utility>
#include <memory>
#include <optional>
#include "Move.h"
#include "Piece.h"

struct MoveDataPair {
	MoveDataPair(Move& move, std::unique_ptr<Piece> taken_over) : move(move), taken_over(std::move(taken_over)) {}

	Move move;
	std::unique_ptr<Piece> taken_over;
};

typedef std::vector<MoveDataPair> MoveData;

class GameMoveData {
public:
	MoveDataPair& getLastMove();
	MoveDataPair& getLastWhiteMove();
	MoveDataPair& getLastBlackMove();

	void pushMove(Move& new_move, GameModel& model);
	void popMove();

private:
	MoveDataPair& getLastMove(ChessColor player_color);

	bool wasBlackLast();

	MoveData white_move_data;
	MoveData black_move_data;
};

