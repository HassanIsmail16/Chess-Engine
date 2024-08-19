#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "GameData.h"
#include <variant>
class Shape {
public:
	enum class Type {
		Rectangle,
		Sprite
	};

	Shape(std::shared_ptr<GameData> game_data, Type type);

	void draw();
	
protected:
	Type shape_type;
	sf::RectangleShape rectangle_data;
	sf::Sprite sprite_data;
	std::shared_ptr<GameData> game_data;
};

