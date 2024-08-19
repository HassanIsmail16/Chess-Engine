#pragma once

#include <vector>
#include "Position.h"

class GameModel;

enum class ChessColor {
	White,
	Black
};

class Piece {
public:
	Piece(ChessColor color);

	ChessColor getColor();
	virtual std::vector<Position>& getValidPositions(const GameModel& model) = 0;

protected:
	ChessColor color;
};

class Pawn : public Piece {
public:
	Pawn(ChessColor color);

	std::vector<Position>& getValidPositions(const GameModel& model);
	bool isMoved();

private:
	bool is_moved;
};

class Rook : public Piece {
public:
	Rook(ChessColor color);

	std::vector<Position>& getValidPositions(const GameModel& model);
	bool isMoved();

private:
	bool is_moved;
};

class Bishop : public Piece {
public:
	Bishop(ChessColor color);

	std::vector<Position>& getValidPositions(const GameModel& model);
};

class Knight : public Piece {
public:
	Knight(ChessColor color);

	std::vector<Position>& getValidPositions(const GameModel& model);
};

class Queen : public Piece {
public:
	Queen(ChessColor color);

	std::vector<Position>& getValidPositions(const GameModel& model);
};

class King : public Piece {
public:
	King(ChessColor color);

	std::vector<Position>& getValidPositions(const GameModel& model);
	bool isChecked();
	bool isMoved();

private:
	bool is_checked;
	bool is_moved;
};

