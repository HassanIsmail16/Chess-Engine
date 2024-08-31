#include "GameState.h"
#include <iostream>

GameState::GameState(GameView* view, GameModel* model, StateManager* state_manager) : State(view, model, state_manager) {
	this->init();
}

void GameState::run(const float& dt) {
	this->update(dt);
	
	this->handleInput();

	this->render(dt);
}

void GameState::init() {
	//this->view->renderGameStateView(3);
	return;
}

void GameState::handleInput() {
	sf::Time current_time = this->clock.getElapsedTime();

	if (current_time - last_action_time < DEBOUNCE_TIME) {
		return;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		last_action_time = current_time;

		Piece* selected_piece(this->model->movement_manager.getSelectedPiece());

		if (selected_piece && this->model->status_manager.isCurrentPlayerPiece(selected_piece)) {
			std::vector<Position> valid_positions(selected_piece->getValidPositions(*(this->model)));

			Position selected_position(this->getSquareAtMousePosition());
			
			if (selected_piece->getPieceType() == PieceType::King) {
				std::vector<Position> valid_positions = selected_piece->getValidPositions(*(this->model));

				bool can_castle_king_side = false;
				bool can_castle_queen_side = false;

				for (auto position : valid_positions) {
					if (position.col == Col::KingSideCastle) {
						can_castle_king_side = true;
					} 

					if (position.col == Col::QueenSideCasle) {
						can_castle_queen_side = true;
					}

					if (can_castle_king_side && can_castle_queen_side) {
						break;
					}
				}

				if ((this->model->getBoard().hasPieceAt(selected_position) && this->model->getBoard().getPieceAt(selected_position)->getPieceType() == PieceType::Rook)
					|| abs(selected_position.col - selected_piece->getPosition().col) == 2) {
					int movement_direction = (selected_position.col > selected_piece->getPosition().col) ? 1 : -1;

					if (movement_direction == 1 && !can_castle_king_side) {
						return;
					}

					if (movement_direction == -1 && !can_castle_queen_side) {
						return;
					}
					
					int king_destination_col = (movement_direction == 1) ? 6 : 2;
					int rook_destination_col = (movement_direction == 1) ? 5 : 3;

					Position king_destination(king_destination_col, selected_piece->getPosition().row);
					Position rook_destination(rook_destination_col, selected_piece->getPosition().row);

					int rook_current_col = (movement_direction == 1) ? 7 : 0;

					Move king_move(selected_piece->getPosition(), king_destination);
					Move rook_move(Position(rook_current_col, selected_piece->getPosition().row), rook_destination);

					this->model->movement_manager.makeMove(king_move, *(this->model));
					this->model->movement_manager.makeMove(rook_move, *(this->model));

					this->model->movement_manager.unselectPiece();

					this->model->status_manager.updateCurrentPlayer();

					return;
				}
			}

			for (const Position& position : valid_positions) {
				if (position == selected_position) {
					Move new_move(selected_piece->getPosition(), selected_position);

					this->model->movement_manager.makeMove(new_move, *(this->model));

					this->model->movement_manager.unselectPiece();

					this->model->status_manager.updateCurrentPlayer();

					return;
				}
			}
		}
		
		this->selectPieceAtMousePosition();
	}
}

void GameState::update(const float& dt) {
	return;
}

void GameState::render(const float& dt) {
	this->view->renderGameStateView(dt);
}

Position GameState::getSquareAtMousePosition() const {
	sf::Vector2i mouse_position = sf::Mouse::getPosition(this->view->getWindow());

	int selected_col_index = mouse_position.x / 50;
	int selected_row_index = mouse_position.y / 50;

	return Position(selected_col_index, selected_row_index);
}

void GameState::selectPieceAtMousePosition() const {
	Position selected_position = this->getSquareAtMousePosition();

	if (this->model->getBoard().isInBound(selected_position)) {
		this->model->movement_manager.selectPiece(selected_position);
	} else {
		this->model->movement_manager.unselectPiece();
	}
}