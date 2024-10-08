#pragma once

#include <string>
#include "StateManager.h"
#include "MainMenuState.h"
#include "GameState.h"

class Game {
public:
	Game(float width, float height, std::string title);
	~Game(); 

	void run();
	
private:
	void exit();
	void updateDt();

private:
	std::shared_ptr<sf::RenderWindow> window;
	std::shared_ptr<GameModel> model;
	std::shared_ptr<GameView> view;
	StateManager state_manager;
	sf::Clock dt_clock;
	float dt;
};

