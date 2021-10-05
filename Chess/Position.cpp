#include "Position.h"

namespace chess {
	Position::Position() {
		column = LOWER_COLUMN_BOUND;
		row = LOWER_ROW_BOUND;
	}



	Position::Position(const int initialColumn, const int initialRow) {
		column = initialColumn;
		row = initialRow;
	}



	void Position::setPosition(const int newColumn, const int newRow) {
		column = newColumn;
		row = newRow;
	}



	int Position::getColumn() const {
		return column;
	}



	int Position::getRow() const {
		return row;
	}



	Position operator+(const Position& augend, const Position& addend) {
		Position sum(augend.column + addend.column, augend.row + addend.row);
		return sum;
	}



	Position operator-(const Position& minuend, const Position& subtrahend) {
		Position difference(minuend.column - subtrahend.column, minuend.row - subtrahend.row);
		return difference;
	}



	bool operator==(const Position& comparandLeft, const Position& comparandRight) {
		return ((comparandLeft.column == comparandRight.column) && (comparandLeft.row == comparandRight.row));
	}



	bool operator!=(const Position& comparandLeft, const Position& comparandRight) {
		return ((comparandLeft.column != comparandRight.column) || (comparandLeft.row != comparandRight.row));
	}



	bool operator<(const Position& comparandLeft, const Position& comparandRight) {
		if (comparandLeft.column < comparandRight.column) {
			return true;
		}
		else if (comparandLeft.column > comparandRight.column) {
			return false;
		}
		else { //equal columns
			if (comparandLeft.row < comparandRight.row) {
				return true;
			}
			else {
				return false;
			}
		}
	}



	bool operator<=(const Position& comparandLeft, const Position& comparandRight) {
		if (comparandLeft.column < comparandRight.column) {
			return true;
		}
		else if (comparandLeft.column > comparandRight.column) {
			return false;
		}
		else { //equal columns
			if (comparandLeft.row <= comparandRight.row) {
				return true;
			}
			else {
				return false;
			}
		}
	}



	bool operator>(const Position& comparandLeft, const Position& comparandRight) {
		if (comparandLeft.column > comparandRight.column) {
			return true;
		}
		else if (comparandLeft.column < comparandRight.column) {
			return false;
		}
		else { //equal columns
			if (comparandLeft.row > comparandRight.row) {
				return true;
			}
			else {
				return false;
			}
		}
	}



	bool operator>=(const Position& comparandLeft, const Position& comparandRight) {
		if (comparandLeft.column > comparandRight.column) {
			return true;
		}
		else if (comparandLeft.column < comparandRight.column) {
			return false;
		}
		else { //equal columns
			if (comparandLeft.row >= comparandRight.row) {
				return true;
			}
			else {
				return false;
			}
		}
	}



	bool Position::isInBounds() const {
		if (noBounds) {
			return true;
		}

		return (column >= LOWER_COLUMN_BOUND
			&& column <= UPPER_COLUMN_BOUND
			&& row >= LOWER_ROW_BOUND
			&& row <= UPPER_ROW_BOUND);
	}



	void Position::printTest() const {
		std::cout << "(" << column << ", " << row << ")" << std::endl;
	}

}