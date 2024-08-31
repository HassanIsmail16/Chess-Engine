#pragma once

#include "Board.h"
#include "Move.h"
#include "GameMovementManager.h"
#include "GameStatusManager.h"
#include <optional>

/*
* when game model is initialized, the board will be initialized with pieces and what not
* when a new game begins the model will be reset
* the gamestate will take input from the player if it's their turn
* the input will be passed onto model to be updated and update view accordingly
*/

class GameModel {
public:
	GameModel();

	void reset();

	void update();
	void updateTurn();
	void updateBoard();

	const std::vector<Position> getValidPositions(Piece* piece);

	Board& getBoard();

	const bool isMainPlayerTurn() const;

	const bool mainPlayerWon();
	const bool mainPlayerLost();
	const bool isStaleMate();

	bool isKingChecked(ChessColor king_color);

	void handlePromotions();

	GameMovementManager movement_manager;
	GameStatusManager status_manager;
private:
	// ---- methods ----

	void initBoard();
	void initPlayers();

	bool searchDiagonalCheck(Position& king_position, ChessColor& king_color);
	bool searchStraightLineCheck(Position& king_position, ChessColor& king_color);
	bool searchLineCheck(Position& king_position, ChessColor& king_color, const int dx, const int dy);
	bool searchKnightCheck(Position& king_position, ChessColor& king_color);

	// ---- attributes ----

	Board board;
	ChessColor main_player_color = ChessColor::White;
	ChessColor current_player;
	
	/*
	GameStatus game_status;
	GameMoveData move_data;
	PlayerData player_data;
	*/
};

