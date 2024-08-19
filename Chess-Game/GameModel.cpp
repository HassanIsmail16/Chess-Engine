#include "GameModel.h"

GameModel::GameModel() {
	this->initBoard();
	this->initPlayers();
}

void GameModel::reset() {
	// TODO: implement
}

void GameModel::update() {
	// TODO: implement;
}

void GameModel::updateBoard() {
	// TODO: implement;
}

Piece& GameModel::getPiece(Position position) {
	return this->board.getPieceAt(position);
}

Piece& GameModel::getPiece(int col, int row) {
	// NOTE: 0-indexed
	return this->board.getPieceAt(col, row);
}

Piece& GameModel::getPiece(Col col, Row row) {
	return this->board.getPieceAt(col, row);
}

const bool GameModel::isOpponentPiece(Piece& piece) const {
	return piece.getColor() != this->main_player_color;
}

const bool GameModel::isMainPlayerPiece(Piece& piece) const {
	return !(this->isOpponentPiece(piece));
}

const std::vector<Position>& GameModel::getValidPositions(Piece& piece) const {
	return piece.getValidPositions(*this);
}

Board& GameModel::getBoard() {
	return this->board;
}

const bool GameModel::isMainPlayerTurn() const {
	return this->current_player == this->main_player_color;
}

const bool GameModel::mainPlayerWon() const {
	// TODO: implement
	return true;
}

const bool GameModel::mainPlayerLost() const {
	// TODO: implement
	return true;
}

const bool GameModel::isStaleMate() const {
	// TODO: implement
	return true;

}

const bool GameModel::isWhiteChecked() const {
	// TODO: implement
	return true;

}

const bool GameModel::isBlackChecked() const {
	// TODO: implement
	return true;

}

void GameModel::makeMove(Move& new_move) {
	// TODO: implement;
}

void GameModel::handlePromotions() {
	// TODO: implement;
}

void GameModel::initBoard() {
	this->board = Board();
}

void GameModel::initPlayers() {
	this->main_player_color = ChessColor::White;
	this->current_player = ChessColor::White;
}