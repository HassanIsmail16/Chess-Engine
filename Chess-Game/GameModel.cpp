#include "GameModel.h"
#include <iostream>

GameModel::GameModel() {
	this->initBoard();
	this->initPlayers();
	this->movement_manager = GameMovementManager(&(this->board));
}

void GameModel::reset() {
	// TODO: implement
}

void GameModel::update() {
	return;
}

void GameModel::updateTurn() {
	this->status_manager.updateCurrentPlayer();
}

const std::vector<Position> GameModel::getValidPositions(Piece* piece) {
	return piece->getValidPositions(*this);
}

Board& GameModel::getBoard() {
	return this->board;
}

void GameModel::initBoard() {
	this->board = Board();
}

void GameModel::initPlayers() {
	this->main_player_color = ChessColor::White;
	this->current_player = ChessColor::White;
}



