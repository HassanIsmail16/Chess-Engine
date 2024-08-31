#pragma once
#include "Piece.h"

class GameStatusManager {
public:
	GameStatusManager();

	bool isCurrentPlayerPiece(Piece* piece);
	
	void updateCurrentPlayer();

private:
	ChessColor current_player;
};

