#ifndef POSITION_H
#define POSITION_H

#include <iostream>

namespace chess {
	class Position {
	public:
		Position();
		Position(const int initalColumn, const int initalRow);
		
		void setPosition(const int newColumn, const int newRow);
		int getColumn() const;
		int getRow() const;

		friend Position operator+(const Position& augend, const Position& addend);
		friend Position operator-(const Position& minuend, const Position& subtrahend);

		friend bool operator==(const Position& comparandLeft, const Position& comparandRight);
		friend bool operator!=(const Position& comparandLeft, const Position& comparandRight);
		friend bool operator<(const Position& comparandLeft, const Position& comparandRight);
		friend bool operator<=(const Position& comparandLeft, const Position& comparandRight);
		friend bool operator>(const Position& comparandLeft, const Position& comparandRight);
		friend bool operator>=(const Position& comparandLeft, const Position& comparandRight);

		void printTest() const; 

		// These bounds are only used for isInBounds() and for use outside of this class.
		// Position objects will not check to make sure that a position falls within these bounds
		//unless isInBounds() is called specfically.
		bool isInBounds() const;
		static const int LOWER_COLUMN_BOUND = 1;
		static const int UPPER_COLUMN_BOUND = 8;
		static const int LOWER_ROW_BOUND = 1;
		static const int UPPER_ROW_BOUND = 8;
		static const bool noBounds = (LOWER_COLUMN_BOUND == UPPER_COLUMN_BOUND || LOWER_ROW_BOUND == UPPER_ROW_BOUND);

	private:
		int column;
		int row;

	};
}

#endif
