#include "MainMenuState.h"
#include "GameState.h"
#include <iostream>

MainMenuState::MainMenuState(GameView* view, GameModel* model, StateManager* state_manager) : State(view, model, state_manager) {
	this->init();
}

void MainMenuState::run(const float& dt) {
	this->update(dt);

	this->handleInput();

	this->render(dt);
}

void MainMenuState::init() {}

void MainMenuState::handleInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		std::cout << "Exiting MainMenuState..." << std::endl;

		auto new_state = std::make_unique<GameState>(this->view, this->model, this->state_manager);
		this->state_manager->replaceCurrentState(std::move(new_state));
	}
}

void MainMenuState::update(const float& dt) {}

void MainMenuState::render(const float& dt) {
	this->view->renderMainMenuStateView(dt);
}






