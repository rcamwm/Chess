#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <map>
#include <set>

#include "ChessPiece.h"

namespace chess {
	struct LastMove {
		Position initial;
		Position final;
		PieceType pieceType = PieceType::Queen;
		Color pieceColor = Color::black;
	};

	class ChessBoard {
	public:
		ChessBoard(Color givenColor = Color::white);

		void addPieceToBoard(const int& initialColumn, const int& initialRow, const PieceType& givenType, const Color& givenColor);
		void removePieceFromBoard(const Position& piecePosition);
		void clearBoard();

		void updateBoardLegalMoves();
		bool movePiece(const Position& pieceInitialPosition, const Position& pieceDestination, const bool& computer = 0);
		//bool updateState(const Position& pieceInitialPosition, const Position& pieceDestination, const bool& computer = 0);
		void promoteLastMovedPiece(const PieceType& newType);
		
		LastMove getLastMove() const;
		std::vector<Position> getPieceLocations() const;
		ChessPiece getPiece(const Position& position) const;
		int getNumberOfPiecesOnBoard() const;

		bool isSpaceEmpty(const Position& space) const;
		bool isInCheck(const Color& checkKingColor) const;
		bool canPromoteLastMovedPawn();
		bool areMovesAvailable(const Color& movingColor);

		static Color getOppositeColor(const Color& originalColor);

		void printKingDangerZone(const Color& kingColor);	

	private:
		std::map<Position, ChessPiece> piecesOnBoard;
		std::vector<Position>pieceLocations;
		Color playerOneColor;

		std::map<Color, Position> kingMap;
		std::set<Position> whiteKingDangerZone;
		std::set<Position> blackKingDangerZone;
		bool whiteInCheck = 0;
		bool blackInCheck = 0;
		void setCheck(const Color& checkKingColor, const bool& inCheck);
		bool isInKingDangerZone(const Color& kingColor, const Position& space) const;
		void updateKingDangerZone(const Color& movingColor, const Position& forbiddenSpace);
		
		LastMove lastMove;
		std::vector<Position> findLegalMoves(const Position position);
		void updateLegalMoveVectorPawn(const Position& movingPiece,
			const std::vector<std::vector<Position>>& moveRange,
			const std::vector<Position>& occupiedRange,
			std::vector<Position>& legalMoves);
		bool canEnPessant(const Position& attackingPawn) const;
		void updateLegalMoveVectorKnight(const Position& movingPiece,
			const std::vector<Position>& moveRange,
			const std::vector<Position>& occupiedRange,
			std::vector<Position>& legalMoves);
		int updateLegalMoveVectorBishopRookQueen(const Position& movingPiece,
			const std::vector<Position>& moveRange,
			const std::vector<Position>& occupiedRange,
			std::vector<Position>& legalMoves);
		void updateLegalMoveVectorKing(const Position& movingPiece,
			const std::vector<std::vector<Position>>& moveRange,
			const std::vector<Position>& occupiedRange,
			std::vector<Position>& legalMoves);
		bool castleKing(const Position& initialPosition, const Position& destination, const bool isToTheRight);
	};
}

#endif