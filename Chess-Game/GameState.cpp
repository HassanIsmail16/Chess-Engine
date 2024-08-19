#include "GameState.h"
#include <iostream>

GameState::GameState(GameView* view, GameModel* model, StateManager* state_manager) : State(view, model, state_manager) {
	this->init();
}

void GameState::run(const float& dt) {
	this->update(dt);

	this->handleInput();

	std::cout << "TEST" << std::endl;

	this->render(dt);
}

void GameState::init() {
	//this->view->renderGameStateView(3);
	return;
}

void GameState::handleInput() {
	return;
}

void GameState::update(const float& dt) {
	return;
}

void GameState::render(const float& dt) {
	this->view->renderGameStateView(dt);
}
