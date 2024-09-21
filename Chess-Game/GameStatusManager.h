#pragma once
#include <string>
#include <unordered_map>
#include "Board.h"
#include "Piece.h"

class GameModel;

class GameStatusManager {
public:
	GameStatusManager();

	bool isCurrentPlayerPiece(Piece* piece);
	
	void updateCurrentPlayer();


	bool isDraw(GameModel& model);
	bool hasWhiteWon(GameModel& model);
	bool hasBlackWon(GameModel& model);

	bool isKingChecked(GameModel& model, ChessColor king_color);

	void registerBoardState(Board& board);
	void undoBoardStateRegistery(Board& board);

private:
	bool isStaleMate(GameModel& model);
	bool insufficientMaterials(GameModel& model);
	bool isDrawByRepitition(GameModel& model);

	bool playerHasMoves(GameModel& model, ChessColor player_color);
	bool isPlayerStale(GameModel& model, ChessColor player_color);
	bool isCheckMated(GameModel& model, ChessColor player_color);

	bool searchDiagonalCheck(GameModel& model, Position& king_position, ChessColor& king_color);
	bool searchStraightLineCheck(GameModel& model, Position& king_position, ChessColor& king_color);
	bool searchLineCheck(GameModel& model, Position& king_position, ChessColor& king_color, const int dx, const int dy);
	bool searchKnightCheck(GameModel& model, Position& king_position, ChessColor& king_color);

	std::string generateBoardStateHash(Board& board);

	ChessColor current_player;
	std::unordered_map<std::string, int> board_state_history;
};

