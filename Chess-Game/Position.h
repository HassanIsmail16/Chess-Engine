#pragma once

enum Col : int {
	A = 0,
	B = 1,
	C = 2,
	D = 3,
	E = 4,
	F = 5,
	G = 6,
	H = 7,
	KingSideCastle,
	QueenSideCasle
};

enum Row : int {
	One = 0,
	Two = 1,
	Three = 2,
	Four = 3,
	Five = 4,
	Six = 5,
	Seven = 6,
	Eight = 7
};

class Position {
public:
	Position() = default;
	Position(Col col, Row row);
	Position(int col, int row);

	const bool& operator==(const Position& other_position) const;
	const bool& operator!=(const Position& other_position) const;

	Col col;
	Row row;
};

