#include "Shape.h"

Shape::Shape(std::shared_ptr<GameData> game_data, Type type) {
	this->game_data = game_data;

	if (type == Type::Rectangle) {
		this->rectangle_data = sf::RectangleShape();
	} else {
		this->sprite_data = sf::Sprite();
	}
	
	this->shape_type = type;
}

void Shape::draw() {
	if (this->shape_type == Type::Rectangle) {
		this->game_data->window->draw(this->rectangle_data);
	} else {
		this->game_data->window->draw(this->sprite_data);
	}
}

