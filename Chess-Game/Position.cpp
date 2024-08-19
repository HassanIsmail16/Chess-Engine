#include "Position.h"

Position::Position(Col col, Row row): col(col), row(row) {}

Position::Position(int col, int row): col(static_cast<Col>(col)), row(static_cast<Row>(row)) {}

const bool& Position::operator==(Position& other_position) const {
	return this->col == other_position.col && this->row == other_position.row;
}

const bool& Position::operator!=(Position& other_position) const {
	return !this->operator==(other_position);
}
