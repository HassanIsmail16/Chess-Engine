#include "Game.h"
#include <iostream>

Game::Game(float width, float height, std::string title) {
	this->window = std::move(std::make_shared<sf::RenderWindow>(sf::VideoMode(width, height), title));
	this->model = std::move(std::make_shared<GameModel>());
	this->view = std::move(std::make_shared<GameView>(this->window.get(), this->model.get()));
	this->state_manager = StateManager(this->view.get(), this->model.get());
	this->dt_clock = sf::Clock();

	this->view->updateView();

	//this->state_manager.addState(std::make_unique<MainMenuState>(this->view.get(), this->model.get(), &(this->state_manager)));
	this->state_manager.addState(std::make_unique<GameState>(this->view.get(), this->model.get(), &(this->state_manager)));
	this->state_manager.processStateChanges();
	//this->window->setFramerateLimit(60);
}

Game::~Game() {
	this->exit();
}


void Game::run() {
	sf::Event event;

	while (this->window->isOpen()) {
		this->updateDt();

		this->state_manager.getCurrentState().run(this->dt, event);

		this->state_manager.processStateChanges();

		while (this->window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				this->window->close();
				this->exit();
			}

			if (event.type == sf::Event::Resized) {
				this->view->updateView();
			}
		}
	}
}

void Game::exit() {
	while (this->state_manager.hasState()) {
		this->state_manager.removeState();
		this->state_manager.processStateChanges();
	}

	
	std::cout << "Exiting Game..." << std::endl;
}

void Game::updateDt() {
	sf::Time deltaTime = this->dt_clock.restart();
	this->dt = deltaTime.asSeconds();
}



