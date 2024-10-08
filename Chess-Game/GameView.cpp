#include "GameView.h"
#include <iostream>

#define PI 3.14159265358979323846

std::map<SquareColor, sf::Color> square_color_match;

GameView::GameView(sf::RenderWindow* window, GameModel* model) : window(window), model(model) {
	// TODO: Figure out a better way to handle this map
	square_color_match[SquareColor::Black] = sf::Color(168, 125, 81);
	square_color_match[SquareColor::White] = sf::Color(247, 206, 163);
	square_color_match[SquareColor::Active] = sf::Color(214, 152, 88);
	square_color_match[SquareColor::Highlight] = sf::Color(248, 255, 176);
	square_color_match[SquareColor::Check] = sf::Color(255, 77, 0);
	square_color_match[SquareColor::TakeOver] = sf::Color(255, 91, 77);
}

void GameView::renderMainMenuStateView(const float& dt) {
	this->window->clear(sf::Color::Magenta);

	this->window->display();
}

void GameView::renderGameStateView(const float& dt) {
	this->window->clear(sf::Color::Black);

	this->drawGameStateBoard(dt);

	this->drawPlan(dt);

	this->window->display();
}

sf::RenderWindow& GameView::getWindow() {
	return *(this->window);
}

void GameView::updateView() {
	// get current window dimensions
	float width = static_cast<float>(this->window->getSize().x);
	float height = static_cast<float>(this->window->getSize().y);

	// calculate size for a square view
	float size = std::min(width, height);

	// create a square view
	sf::View view(sf::FloatRect(0, 0, size, size));

	// center the view in the window
	view.setCenter(size / 2, size / 2);

	// set the viewport to fill the window while maintaining a 1:1 aspect ratio
	float left = (width - size) / 2 / width;
	float top = (height - size) / 2 / height;
	float viewportWidth = size / width;
	float viewportHeight = size / height;

	view.setViewport(sf::FloatRect(left, top, viewportWidth, viewportHeight));

	this->window->setView(view);
}

float GameView::getSquareSize() const {
	return this->square_size;
}

void GameView::drawGameStateBoard(const float& dt) {
	this->updateSquareColors();

	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			Position current_position(col, row);

			this->drawSquare(dt, current_position);

			this->drawPiece(dt, current_position);
		}
	}
}

void GameView::drawSquare(const float& dt, Position& position) {
	// if the sum of the row and col is even the square will be black, else it'll be white
	float square_size = std::min(this->window->getSize().x, this->window->getSize().y) / 8.0f;

	this->square_size = square_size;

	sf::Vector2f square_position{
		square_size * static_cast<float>(position.col),
		square_size * static_cast<float>(position.row)
	};

	sf::RectangleShape square(sf::Vector2f(square_size, square_size));

	SquareColor square_color_key = this->getSquareColor(position);

	sf::Color square_color = square_color_match[square_color_key];

	square.setFillColor(square_color);

	square.setPosition(square_position);

	this->window->draw(square);
}

void GameView::drawPiece(const float& dt, Position& position) {
	if (!this->model->getBoard().hasPieceAt(position)) {
		return;
	}

	auto& current_piece = this->model->getBoard().getPieceAt(position);

	sf::Vector2f piece_position {
		square_size * static_cast<float>(position.col) + square_size / 2,
		square_size * static_cast<float>(position.row) + square_size / 2
	};

	sf::Sprite piece(this->texture_manager.getTexture(current_piece->getPieceType(), current_piece->getColor()));
	

	float piece_scale = this->square_size / piece.getLocalBounds().width + 0.1;

	piece.setScale(piece_scale, piece_scale);
	piece.setOrigin(piece.getLocalBounds().width / 2, piece.getLocalBounds().height / 2);
	piece.setPosition(piece_position);

	this->window->draw(piece);
}

void GameView::drawPlan(const float& dt) {
	auto* current_arrow = this->model->planning_manager.getCurrentArrow();

	if (current_arrow) {
		if (isCircle(*current_arrow)) {
			this->drawCircle(dt, *current_arrow);
		} else {
			this->drawArrow(dt, *current_arrow);
		}
	}

	for (auto& arrow : this->model->planning_manager.getArrows()) {
		if (this->isCircle(arrow)) {
			this->drawCircle(dt, arrow);
		} else {
			this->drawArrow(dt, arrow);
		}
	}
}

// TODO: Fix arrow head
void GameView::drawArrow(const float& dt, ArrowData& arrow_data) {
	// Calculate the start and end positions based on square size
	sf::Vector2f start{
		this->square_size * static_cast<float>(arrow_data.start.col) + (this->square_size / 2.0f),
		this->square_size * static_cast<float>(arrow_data.start.row) + (this->square_size / 2.0f)
	};
	sf::Vector2f end{
		this->square_size * static_cast<float>(arrow_data.end.col) + (this->square_size / 2.0f),
		this->square_size * static_cast<float>(arrow_data.end.row) + (this->square_size / 2.0f)
	};

	// Create the vector from start to end
	sf::Vector2f vector = end - start;
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	float angle = std::atan2(vector.y, vector.x) * (180 / PI); // Convert to degrees

	// Set the height of the arrow
	float height = this->square_size / 9.f; // Adjust height relative to square size
	sf::RectangleShape arrow_body(sf::Vector2f(length, height));

	// Set the origin to the center of the rectangle
	arrow_body.setOrigin(length / 2.0f, height / 2.0f);

	// Set the position based on the midpoint between start and end
	sf::Vector2f midpoint = (start + end) / 2.0f;
	arrow_body.setPosition(midpoint);

	// Set the rotation of the arrow
	arrow_body.setRotation(angle);

	// Set the fill color and draw the arrow
	arrow_body.setFillColor(sf::Color::Black);
	this->window->draw(arrow_body);

	// Draw arrowhead
	const float arrowhead_length = this->square_size / 1.7f; // Length of the arrowhead
	const float arrowhead_angle = 30.f; // Angle of the arrowhead

	// Calculate position for the arrowhead (it should be above the tip of the arrow)
	sf::Vector2f arrowhead_tip = end; // The tip of the arrow is at the end point
	sf::Vector2f arrowhead_base1 = arrowhead_tip - sf::Vector2f(
		arrowhead_length * std::cos((angle + arrowhead_angle) * (PI / 180)),
		arrowhead_length * std::sin((angle + arrowhead_angle) * (PI / 180))
	);

	sf::Vector2f arrowhead_base2 = arrowhead_tip - sf::Vector2f(
		arrowhead_length * std::cos((angle - arrowhead_angle) * (PI / 180)),
		arrowhead_length * std::sin((angle - arrowhead_angle) * (PI / 180))
	);

	// Move the arrowhead's base up a bit from the tip to avoid intersection
	const float arrowhead_offset = height; // You can adjust this offset as needed

	arrowhead_base1 += sf::Vector2f(
		arrowhead_offset * std::cos(angle * (PI / 180)),
		arrowhead_offset * std::sin(angle * (PI / 180))
	);

	arrowhead_base2 += sf::Vector2f(
		arrowhead_offset * std::cos(angle * (PI / 180)),
		arrowhead_offset * std::sin(angle * (PI / 180))
	);

	// Draw the arrowhead triangles
	sf::ConvexShape arrowhead;
	arrowhead.setPointCount(3);
	arrowhead.setPoint(0, arrowhead_tip);
	arrowhead.setPoint(1, arrowhead_base1);
	arrowhead.setPoint(2, arrowhead_base2);
	arrowhead.setFillColor(sf::Color::Black);

	this->window->draw(arrowhead);
}

void GameView::drawCircle(const float& dt, ArrowData& circle_data) {
	float circle_radius = this->square_size / 2.8f;

	sf::Vector2f circle_position{
		this->square_size * static_cast<float>(circle_data.start.col) + 0.5f * this->square_size,
		this->square_size * static_cast<float>(circle_data.start.row) + 0.5f * this->square_size
	};

	sf::CircleShape circle(circle_radius);

	circle.setOrigin(circle_radius, circle_radius);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(sf::Color::Black);
	circle.setOutlineThickness(std::min(this->window->getSize().x, this->window->getSize().y) * 0.012f);

	circle.setPosition(circle_position);

	this->window->draw(circle);
}

bool GameView::isCircle(ArrowData& arrow_data) {
	return arrow_data.start == arrow_data.end;
}

void GameView::updateSquareColors() {
	bool is_white_checked = this->model->status_manager.isKingChecked(*(this->model), ChessColor::White);
	bool is_black_checked = this->model->status_manager.isKingChecked(*(this->model), ChessColor::Black);

	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			auto& current_piece = this->model->getBoard().getPieceAt(col, row);
			// handle check
			if (current_piece && current_piece->getPieceType() == PieceType::King) {
				if (is_white_checked && current_piece->getColor() == ChessColor::White
					|| is_black_checked && current_piece->getColor() == ChessColor::Black) {
					this->setSquareColor(Position(col, row), SquareColor::Check);
					continue;
				}
			}
			this->resetSquareColor(Position(col, row));
		}
	}

	// handle seleceted piece
	Piece* selected_piece = this->model->movement_manager.getSelectedPiece();

	if (selected_piece == nullptr || !model->status_manager.isCurrentPlayerPiece(selected_piece)) {
		return;
	}

	this->setSquareColor(selected_piece->getPosition(), SquareColor::Active);

	std::vector<Position> valid_positions = this->model->getValidPositions(selected_piece);


	for (Position& position : valid_positions) {
		if (selected_piece->getPieceType() == PieceType::King) {
			if (position.col == Col::KingSideCastle || position.col == Col::QueenSideCasle) {
				int target_rook_col = (position.col == Col::KingSideCastle) ? 7 : 0;
				int movement_direction = (position.col == Col::KingSideCastle) ? 1 : -1;
				
				Position rook_position(target_rook_col, selected_piece->getPosition().row);
				Position side_step(selected_piece->getPosition().col + movement_direction * 2, selected_piece->getPosition().row);

				this->setSquareColor(rook_position, SquareColor::Highlight);
				this->setSquareColor(side_step, SquareColor::Highlight);

				continue;
			}
		}

		if (!this->model->getBoard().hasPieceAt(position)) {
			this->setSquareColor(position, SquareColor::Highlight);
			continue;
		}

		auto& current_piece = this->model->getBoard().getPieceAt(position);

		if (current_piece->getColor() != selected_piece->getColor()) {
			this->setSquareColor(position, SquareColor::TakeOver);
		}
	}
}

void GameView::setSquareColor(Position position, SquareColor new_color) {
	square_colors[position.row][position.col] = new_color;
}

void GameView::resetSquareColor(Position position) {
	SquareColor color = ((position.col + position.row) % 2 == 0) ? SquareColor::Black : SquareColor::White;

	this->setSquareColor(position, color);
}

SquareColor GameView::getSquareColor(const Position& position) {
	return this->square_colors[position.row][position.col];
}

