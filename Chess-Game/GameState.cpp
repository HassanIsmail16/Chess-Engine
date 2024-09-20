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
	this->handlePawnPromotion();

	if (!this->isActionAllowed()) {
		return;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// update action time
		last_action_time = this->clock.getElapsedTime();

		Piece* selected_piece(this->model->movement_manager.getSelectedPiece());

		if (this->isValidPlayerPiece(selected_piece)) {
			this->handleMovement(selected_piece);
					std::cout << "num white moves: " << this->model->movement_manager.getMoveData().white_move_data.size() << std::endl
			<< "num black moves: " << this->model->movement_manager.getMoveData().black_move_data.size() << std::endl;
		} 

		this->selectPieceAtMousePosition();
	}
}

void GameState::update(const float& dt) {
	this->model->status_manager.isDraw(*(this->model));
	this->model->status_manager.hasBlackWon(*(this->model));
	this->model->status_manager.hasWhiteWon(*(this->model));
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

void GameState::handleMovement(Piece* selected_piece) {
	std::vector<Position> valid_positions(selected_piece->getValidPositions(*(this->model)));

	Position selected_position(this->getSquareAtMousePosition());

	// handling castling
	if (selected_piece->getPieceType() == PieceType::King) {
		this->handleCastling(selected_piece->getPosition(), valid_positions, selected_position);
	}

	// handling other moves
	if (this->isSelectedPositionValid(valid_positions, selected_position)) {
		this->applyMove(Move(selected_piece->getPosition(), selected_position));
		return;
	}
}

bool GameState::isSelectedPositionValid(std::vector<Position>& valid_positions, Position& selected_position) {
	auto selected_position_iterator = std::find(valid_positions.begin(), valid_positions.end(), selected_position);

	std::cout << (selected_position_iterator != valid_positions.end()) << std::endl;
	return selected_position_iterator != valid_positions.end();
}

void GameState::applyMove(Move move) {
	this->model->movement_manager.makeMove(move, *(this->model));

	this->model->movement_manager.unselectPiece();

	this->model->status_manager.updateCurrentPlayer();
}

void GameState::handleCastling(Position king_position, std::vector<Position>& valid_positions, Position& selected_position) {
	if (!isCastling(king_position, selected_position)) {
		return;
	}

	bool is_king_side = (selected_position.col > king_position.col);
	if (!canCastle(king_position, valid_positions, is_king_side)) {
		return;
	}

	Position current_rook = (is_king_side) ? Position(7, king_position.row) : Position(0, king_position.row);
	Position rook_destination = (is_king_side) ? Position(5, king_position.row) : Position(3, king_position.row);
	Position king_destination = (is_king_side) ? Position(6, king_position.row) : Position(2, king_position.row);

	this->model->movement_manager.makeMove(Move(king_position, king_destination), *(this->model));
	this->applyMove(Move(current_rook, rook_destination));
}

bool GameState::isCastling(Position& king_position, Position& selected_position) {
	if (selected_position.row != king_position.row) {
		return false;
	}
	
	if (selected_position.col != 6 && selected_position.col != 2
		&& selected_position.col != 7 && selected_position.col != 0) {
		return false;
	}

	return true;
}

bool GameState::canCastle(Position& king_position, std::vector<Position>& valid_positions, bool is_king_side) {
	Position king_side(Col::KingSideCastle, king_position.row);
	Position queen_side(Col::QueenSideCasle, king_position.row);

	return (is_king_side && this->isSelectedPositionValid(valid_positions, king_side)) ||
		(!is_king_side && this->isSelectedPositionValid(valid_positions, queen_side));
}

void GameState::handlePawnPromotion() {
	if (!this->model->movement_manager.isPromotionPending()) {
		return;
	} // no promotions are pending

	std::cout << "promote your pawn (1: Rook, 2: Knight, 3: Queen, 4: Bishop)" << std::endl;

	// TODO: update this to an event-based system
	while (true) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
			this->model->movement_manager.handlePawnPromotion(PieceType::Rook);
			std::cout << "promoted pawn to rook" << std::endl;
			return;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
			this->model->movement_manager.handlePawnPromotion(PieceType::Knight);
			std::cout << "promoted pawn to knight" << std::endl;
			return;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
			this->model->movement_manager.handlePawnPromotion(PieceType::Queen);
			std::cout << "promoted pawn to queen" << std::endl;
			return;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
			this->model->movement_manager.handlePawnPromotion(PieceType::Bishop);
			std::cout << "promoted pawn to bishop" << std::endl;
			return;
		}
	}
}

bool GameState::isActionAllowed() {
	sf::Time current_time(this->clock.getElapsedTime());

	return current_time - this->last_action_time >= DEBOUNCE_TIME;
}

bool GameState::isValidPlayerPiece(Piece* selected_piece) {
	return selected_piece && this->model->status_manager.isCurrentPlayerPiece(selected_piece);
}
