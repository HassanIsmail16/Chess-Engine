#include "GameView.h"
#include <iostream>

GameView::GameView(sf::RenderWindow* window, GameModel* model) : window(window), model(model) {}

void GameView::renderMainMenuStateView(const float& dt) {
	this->window->clear(sf::Color::Magenta);

	this->window->display();
}

void GameView::renderGameStateView(const float& dt) {
	this->window->clear(sf::Color::Green);

	this->window->display();
}

void GameView::drawGameStateBoard(const float& dt) {
	
}

