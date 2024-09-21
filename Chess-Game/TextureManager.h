#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Piece.h"

namespace TexturePaths {
	const std::string white_pawn("assets/pawn-w.png");
	const std::string black_pawn("assets/pawn-b.png");
	const std::string white_knight("assets/knight-w.png");
	const std::string black_knight("assets/knight-b.png");
	const std::string white_rook("assets/rook-w.png");
	const std::string black_rook("assets/rook-b.png");
	const std::string white_king("assets/king-w.png");
	const std::string black_king("assets/king-b.png");
	const std::string white_bishop("assets/bishop-w.png");
	const std::string black_bishop("assets/bishop-b.png");
	const std::string white_queen("assets/queen-w.png");
	const std::string black_queen("assets/queen-b.png");
}

class TextureManager {
public:
	TextureManager();

	const sf::Texture& getTexture(PieceType type, ChessColor color);

private:
	std::unordered_map<PieceType, std::unordered_map<ChessColor, sf::Texture>> piece_textures;
};

