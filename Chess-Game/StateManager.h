#pragma once

#include <stack>
#include <memory>
#include "GameModel.h"
#include "GameView.h"

class State;

class StateManager {
public:
	StateManager();
	StateManager(GameView* view, GameModel* model);

	void addState(std::unique_ptr<State> new_state);
	void removeState();
	void replaceCurrentState(std::unique_ptr<State> new_state);

	void processStateChanges();

	const bool hasState() const;
	
	State& getCurrentState();

private:
	GameView* view;
	GameModel* model;
	std::stack<std::unique_ptr<State>> states;
	std::unique_ptr<State> pending_state;
	bool state_removal_pending = false;
};

