#include "GameView.h"
#include <iostream>

std::map<SquareColor, sf::Color> square_color_match;

GameView::GameView(sf::RenderWindow* window, GameModel* model) : window(window), model(model) {
	// TODO: Figure out a better way to handle this map
	square_color_match[SquareColor::Black] = sf::Color::Blue;
	square_color_match[SquareColor::White] = sf::Color::White;
	square_color_match[SquareColor::Active] = sf::Color::Red;
	square_color_match[SquareColor::Highlight] = sf::Color::Yellow;
	square_color_match[SquareColor::Check] = sf::Color::Magenta;
	square_color_match[SquareColor::TakeOver] = sf::Color::Black;
}

void GameView::renderMainMenuStateView(const float& dt) {
	this->window->clear(sf::Color::Magenta);

	this->window->display();
}

void GameView::renderGameStateView(const float& dt) {
	this->window->clear(sf::Color::Black);

	this->drawGameStateBoard(dt);

	this->window->display();
}

sf::RenderWindow& GameView::getWindow() {
	return *(this->window);
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
	sf::Vector2f square_size(50.f, 50.f);

	sf::Vector2f square_position(square_size.x * static_cast<float>(position.col), square_size.y * static_cast<float>(position.row));

	sf::RectangleShape square(square_size);

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

	sf::Color piece_color = (current_piece->getColor() == ChessColor::White) ? sf::Color::Green : sf::Color::Magenta;

	float radius = 12.5f;

	sf::Vector2f piece_position(50.f * static_cast<float>(position.col) + 25.f, 50.f * static_cast<float>(position.row) + 25.f);

	sf::Vector2f origin(radius, radius);

	sf::CircleShape piece(radius);

	piece.setOrigin(origin);

	piece.setPosition(piece_position);

	if (this->model->movement_manager.getSelectedPiece() == this->model->getBoard().getPieceAt(position).get()) {
		piece_color = sf::Color::Red;
	}
	
	piece.setFillColor(piece_color);

	this->window->draw(piece);
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

