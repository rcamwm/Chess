#include "ChessPiece.h"

namespace chess {

	Color ChessPiece::playerOneColor = Color::black;
	bool ChessPiece::isPlayerOneColorSet = false;

	ChessPiece::ChessPiece() {
		Position defaultPosition;
		position = defaultPosition;

		type = PieceType::Pawn;
		color = Color::black;
		hasPieceMoved = false;
		setNumberOfMoveTypes();
		determineFullMoveRange();
	}



	ChessPiece::ChessPiece(const int& initialColumn, const int& initialRow, const PieceType& givenType,
		const Color& givenColor, const bool& hasMoved) {
		position = Position(initialColumn, initialRow);
		type = givenType;
		color = givenColor;
		hasPieceMoved = hasMoved;
		setNumberOfMoveTypes();
		determineFullMoveRange();
	}



	ChessPiece::ChessPiece(const Position& initialPosition, const PieceType& givenType,
		const Color& givenColor, const bool& hasMoved) {
		position = initialPosition;
		type = givenType;
		color = givenColor;
		hasPieceMoved = hasMoved;
		setNumberOfMoveTypes();
		determineFullMoveRange();
	}



	Position ChessPiece::getPosition() const {
		return position;
	}
	


	PieceType ChessPiece::getPieceType() const {
		return type;
	}



	Color ChessPiece::getPieceColor() const {
		// Visual Studio causing issues -> simplify later

		// return color;
		if (color == Color::white) {
			return Color::white;
		}
		else {
			return Color::black;
		}
	}



	Color ChessPiece::getOppositePieceColor() const {
		if (color == Color::black) {
			return Color::white;
		}
		else {
			return Color::black;
		}
	}



	bool ChessPiece::getHasPieceMoved() const {
		return hasPieceMoved;
	}



	int ChessPiece::getNumberOfMoveTypes() const {
		return numberOfMoveTypes;
	}



	std::vector<std::vector<Position>> ChessPiece::getFullMoveRange() const {
		return fullMoveRange;
	}



	std::vector<Position> ChessPiece::getLegalMoves() const {
		return legalMoves;
	}



	bool ChessPiece::isMovePossible(const Position& newPosition) {
		for (int index = 0; index < static_cast<int>(legalMoves.size()); index++) {
			if (legalMoves[index] == newPosition) {
				return true;
			}
		}
		return false;
	}

	
	
	void ChessPiece::determineFullMoveRange() {
		switch (type) {
		case PieceType::Pawn:
			determineFullPawnRange();
			break;
		case PieceType::Bishop:
			determineFullBishopRange();
			break;
		case PieceType::Knight:
			determineFullKnightRange();
			break;
		case PieceType::Rook:
			determineFullRookRange();
			break;
		case PieceType::Queen:
			determineFullQueenRange();
			break;
		case PieceType::King:
			determineFullKingRange();
			break;
		default:
			break;
		}
	}



	void ChessPiece::updateLegalMoves(const std::vector<Position> &newMoves) {
		legalMoves = newMoves;
	}



	void ChessPiece::pawnPromotion(const PieceType& newType) {
		if (type == PieceType::Pawn) {
			type = newType;
			setNumberOfMoveTypes();
			determineFullMoveRange();
		}
	}



	void ChessPiece::setPlayerOneColor(const Color& player) {
		if (not isPlayerOneColorSet) {
			playerOneColor = player;
			isPlayerOneColorSet = true;
		}
	}



	void ChessPiece::setNumberOfMoveTypes() {
		switch (type) {
		case PieceType::Pawn:
			numberOfMoveTypes = 2;
			break;
		case PieceType::Bishop:
			numberOfMoveTypes = 4;
			break;
		case PieceType::Knight:
			numberOfMoveTypes = 1;
			break;
		case PieceType::Rook:
			numberOfMoveTypes = 4;
			break;
		case PieceType::Queen:
			numberOfMoveTypes = 8;
			break;
		case PieceType::King:
			numberOfMoveTypes = 1;
			break;
		default:
			break;
		}
	}



	void ChessPiece::determineFullBishopRange() {
		fullMoveRange.clear();
		updateDiagonalMoves();
	}



	void ChessPiece::determineFullKingRange() {
		fullMoveRange.clear();

		std::vector<Position> moveset;

		const int COLUMN_MOVES[4] = { 1,1,0,-1 };
		const int ROW_MOVES[4] = { 0,1,1,1 };
		const int NUMBER_OF_MOVES = 4;

		const int MULTIPLIERS[2] = { 1, -1 };
		const int NUMBER_OF_MULITPLIERS = 2;

		int column = position.getColumn();
		int row = position.getRow();

		for (int multiplier = 0; multiplier < NUMBER_OF_MULITPLIERS; multiplier++) {
			for (int moves = 0; moves < NUMBER_OF_MOVES; moves++) {
				Position possiblePosition(column + (COLUMN_MOVES[moves] * MULTIPLIERS[multiplier]),
					row + (ROW_MOVES[moves] * MULTIPLIERS[multiplier]));
				if (possiblePosition != position && possiblePosition.isInBounds()) {
					moveset.push_back(possiblePosition);
					//possiblePosition.printTest();
				}
			}
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::determineFullKnightRange() {
		fullMoveRange.clear();

		std::vector<Position> moveset;

		const int COLUMN_MULTIPLIERS[4] = { 1, -1, 1, -1 };
		const int ROW_MUTLIPLIERS[4] = { 1, 1, -1, -1 };
		const int NUMBER_OF_MULITPLIERS = 4;

		int column = position.getColumn();
		int row = position.getRow();

		for (int multiplier = 0; multiplier < NUMBER_OF_MULITPLIERS; multiplier++) {
			Position possiblePosition1(column + (COLUMN_MULTIPLIERS[multiplier] * 2),
				row + (ROW_MUTLIPLIERS[multiplier] * 1));
			if (possiblePosition1 != position && possiblePosition1.isInBounds()) {
				moveset.push_back(possiblePosition1);
				//possiblePosition1.printTest();
			}

			Position possiblePosition2(column + (COLUMN_MULTIPLIERS[multiplier] * 1),
				row + (ROW_MUTLIPLIERS[multiplier] * 2));
			if (possiblePosition2 != position && possiblePosition2.isInBounds()) {
				moveset.push_back(possiblePosition2);
				//possiblePosition2.printTest();
			}
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::determineFullPawnRange() {
		fullMoveRange.clear();

		std::vector<Position> movesetAdvance;
		std::vector<Position> movesetCapture;

		int rowAdvance;
		int startingRow;
		if (color == playerOneColor) {
			rowAdvance = 1;
			startingRow = Position::LOWER_ROW_BOUND + 1;
		}
		else {
			rowAdvance = -1;
			startingRow = Position::UPPER_ROW_BOUND - 1;
		}

		for (int columnAdvance = -1; columnAdvance <= 1; columnAdvance++) {
			Position move(position.getColumn() + columnAdvance, position.getRow() + rowAdvance);
			if (columnAdvance == 0 && move.isInBounds()) {
				movesetAdvance.push_back(move);
			}
			else if (columnAdvance != 0 && move.isInBounds()) {
				movesetCapture.push_back(move);
			}
		}

		if (position.getRow() == startingRow) {
			movesetAdvance.emplace_back(position.getColumn(), position.getRow() + (2 * rowAdvance));
		}

		fullMoveRange.push_back(movesetAdvance);
		fullMoveRange.push_back(movesetCapture);
	}



	void ChessPiece::determineFullQueenRange() {
		fullMoveRange.clear();
		updateVerticalHorizonalMoves();
		updateDiagonalMoves();
	}



	void ChessPiece::determineFullRookRange() {
		fullMoveRange.clear();
		updateVerticalHorizonalMoves();
	}



	void ChessPiece::updateVerticalHorizonalMoves() {
		updateMovesetRight();
		updateMovesetLeft();
		updateMovesetUp();
		updateMovesetDown();
	}



	void ChessPiece::updateDiagonalMoves() {
		updateMovesetUpRight();
		updateMovesetUpLeft();
		updateMovesetDownRight();
		updateMovesetDownLeft();
	}



	void ChessPiece::updateMovesetRight() {
		std::vector<Position> moveset;
		for (int column1 = position.getColumn() + 1; column1 <= position.UPPER_COLUMN_BOUND; column1++) {
			moveset.emplace_back(column1, position.getRow());
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::updateMovesetLeft() {
		std::vector<Position> moveset;
		for (int column2 = position.getColumn() - 1; column2 >= position.LOWER_COLUMN_BOUND; column2--) {
			moveset.emplace_back(column2, position.getRow());
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::updateMovesetUp() {
		std::vector<Position> moveset;
		for (int row1 = position.getRow() + 1; row1 <= position.UPPER_ROW_BOUND; row1++) {
			moveset.emplace_back(position.getColumn(), row1);
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::updateMovesetDown() {
		std::vector<Position> moveset;
		for (int row2 = position.getRow() - 1; row2 >= position.LOWER_ROW_BOUND; row2--) {
			moveset.emplace_back(position.getColumn(), row2);
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::updateMovesetUpRight() {
		std::vector<Position> moveset;
		for (int column = position.getColumn() + 1, row = position.getRow() + 1;
			column <= position.UPPER_COLUMN_BOUND && row <= position.UPPER_ROW_BOUND;
			column++, row++) {
			moveset.emplace_back(column, row);
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::updateMovesetUpLeft() {
		std::vector<Position> moveset;
		for (int column = position.getColumn() - 1, row = position.getRow() + 1;
			column >= position.LOWER_COLUMN_BOUND && row <= position.UPPER_ROW_BOUND;
			column--, row++) {

			moveset.emplace_back(column, row);
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::updateMovesetDownRight() {
		std::vector<Position> moveset;
		for (int column = position.getColumn() + 1, row = position.getRow() - 1;
			column <= position.UPPER_COLUMN_BOUND && row >= position.LOWER_ROW_BOUND;
			column++, row--) {

			moveset.emplace_back(column, row);
		}
		fullMoveRange.push_back(moveset);
	}



	void ChessPiece::updateMovesetDownLeft() {
		std::vector<Position> moveset;
		for (int column = position.getColumn() - 1, row = position.getRow() - 1;
			column >= position.LOWER_COLUMN_BOUND && row >= position.LOWER_ROW_BOUND;
			column--, row--) {

			moveset.emplace_back(column, row);
		}
		fullMoveRange.push_back(moveset);
	}




}