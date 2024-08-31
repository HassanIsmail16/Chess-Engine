#pragma once

#include "Position.h"

struct Move {
	Move(Position start_position, Position end_position): start_position(start_position), end_position(end_position) {}

	Position start_position;
	Position end_position;
};

