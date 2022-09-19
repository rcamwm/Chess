#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <vector>
#include <memory>

#include "Position.h"

namespace chess {

	enum class Color { white, black };
	enum class PieceType { Pawn, Bishop, Knight, Rook, Queen, King };

	class ChessPiece {
	public:
		ChessPiece();
		ChessPiece(const int& initialColumn, const int& initialRow, const PieceType& givenType,
			const Color& givenColor, const bool& hasMoved = 0);
		ChessPiece(const Position& initialPosition, const PieceType& givenType,
			const Color& givenColor, const bool& hasMoved = 0);

		Position getPosition() const;
		PieceType getPieceType() const;
		Color getPieceColor() const;
		Color getOppositePieceColor() const;
		bool getHasPieceMoved() const;

		int getNumberOfMoveTypes() const;
		std::vector<std::vector<Position>> getFullMoveRange() const;
		std::vector<Position> getLegalMoves() const;

		bool isMovePossible(const Position& newPosition);
		void determineFullMoveRange();
		void updateLegalMoves(const std::vector<Position>& newMoves);

		void pawnPromotion(const PieceType& newType);

		static void setPlayerOneColor(const Color& player);

	private:
		Position position;
		PieceType type;
		Color color;
		bool hasPieceMoved;

		int numberOfMoveTypes;
		std::vector<std::vector<Position>> fullMoveRange;
		std::vector<Position> legalMoves;

		static Color playerOneColor;
		static bool isPlayerOneColorSet;

		void setNumberOfMoveTypes();

		void determineFullBishopRange();
		void determineFullKingRange();
		void determineFullKnightRange();
		void determineFullPawnRange();
		void determineFullQueenRange();
		void determineFullRookRange();

		void updateVerticalHorizonalMoves();
		void updateDiagonalMoves();
		void updateMovesetRight();
		void updateMovesetLeft();
		void updateMovesetUp();
		void updateMovesetDown();
		void updateMovesetUpRight();
		void updateMovesetUpLeft();
		void updateMovesetDownRight();
		void updateMovesetDownLeft();

	};
}

#endif