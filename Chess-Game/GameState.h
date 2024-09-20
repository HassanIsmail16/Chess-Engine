#pragma once

#include "State.h"

class GameState : public State {
public:
	GameState(GameView* view, GameModel* model, StateManager* state_manager);

	// main game loop
	void run(const float& dt) override;

private:
	void init() override;
	void handleInput() override;
	void update(const float& dt) override;
	void render(const float& dt) override;
	
	// retrieves the position the mouse is pointing at
	Position getSquareAtMousePosition() const;

	// selects the piece at the current mouse position
	void selectPieceAtMousePosition() const;

	// handles movement for the selected piece, including validation and execution
	void handleMovement(Piece* selected_piece);

	// checks if the selected position is inside the valid_positions vector
	bool isSelectedPositionValid(std::vector<Position>& valid_positions, Position& selected_position);
	
	// makes the move, unselects the selected piece, and updates the current player
	void applyMove(Move move);

	// handles castling logic for the king including validation and execution
	void handleCastling(Position king_position, std::vector<Position>& valid_positions, Position& selected_position);

	// checks if the player is attempting to castle
	bool isCastling(Position& king_position, Position& selected_position);

	// checks if a castling move is allowed 
	bool canCastle(Position& king_position, std::vector<Position>& valid_positions, bool is_king_side);

	void handlePawnPromotion();

	// checks if actions are allowed
	// @return true if the duration between the current action and the last one is greater than DEBOUNCE_TIME
	bool isActionAllowed();

	// checks if there's a selected piece and if it's of the same color as the current player
	// @param selected_piece: the selected piece
	// @return true if the pointer is not null and the piece color is the same as the current player
	bool isValidPlayerPiece(Piece* selected_piece);

	sf::Clock clock; // clock to keep track of the time between actions
	sf::Time last_action_time; // store the time of the last action 
	const sf::Time DEBOUNCE_TIME = sf::seconds(0.2f); // minimum time between allowed actions
};

