#pragma once

#include "Board.h"
#include "Move.h"
#include "GameMovementManager.h"
#include "GameStatusManager.h"
#include "GamePlanningManager.h"
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

	const std::vector<Position> getValidPositions(Piece* piece);

	Board& getBoard();

	GameMovementManager movement_manager;
	GameStatusManager status_manager;
	GamePlanningManager planning_manager;
	
private:
	// ---- methods ----

	void initBoard();
	void initPlayers();

	// ---- attributes ----

	Board board;
	ChessColor main_player_color = ChessColor::White;
	ChessColor current_player;
};

