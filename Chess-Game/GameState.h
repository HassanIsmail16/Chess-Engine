#pragma once

#include "State.h"

class GameState : public State {
public:
	GameState(GameView* view, GameModel* model, StateManager* state_manager);

	void run(const float& dt) override;

private:
	void init() override;
	void handleInput() override;
	void update(const float& dt) override;
	void render(const float& dt) override;
};

