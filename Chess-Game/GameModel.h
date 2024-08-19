#pragma once

#include "Board.h"
#include "Move.h"

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
	void updateBoard();
	//void updateGameStatus()

	Piece& getPiece(Position position);
	Piece& getPiece(int col, int row);
	Piece& getPiece(Col col, Row row);

	const bool isOpponentPiece(Piece& piece) const;
	const bool isMainPlayerPiece(Piece& piece) const;

	const std::vector<Position>& getValidPositions(Piece& piece) const;

	Board& getBoard();

	// const GameMoveData& getMoveData();

	// const PlayerData& getPlayerData();

	const bool isMainPlayerTurn() const;

	const bool mainPlayerWon() const;
	const bool mainPlayerLost() const;
	const bool isStaleMate() const;
	const bool isWhiteChecked() const;
	const bool isBlackChecked() const;

	void makeMove(Move& new_move);
	void handlePromotions();


private:
	// ---- methods ----

	void initBoard();
	void initPlayers();

	// ---- attributes ----

	Board board;
	ChessColor main_player_color;
	ChessColor current_player;
	/*
	GameStatus game_status;
	GameMoveData move_data;
	PlayerData player_data;
	*/
};

