#include "Square.h"


std::map<SquareColor, sf::Color> shape_color_match{
	{SquareColor::Black, sf::Color::Black},
	{SquareColor::White, sf::Color::White},
	{SquareColor::Active, sf::Color::Yellow},
	{SquareColor::Alert, sf::Color::Red},
	{SquareColor::Highlight, sf::Color::Green}
};

Square::Square(std::shared_ptr<GameData> game_data, Position position, float size) : Shape(game_data, Type::Rectangle) {
	this->setSize(this->size);

	this->position = position;

	this->initPiece();

	this->setColor(this->getDefaultColor());
}

sf::RectangleShape& Square::getShape() {
	return this->rectangle_data;
}

void Square::setPiece(std::shared_ptr<Piece> new_piece) {
	this->current_piece = new_piece;
}

void Square::removePiece() {
	this->current_piece = nullptr;
}

Piece& Square::getPiece() {
	if (this->isEmpty()) {
		throw "ERROR: square has no piece";
	}

	return *(this->current_piece);
}

void Square::initPiece() {
	PieceColor piece_color = (this->position.row == Row::Two || this->position.row == Row::One) ? PieceColor::White : PieceColor::Black;

	if (this->position.row == Row::Two || this->position.row == Row::Seven) {
		this->current_piece = std::make_shared<Pawn>(this->game_data, piece_color);
		return;
	} 

	if (this->position.col == Col::A || this->position.col == Col::H) {
		this->current_piece == std::make_shared<Rook>(this->game_data, piece_color);
		return;
	}
	
	if (this->position.col == Col::B || this->position.col == Col::G) {
		this->current_piece == std::make_shared<Knight>(this->game_data, piece_color);
		return;
	}
	
	if (this->position.col == Col::C || this->position.col == Col::F) {
		this->current_piece == std::make_shared<Bishop>(this->game_data, piece_color);
		return;
	}
	
	if (this->position.col == Col::D) {
		this->current_piece = std::make_shared<Queen>(this->game_data, piece_color);
		return;
	}
	
	if (this->position.col == Col::E) {
		this->current_piece = std::make_shared<King>(this->game_data, piece_color);
		return;
	}
}

void Square::setSize(float new_size) {
	this->size = new_size;
	this->rectangle_data.setSize(sf::Vector2f(new_size, new_size));
}

const SquareColor Square::getDefaultColor() const {
	return ((static_cast<int>(this->position.row) + static_cast<int>(this->position.col)) % 2 == 0) ? SquareColor::Black : SquareColor::White;
}

void Square::setColor(SquareColor new_color) {
	this->current_color = new_color;
	this->getShape().setFillColor(shape_color_match[new_color]);
}

void Square::activate() {
	if (!this->isEmpty()) {
		this->setColor(SquareColor::Active);
		this->current_piece->activate();
	}
}

void Square::highlight() {
	this->setColor(SquareColor::Highlight);
}

const bool Square::isEmpty() const {
	return this->current_piece == nullptr;
}
