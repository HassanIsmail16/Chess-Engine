#pragma once

#include <memory>
#include "GameView.h"
#include "GameModel.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>


class State {
public:
	State(GameView* view, GameModel* model, StateManager* state_manager) : view(view), model(model), state_manager(state_manager) {}

	virtual void run(const float& dt, sf::Event& event) = 0;

protected:
	virtual void init() = 0;

	virtual void handleInput(sf::Event& event) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(const float& dt) = 0;
	
	GameView* view;
	GameModel* model;
	StateManager* state_manager;
};

