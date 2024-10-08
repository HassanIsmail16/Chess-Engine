#pragma once

#include <vector>
#include "Position.h"
#include "Move.h"

struct ArrowData {
	Position start;
	Position end;

	const bool& operator==(const ArrowData& other_arrow) const {
		return this->start == other_arrow.start && this->end == other_arrow.end;
	}
};

class GamePlanningManager {
public:
	void createArrow(Position& starting_position);
	void updateCurrentArrow(Position& end_position);
	void releaseCurrentArrow();
	void removeArrow(const ArrowData& arrow);
	void exit();

	std::vector<ArrowData>& getArrows();
	ArrowData* getCurrentArrow();

	void printArrows();

private:
	void clear();
	bool arrowExists(const ArrowData& arrow);

	ArrowData* current_arrow = nullptr;
	std::vector<ArrowData> arrows;
};

