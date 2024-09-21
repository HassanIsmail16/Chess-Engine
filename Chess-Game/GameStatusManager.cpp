#include "GameStatusManager.h"
#include "GameModel.h"
#include <iostream>
#include <map>

GameStatusManager::GameStatusManager() {
	this->current_player = ChessColor::White;
}

bool GameStatusManager::isCurrentPlayerPiece(Piece* piece) {
	return piece->getColor() == this->current_player;
}

void GameStatusManager::updateCurrentPlayer() {
	this->current_player = (this->current_player == ChessColor::White) ? ChessColor::Black : ChessColor::White;
}

bool GameStatusManager::isDraw(GameModel& model) {
	bool stalemate = this->isStaleMate(model);
	bool insufficient_materials = this->insufficientMaterials(model);
	bool draw_by_repitition = this->isDrawByRepitition(model);
	
	std::cout << (stalemate ? "stalemate\n" : "")
		<< (insufficient_materials ? "insuff_materials\n" : "")
		<< (draw_by_repitition ? "repitition\n" : "");

	return stalemate || insufficient_materials || draw_by_repitition;
}

bool GameStatusManager::hasWhiteWon(GameModel& model) {
	return this->isCheckMated(model, ChessColor::Black);
}

bool GameStatusManager::hasBlackWon(GameModel& model) {
	return this->isCheckMated(model, ChessColor::White);
}

bool GameStatusManager::isKingChecked(GameModel& model, ChessColor king_color) {
	auto& king = model.getBoard().getKing(king_color);

	if (!king) {
		std::cerr << "error no king" << std::endl;
	}

	Position king_position = king->getPosition();

	bool straight_line_check = this->searchStraightLineCheck(model, king_position, king_color);
	bool diagonal_check = this->searchDiagonalCheck(model, king_position, king_color);
	bool knight_check = this->searchKnightCheck(model, king_position, king_color);

	return straight_line_check || diagonal_check || knight_check;
}

void GameStatusManager::registerBoardState(Board& board) {
	std::string current_state_hash = this->generateBoardStateHash(board);

	this->board_state_history[current_state_hash]++;
}

void GameStatusManager::undoBoardStateRegistery(Board& board) {
	std::string current_state_hash = this->generateBoardStateHash(board);

	this->board_state_history[current_state_hash]--;
}

bool GameStatusManager::isStaleMate(GameModel& model) {
	bool is_white_stale(this->isPlayerStale(model, ChessColor::White));
	bool is_black_stale(this->isPlayerStale(model, ChessColor::Black));

	return is_white_stale || is_black_stale;
}

bool GameStatusManager::insufficientMaterials(GameModel& model) {
	auto& board = model.getBoard();

	auto countMaterials = [&board](ChessColor player_color, int& bishop_count, int& knight_count, bool& has_other_materials) {
		for (int row = 0; row < BOARD_SIZE; row++) {
			for (int col = 0; col < BOARD_SIZE; col++) {
				Position current_position(col, row);

				if (!board.hasPieceAt(current_position)) {
					continue;
				}

				auto& piece = board.getPieceAt(col, row);

				if (piece->getColor() == player_color) {
					switch (piece->getPieceType()) {
						case PieceType::Pawn:
						case PieceType::Rook:
						case PieceType::Queen:
						has_other_materials = true;
						return;
						break;

						case PieceType::Knight:
						knight_count++;
						break;

						case PieceType::Bishop:
						bishop_count++;
						break;

						default:
						break;
					}
				}
			}
		}
	};

	int white_bishop_count = 0;
	int white_knight_count = 0;
	bool white_has_other_materials = false;
	countMaterials(ChessColor::White, white_bishop_count, white_knight_count, white_has_other_materials);
	
	int black_bishop_count = 0;
	int black_knight_count = 0;
	bool black_has_other_materials = false;
	countMaterials(ChessColor::Black, black_bishop_count, black_knight_count, black_has_other_materials);

	// there are pieces that can checkmate
	if (white_has_other_materials || black_has_other_materials) {
		return false;
	}

	// king vs king
	if ((white_bishop_count == 0 && white_knight_count == 0)
		&& (black_bishop_count == 0 && black_knight_count == 0)) {
		return true;
	}

	// knight & king vs king
	if ((white_bishop_count == 0 && white_knight_count == 1)
		&& (black_bishop_count == 0 && black_knight_count == 0)) {
		return true;
	}

	// king vs knight & king
	if ((white_bishop_count == 0 && white_knight_count == 0)
		&& (black_bishop_count == 0 && black_knight_count == 1)) {
		return true;
	}

	// bishop and king vs king
	if ((white_bishop_count == 1 && white_knight_count == 0)
		&& (black_bishop_count == 0 && black_knight_count == 0)) {
		return true;
	}

	// king vs bishop and king
	if ((white_bishop_count == 0 && white_knight_count == 0)
		&& (black_bishop_count == 1 && black_knight_count == 0)) {
		return true;
	}

	// only works if there is a lone bishop
	auto getLoneBishopSquareColor = [&board](ChessColor bishop_color) -> ChessColor {
		for (int row = 0; row < BOARD_SIZE; row++) {
			for (int col = 0; col < BOARD_SIZE; col++) {
				Position current_position(col, row);

				if (board.hasPieceAt(current_position)) {
					auto& piece = board.getPieceAt(current_position);

					if (piece->getPieceType() == PieceType::Bishop) {
						return ((col + row) % 2 == 0) ? ChessColor::Black : ChessColor::White;
					}
				}
			}
		}
		return bishop_color;
		};

	ChessColor white_bishop_square_color = getLoneBishopSquareColor(ChessColor::White);
	ChessColor black_bishop_square_color = getLoneBishopSquareColor(ChessColor::Black);

	// bishop and king vs bishop and king (both bishops on same color
	if ((white_bishop_count == 1 && white_knight_count == 0)
		&& (black_bishop_count == 1 && black_knight_count == 0)) {
		if (white_bishop_square_color == black_bishop_square_color) {
			return true;
		}
	}

	return false;
}

bool GameStatusManager::isDrawByRepitition(GameModel& model) {
	auto& board = model.getBoard();

	for (auto it = this->board_state_history.begin(); it != this->board_state_history.end(); it++) {
		if (it->second == 3) {
			return true;
		}
	}

	return false;
}

bool GameStatusManager::playerHasMoves(GameModel& model, ChessColor player_color) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			Position current_position(col, row);

			if (!model.getBoard().hasPieceAt(current_position)) {
				continue;
			}

			auto& current_piece(model.getBoard().getPieceAt(current_position));

			if (current_piece->getColor() != player_color) {
				continue;
			}

			if (!current_piece->getValidPositions(model).empty()) {
				return true;
			}
		}
	}

	return false;
}

bool GameStatusManager::isPlayerStale(GameModel& model, ChessColor player_color) {
	return !this->isKingChecked(model, player_color) && !this->playerHasMoves(model, player_color);
}

bool GameStatusManager::isCheckMated(GameModel& model, ChessColor player_color) {
	return this->isKingChecked(model, player_color) && !this->playerHasMoves(model, player_color);
}

bool GameStatusManager::searchDiagonalCheck(GameModel& model, Position& king_position, ChessColor& king_color) {
	bool north_east_check = this->searchLineCheck(model, king_position, king_color, 1, 1);
	bool north_west_check = this->searchLineCheck(model, king_position, king_color, -1, 1);
	bool south_east_check = this->searchLineCheck(model, king_position, king_color, 1, -1);
	bool south_west_check = this->searchLineCheck(model, king_position, king_color, -1, -1);

	return north_east_check || north_west_check || south_east_check || south_west_check;
}

bool GameStatusManager::searchStraightLineCheck(GameModel& model, Position& king_position, ChessColor& king_color) {
	bool north_check = this->searchLineCheck(model, king_position, king_color, 0, 1);
	bool south_check = this->searchLineCheck(model, king_position, king_color, 0, -1);
	bool east_check = this->searchLineCheck(model, king_position, king_color, 1, 0);
	bool west_check = this->searchLineCheck(model, king_position, king_color, -1, 0);

	return north_check || south_check || east_check || west_check;
}

bool GameStatusManager::searchLineCheck(GameModel& model, Position& king_position, ChessColor& king_color, const int dx, const int dy) {
	for (int row = king_position.row + dy, col = king_position.col + dx, cnt = 0; true; row += dy, col += dx, cnt++) {
		if (!model.getBoard().isInBound(col, row)) {
			return false;
		}

		Position current_position(col, row);

		if (!model.getBoard().hasPieceAt(current_position)) {
			continue;
		}

		auto& found_piece = model.getBoard().getPieceAt(current_position);

		if (found_piece->getColor() == king_color) {
			return false;
		}

		// handle pawns and king
		if (cnt == 0) {
			if (found_piece->getPieceType() == PieceType::King) {
				return true;
			} else if (found_piece->getPieceType() == PieceType::Pawn) {
				if (king_color == ChessColor::White) {
					return (dy == 1 && abs(dx) == 1);
				} else {
					return (dy == -1 && abs(dx) == 1);
				}
			}
		}

		if (found_piece->getPieceType() == PieceType::Queen) {
			return true;
		}

		if (found_piece->getPieceType() == PieceType::Rook) {
			return (abs(dx) + abs(dy) == 1);
		}

		if (found_piece->getPieceType() == PieceType::Bishop) {
			return (abs(dx) + abs(dy) == 2);
		}

		return false;
	}
}

bool GameStatusManager::searchKnightCheck(GameModel& model, Position& king_position, ChessColor& king_color) {
	std::array<Position, 8> attacking_knight_positions = {
		Position(king_position.col - 1, king_position.row - 2),
		Position(king_position.col - 2, king_position.row - 1),
		Position(king_position.col - 2, king_position.row + 1),
		Position(king_position.col - 1, king_position.row + 2),
		Position(king_position.col + 1, king_position.row + 2),
		Position(king_position.col + 2, king_position.row + 1),
		Position(king_position.col + 2, king_position.row - 1),
		Position(king_position.col + 1, king_position.row - 2)
	};

	for (auto position : attacking_knight_positions) {
		if (!model.getBoard().isInBound(position)) {
			continue;
		}

		if (!model.getBoard().hasPieceAt(position)) {
			continue;
		}

		auto& found_piece = model.getBoard().getPieceAt(position);

		if (found_piece->getColor() == king_color || found_piece->getPieceType() != PieceType::Knight) {
			continue;
		}

		return true;
	}

	return false;
}

std::string GameStatusManager::generateBoardStateHash(Board& board) {
	std::string hash;

	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			if (!board.hasPieceAt(Position(col, row))) {
				hash += "0;";
				continue;
			}

			auto& current_piece = board.getPieceAt(Position(col, row));

			hash += static_cast<int>(current_piece->getPieceType());
			hash += static_cast<int>(current_piece->getColor());
			hash += ";";
		}
	}

	return hash;
}