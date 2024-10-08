#pragma once

#include "State.h"

class MainMenuState : public State {
public:
	MainMenuState(GameView* view, GameModel* model, StateManager* state_manager);

	void run(const float& dt, sf::Event& event) override;

private:
	void init() override;
	void handleInput(sf::Event& event) override;
	void update(const float& dt) override;
	void render(const float& dt) override;
};

