#include "StateManager.h"
#include "State.h"
#include <iostream>

StateManager::StateManager() : states(std::stack<std::unique_ptr<State>>()), model(nullptr), view(nullptr) {}

StateManager::StateManager(GameView* view, GameModel* model) {
	this->view = view;
	this->model = model;
	this->states = std::stack<std::unique_ptr<State>>();
}

void StateManager::addState(std::unique_ptr<State> new_state) {
	this->pending_state = std::move(new_state);
	std::cout << "Added state, new stack size: " << this->states.size() << std::endl;
}

void StateManager::removeState() {
	if (!this->hasState()) {
		throw "ERROR::StateManager: invoked removeState() whilst having no states";
	}

	this->state_removal_pending = true;

	std::cout << "Removed state, new stack size: " << this->states.size() << std::endl;
}

void StateManager::replaceCurrentState(std::unique_ptr<State> new_state) {
	this->removeState();
	this->addState(std::move(new_state));
}

void StateManager::processStateChanges() {
	if (this->state_removal_pending) {
		this->states.pop();
		this->state_removal_pending = false;
	}

	if (this->pending_state) {
		this->states.push(std::move(this->pending_state));
		this->pending_state = nullptr;
	}
}

const bool StateManager::hasState() const {
	return !this->states.empty();
}

State& StateManager::getCurrentState() {
	return *(this->states.top().get());
}