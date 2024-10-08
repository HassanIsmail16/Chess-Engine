#include "GamePlanningManager.h"
#include <iostream>

void GamePlanningManager::createArrow(Position& starting_position) {
	this->current_arrow = new ArrowData{starting_position, starting_position};
}

void GamePlanningManager::updateCurrentArrow(Position& end_position) {
	*(this->current_arrow) = ArrowData(current_arrow->start, end_position);
}

void GamePlanningManager::releaseCurrentArrow() {
	if (!this->current_arrow) {
		return;
	}

	if (this->arrowExists(*(this->current_arrow))) {
		this->removeArrow(*(this->current_arrow));
	} else {
		this->arrows.emplace_back(*(this->current_arrow));
	}

	this->current_arrow = nullptr;
}

void GamePlanningManager::removeArrow(const ArrowData& arrow) {
	auto arrow_itr = std::find(this->arrows.begin(), this->arrows.end(), arrow);

	if (arrow_itr != this->arrows.end()) {
		this->arrows.erase(arrow_itr);
	}
}

void GamePlanningManager::exit() {
	this->clear();
}

std::vector<ArrowData>& GamePlanningManager::getArrows() {
	return this->arrows;
}

ArrowData* GamePlanningManager::getCurrentArrow() {
	return this->current_arrow;
}

void GamePlanningManager::printArrows() {
	for (auto arr : this->arrows) {
		std::cout << arr.start.col << " " << arr.start.row << " " << arr.end.col << " " << arr.end.row << std::endl;
	}
}

void GamePlanningManager::clear() {
	this->current_arrow = nullptr;
	this->arrows.clear();
}

bool GamePlanningManager::arrowExists(const ArrowData& arrow) {
	auto arrow_itr = std::find(this->arrows.begin(), this->arrows.end(), arrow);
	return arrow_itr != this->arrows.end();
}
