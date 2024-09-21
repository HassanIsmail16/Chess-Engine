#include "TextureManager.h"

TextureManager::TextureManager() {
	this->piece_textures[PieceType::Pawn][ChessColor::White].loadFromFile(TexturePaths::white_pawn);
	this->piece_textures[PieceType::Pawn][ChessColor::Black].loadFromFile(TexturePaths::black_pawn);
	this->piece_textures[PieceType::Knight][ChessColor::White].loadFromFile(TexturePaths::white_knight);
	this->piece_textures[PieceType::Knight][ChessColor::Black].loadFromFile(TexturePaths::black_knight);
	this->piece_textures[PieceType::Rook][ChessColor::White].loadFromFile(TexturePaths::white_rook);
	this->piece_textures[PieceType::Rook][ChessColor::Black].loadFromFile(TexturePaths::black_rook);
	this->piece_textures[PieceType::King][ChessColor::White].loadFromFile(TexturePaths::white_king);
	this->piece_textures[PieceType::King][ChessColor::Black].loadFromFile(TexturePaths::black_king);
	this->piece_textures[PieceType::Bishop][ChessColor::White].loadFromFile(TexturePaths::white_bishop);
	this->piece_textures[PieceType::Bishop][ChessColor::Black].loadFromFile(TexturePaths::black_bishop);
	this->piece_textures[PieceType::Queen][ChessColor::White].loadFromFile(TexturePaths::white_queen);
	this->piece_textures[PieceType::Queen][ChessColor::Black].loadFromFile(TexturePaths::black_queen);

	this->piece_textures[PieceType::Pawn][ChessColor::White].setSmooth(true);
	this->piece_textures[PieceType::Pawn][ChessColor::Black].setSmooth(true);
	this->piece_textures[PieceType::Knight][ChessColor::White].setSmooth(true);
	this->piece_textures[PieceType::Knight][ChessColor::Black].setSmooth(true);
	this->piece_textures[PieceType::Rook][ChessColor::White].setSmooth(true);
	this->piece_textures[PieceType::Rook][ChessColor::Black].setSmooth(true);
	this->piece_textures[PieceType::King][ChessColor::White].setSmooth(true);
	this->piece_textures[PieceType::King][ChessColor::Black].setSmooth(true);
	this->piece_textures[PieceType::Bishop][ChessColor::White].setSmooth(true);
	this->piece_textures[PieceType::Bishop][ChessColor::Black].setSmooth(true);
	this->piece_textures[PieceType::Queen][ChessColor::White].setSmooth(true);
	this->piece_textures[PieceType::Queen][ChessColor::Black].setSmooth(true);
}

const sf::Texture& TextureManager::getTexture(PieceType type, ChessColor color) {
	return this->piece_textures[type][color];
}
