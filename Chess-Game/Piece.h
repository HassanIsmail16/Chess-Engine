#pragma once

#include <vector>
#include "Position.h"

enum class PieceType {
	Pawn,
	Rook,
	Knight,
	Bishop,
	King,
	Queen
};

class GameModel;

enum class ChessColor : uint8_t {
	White,
	Black
};

class Piece {
public:
	Piece(ChessColor color, Position position);
	Piece(const Piece& other);
	Piece& operator=(const Piece& other);

	ChessColor getColor();
	virtual std::vector<Position> getValidPositions(GameModel& model) = 0;
	PieceType& getPieceType();

	virtual void moveTo(Position& new_position);
	Position getPosition() const;
	void printType();

	bool isMoved();
	void setIsMoved(bool is_moved);

protected:
	std::vector<Position> getValidStraightLinePositions(GameModel& model, Position current_position, ChessColor color);
	std::vector<Position> getValidDiagonalPositions(GameModel& model, Position current_position, ChessColor color);

	PieceType type;
	ChessColor color;
	Position position;
	bool is_moved = false;

private:
	std::vector<Position> getValidLinePositions(GameModel& model, Position current_position, ChessColor color, const int& dx, const int& dy);
};

class Pawn : public Piece {
public:
	Pawn(ChessColor color, Position position);

	std::vector<Position> getValidPositions(GameModel& model);
};

class Rook : public Piece {
public:
	Rook(ChessColor color, Position position);
	std::vector<Position> getValidPositions(GameModel& model);
};

class Bishop : public Piece {
public:
	Bishop(ChessColor color, Position position);

	std::vector<Position> getValidPositions(GameModel& model);
};

class Knight : public Piece {
public:
	Knight(ChessColor color, Position position);

	std::vector<Position> getValidPositions(GameModel& model);
};

class Queen : public Piece {
public:
	Queen(ChessColor color, Position position);

	std::vector<Position> getValidPositions(GameModel& model);
};

class King : public Piece {
public:
	King(ChessColor color, Position position);

	std::vector<Position> getValidPositions(GameModel& model);
	bool isChecked();

	void handleCastling(GameModel& model, std::vector<Position>& valid_positions);

private:
	bool is_checked = false;
	
	bool canCastleQueenSide(GameModel& model);
	bool canCastleKingSide(GameModel& model);

	bool canCastle(GameModel& model, const bool is_king_side);
	bool rookCanCastle(GameModel& model, ChessColor king_color, const bool is_king_side);
	bool isCastlingPathClearAndSafe(GameModel& model, ChessColor king_color, const bool is_king_side);
};

