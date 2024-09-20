#pragma once
#include <utility>
#include <memory>
#include <optional>
#include <SFML/System.hpp>
#include "Move.h"
#include "Piece.h"

extern sf::Clock move_clock;

struct MoveDataPair {
	MoveDataPair(Move& move, std::unique_ptr<Piece> taken_over) : move(move), taken_over(std::move(taken_over)), time(move_clock.getElapsedTime()) {}

	Move move;
	std::unique_ptr<Piece> taken_over;
	sf::Time time;
};

typedef std::vector<MoveDataPair> MoveData;

class GameMoveData {
public:
	MoveDataPair& getLastMove();
	MoveDataPair& getLastWhiteMove();
	MoveDataPair& getLastBlackMove();

	bool noMoves();
	bool noWhiteMoves();
	bool noBlackMoves();

	void pushMove(Move& new_move, GameModel& model);
	void popMove();

	MoveData white_move_data;
	MoveData black_move_data;
private:
	MoveDataPair& getLastMove(ChessColor player_color);
	
	bool wasBlackLast();
};

