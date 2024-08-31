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
	
	Position getSquareAtMousePosition() const;
	void selectPieceAtMousePosition() const;

	sf::Clock clock;
	sf::Time last_action_time;
	const sf::Time DEBOUNCE_TIME = sf::seconds(0.2f);
};

