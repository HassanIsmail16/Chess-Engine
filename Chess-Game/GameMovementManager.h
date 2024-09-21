#pragma once
#include <memory>
#include <optional>
#include "Move.h"
#include "Piece.h"
#include "Board.h"
#include "GameMoveData.h"

class GameModel;

class GameMovementManager {
public:
	GameMovementManager();
	GameMovementManager(Board* board);

	void selectPiece(std::unique_ptr<Piece>& piece);
	void selectPiece(Position position);
	void selectPiece(int col, int row);
	void unselectPiece();
	bool hasSelectedPiece();
	Piece* getSelectedPiece();

	void makeMove(Move move, GameModel& model);
	void undoLastMove(GameModel& model);

	bool willKingBeChecked(ChessColor king_color, Move move, GameModel& model);

	GameMoveData& getMoveData();

	bool isPromotionPending();
	void handlePawnPromotion(PieceType new_piece_type);

private:
	// returns null if no promotion pawn was found, returns a pointer to a unique_ptr if one was found
	std::unique_ptr<Piece>* getPromotionPawn();

	Piece* selected_piece;
	GameMoveData move_data;
	Board* board;
};
