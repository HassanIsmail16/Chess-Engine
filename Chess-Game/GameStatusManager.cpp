#include "GameStatusManager.h"
#include <iostream>

GameStatusManager::GameStatusManager() {
	this->current_player = ChessColor::White;
}

bool GameStatusManager::isCurrentPlayerPiece(Piece* piece) {
	return piece->getColor() == this->current_player;
}

void GameStatusManager::updateCurrentPlayer() {
	this->current_player = (this->current_player == ChessColor::White) ? ChessColor::Black : ChessColor::White;
}


