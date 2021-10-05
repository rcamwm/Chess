#include <cmath>
#include "ChessBoard.h"

namespace chess {
	ChessBoard::ChessBoard(Color givenColor) {
		playerOneColor = givenColor;
		ChessPiece::setPlayerOneColor(playerOneColor);
	}



// ADD AND REMOVE PIECES FROM BOARD ######################################################################################
	


	void ChessBoard::addPieceToBoard(const int& initialColumn, const int& initialRow, const PieceType& givenType, const Color& givenColor) {

		Position position(initialColumn, initialRow);
		if (position.isInBounds()) {
			ChessPiece newPiece(position, givenType, givenColor);
			piecesOnBoard.emplace(position, newPiece);
			pieceLocations.push_back(position);
		}
		if (givenType == PieceType::King) {
			kingMap[givenColor] = position;
		}
	}



	void ChessBoard::removePieceFromBoard(const Position& piecePosition) {
		std::map<Position, ChessPiece>::iterator it;
		it = piecesOnBoard.find(piecePosition);
		if (it != piecesOnBoard.end()) {
			pieceLocations.erase(std::find(pieceLocations.begin(), pieceLocations.end(), piecePosition));
			piecesOnBoard.erase(it);
		}
	}



	void ChessBoard::clearBoard() {
		piecesOnBoard.clear();
		pieceLocations.clear();
	}



// MOVING PIECES AND UPDATED LEGAL MOVES ###########################################################################



	void ChessBoard::updateBoardLegalMoves() {
		whiteKingDangerZone.clear();
		blackKingDangerZone.clear();
		for (auto const& pair : piecesOnBoard) {
			piecesOnBoard.at(pair.first).updateLegalMoves(findLegalMoves(pair.first));
		}
		if (lastMove.pieceColor == Color::white) {
			setCheck(Color::black, isInCheck(Color::black));
		}
		else {
			setCheck(Color::white, isInCheck(Color::white));
		}
	}



	bool ChessBoard::movePiece(const Position& initialPosition, const Position& destination, const bool& computer) {

		LastMove lastMoveCopy = lastMove;
		std::map<Position, ChessPiece> boardCopy = piecesOnBoard;
		std::vector<Position> locationsCopy = pieceLocations;
		PieceType type = piecesOnBoard.at(initialPosition).getPieceType();
		Color color = piecesOnBoard.at(initialPosition).getPieceColor();

		if (type == PieceType::King
			&& (initialPosition.getColumn() + 2 == destination.getColumn() || initialPosition.getColumn() - 2 == destination.getColumn()) 
			&& piecesOnBoard.at(initialPosition).isMovePossible(destination)) {

			if (initialPosition.getColumn() + 2 == destination.getColumn() && castleKing(initialPosition, destination, 1)) {
				return true;
			}
			else if (initialPosition.getColumn() - 2 == destination.getColumn() && castleKing(initialPosition, destination, 0)) {
				return true;
			}
			else {
				lastMove = lastMoveCopy;
				piecesOnBoard = boardCopy;
				return false;
			}
			
		}
		else if (type == PieceType::Pawn
			&& destination.getColumn() != initialPosition.getColumn()
			&& isSpaceEmpty(destination)
			&& piecesOnBoard.at(initialPosition).isMovePossible(destination)) {

			removePieceFromBoard(lastMove.final);
			removePieceFromBoard(initialPosition);
			ChessPiece movedPiece(destination, type, color, 1);
			piecesOnBoard.emplace(destination, movedPiece);
			pieceLocations.push_back(destination);
		}
		else if (piecesOnBoard.at(initialPosition).isMovePossible(destination)) {

			removePieceFromBoard(destination);
			removePieceFromBoard(initialPosition);
			ChessPiece movedPiece(destination, type, color, 1);
			piecesOnBoard.emplace(destination, movedPiece);
			pieceLocations.push_back(destination);
			/*if (type == PieceType::Pawn && canPromotePawn(color, destination)) {
				promotePawn(color, destination);
			}*/
			if (type == PieceType::King) {
				kingMap[color] = destination;
			}
		}
		else {
			return false;
		}

		lastMove.initial = initialPosition;
		lastMove.final = destination;
		lastMove.pieceType = type;
		lastMove.pieceColor = color;

		updateBoardLegalMoves();
		if (isInCheck(lastMove.pieceColor)) {
			piecesOnBoard = boardCopy;
			pieceLocations = locationsCopy;
			lastMove = lastMoveCopy;
			if (type == PieceType::King) {
				kingMap[color] = initialPosition;
			}
			return false;
		}
		else {
			setCheck(color, 0);
			return true;
		}

	}



	void ChessBoard::promoteLastMovedPiece(const PieceType& newType) {
		ChessPiece newPiece(lastMove.final, newType, lastMove.pieceColor, 1);
		piecesOnBoard[lastMove.final] = newPiece;
		updateBoardLegalMoves();
	}
	

	
// ACCESSORS #############################################################################################################



	LastMove ChessBoard::getLastMove() const {
		return lastMove;
	}



	std::vector<Position> ChessBoard::getPieceLocations() const {
		return pieceLocations;
	}
	
	
	
	ChessPiece ChessBoard::getPiece(const Position& position) const {
		return piecesOnBoard.at(position);
	}



	int ChessBoard::getNumberOfPiecesOnBoard() const {
		return piecesOnBoard.size();
	}

	
	
// MISC ##################################################################################################################



	bool ChessBoard::isSpaceEmpty(const Position& space) const {
		if (piecesOnBoard.count(space) > 0) {
			return false;
		}
		else {
			return true;
		}
	}



	bool ChessBoard::isInCheck(const Color& checkKingColor) const {
		if (checkKingColor == Color::white && isInKingDangerZone(Color::white, kingMap.at(checkKingColor))) {
			return true;
		}
		else if (checkKingColor == Color::black && isInKingDangerZone(Color::black, kingMap.at(checkKingColor))) {
			return true;
		}
		return false;
	}



	bool ChessBoard::canPromoteLastMovedPawn() {
		if (lastMove.pieceType == PieceType::Pawn) {
			if (lastMove.pieceColor == playerOneColor && lastMove.final.getRow() == Position::UPPER_ROW_BOUND) {
				return true;
			}
			else if (lastMove.pieceColor != playerOneColor && lastMove.final.getRow() == Position::LOWER_ROW_BOUND) {
				return true;
			}
		}
		return false;
	}



	bool ChessBoard::areMovesAvailable(const Color& movingColor) {
		std::vector<Position>::iterator it = pieceLocations.begin();
		for (it; it != pieceLocations.end(); ++it) {
			ChessPiece piece = piecesOnBoard.at(*it);
			if (piece.getPieceColor() == movingColor) {
				std::vector<Position> legalMovesCopy = piece.getLegalMoves();
				for (int moveIndex = 0; moveIndex < static_cast<int>(legalMovesCopy.size()); moveIndex++) {
					ChessBoard boardCopy = *this;
					if (boardCopy.movePiece(*it, legalMovesCopy[moveIndex])) {
						return true;
					}
				}
			}
		}
		return false;
	}



	Color ChessBoard::getOppositeColor(const Color& originalColor) {
		if (originalColor == Color::white) {
			return Color::black;
		}
		else {
			return Color::white;
		}
	}



// DISPLAY FUNCTIONS FOR TESTING #########################################################################################



	void ChessBoard::printKingDangerZone(const Color& kingColor) {
		std::cout << "Forbidden spaces for the ";
		std::set<Position>::iterator it;
		if (kingColor == Color::white) {
			std::cout << "white king" << std::endl;
			for (it = whiteKingDangerZone.begin(); it != whiteKingDangerZone.end(); ++it) {
				it->printTest();
			}

		}
		else {
			std::cout << "black king" << std::endl;
			for (it = blackKingDangerZone.begin(); it != blackKingDangerZone.end(); ++it) {
				it->printTest();
			}
		}
	}



// CHECK AND KING DANGER ZONES - PRIVATE FUNCTIONS #######################################################################



	void ChessBoard::setCheck(const Color& checkKingColor, const bool& inCheck) {
		if (checkKingColor == Color::white) {
			whiteInCheck = inCheck;
		}
		else {
			blackInCheck = inCheck;
		}
	}



	bool ChessBoard::isInKingDangerZone(const Color& kingColor, const Position& space) const {
		std::set<Position>::iterator it;

		if (kingColor == Color::white) {
			for (it = whiteKingDangerZone.begin(); it != whiteKingDangerZone.end(); ++it) {
				if (*it == space) {
					return true;
				}
			}
		}
		else {
			for (it = blackKingDangerZone.begin(); it != blackKingDangerZone.end(); ++it) {
				if (*it == space) {
					return true;
				}
			}
		}
		return false;
	}


	
	void ChessBoard::updateKingDangerZone(const Color& movingColor, const Position& forbiddenSpace) {
		if (movingColor == Color::white) {
			blackKingDangerZone.insert(forbiddenSpace);
		}
		else {
			whiteKingDangerZone.insert(forbiddenSpace);
		}
	}



// PIECE MOVING - PRIVATE FUNCTIONS ######################################################################################



	std::vector<Position> ChessBoard::findLegalMoves(const Position position) {
		std::vector <Position> legalMoves;
		std::vector<std::vector<Position>> fullMoveRange = piecesOnBoard.at(position).getFullMoveRange();
		int moveTypes = static_cast<int>(fullMoveRange.size());
		PieceType pieceType = piecesOnBoard.at(position).getPieceType();


		std::vector <Position> occupiedTerritory;
		for (auto const& pair : piecesOnBoard) {
			if (pair.first != position) {
				occupiedTerritory.push_back(pair.first);
			}
		}

		switch (pieceType) {
		case (PieceType::Pawn):
			updateLegalMoveVectorPawn(position, fullMoveRange, occupiedTerritory, legalMoves);
			break;
		case (PieceType::Knight):
			updateLegalMoveVectorKnight(position, fullMoveRange[0], occupiedTerritory, legalMoves);
			break;
		case (PieceType::King):
			updateLegalMoveVectorKing(position, fullMoveRange, occupiedTerritory, legalMoves);
			break;
		default: 
			for (int moveIndex = 0; moveIndex < moveTypes; moveIndex++) {
				updateLegalMoveVectorBishopRookQueen(position, fullMoveRange[moveIndex], occupiedTerritory, legalMoves);
			}
			break;
		}

		return legalMoves;
	}



	void ChessBoard::updateLegalMoveVectorPawn(const Position& movingPiece,
		const std::vector<std::vector<Position>>& moveRange,
		const std::vector<Position>& occupiedRange,
		std::vector<Position>& legalMoves) {

		Color pawnColor = piecesOnBoard.at(movingPiece).getPieceColor();
		std::vector<Position> advancingMoves = moveRange[0];
		std::vector<Position> capturingMoves = moveRange[1];

		for (int advanceMove = 0; advanceMove < static_cast<int>(advancingMoves.size()); advanceMove++) {
			if (isSpaceEmpty(advancingMoves[advanceMove])) {
				if (advanceMove == 0) {
					legalMoves.push_back(advancingMoves[advanceMove]);
				}
				else if (advanceMove > 0 && isSpaceEmpty(advancingMoves[advanceMove - 1])) {
					legalMoves.push_back(advancingMoves[advanceMove]);
				}
			}
		}

		for (int captureMove = 0; captureMove < static_cast<int>(capturingMoves.size()); captureMove++) {
			updateKingDangerZone(pawnColor, capturingMoves[captureMove]);
			for (int occupiedSpace = 0; occupiedSpace < static_cast<int>(occupiedRange.size()); occupiedSpace++) {
				if (capturingMoves[captureMove] == occupiedRange[occupiedSpace] &&
					pawnColor != piecesOnBoard.at(occupiedRange[occupiedSpace]).getPieceColor()) {

					legalMoves.push_back(capturingMoves[captureMove]);

				}
			}
			if (capturingMoves[captureMove].getColumn() == lastMove.final.getColumn()
				&& canEnPessant(movingPiece)) {
				legalMoves.push_back(capturingMoves[captureMove]);
			}
		}
	}



	bool ChessBoard::canEnPessant(const Position& attackingPawn) const {
		bool isVulnerable;
		if (lastMove.pieceType == PieceType::Pawn && abs(lastMove.initial.getRow() - lastMove.final.getRow()) == 2) {
			isVulnerable = true;
		}
		else {
			return false;
		}

		if (isVulnerable
			&& abs(attackingPawn.getColumn() - lastMove.final.getColumn()) == 1
			&& attackingPawn.getRow() == lastMove.final.getRow()) {
			return true;
		}
		else {
			return false;
		}
	}
	

	
	void ChessBoard::updateLegalMoveVectorKnight(const Position& movingPiece,
		const std::vector<Position>& moveRange,
		const std::vector<Position>& occupiedRange,
		std::vector<Position>& legalMoves) {

		Color movingColor = piecesOnBoard.at(movingPiece).getPieceColor();

		for (int move = 0; move < static_cast<int>(moveRange.size()); move++) {
			for (int occupiedSpace = 0; occupiedSpace < static_cast<int>(occupiedRange.size()); occupiedSpace++) {
				if (moveRange[move] == occupiedRange[occupiedSpace]) {
					if (movingColor == piecesOnBoard.at(occupiedRange[occupiedSpace]).getPieceColor()
						|| piecesOnBoard.at(occupiedRange[occupiedSpace]).getPieceType() == PieceType::King) {
						updateKingDangerZone(movingColor, moveRange[move]);
					}
					else {
						legalMoves.push_back(moveRange[move]);
					}
				}
			}
			if (isSpaceEmpty(moveRange[move])) {
				legalMoves.push_back(moveRange[move]);
				updateKingDangerZone(movingColor, moveRange[move]);
			}
		}
	}



	int ChessBoard::updateLegalMoveVectorBishopRookQueen(const Position& movingPiece,
		const std::vector<Position>& moveRange,
		const std::vector<Position>& occupiedRange,
		std::vector<Position>& legalMoves) {

		Color movingColor = piecesOnBoard.at(movingPiece).getPieceColor();
		bool encounteredKing = false;

		for (int move = 0; move < static_cast<int>(moveRange.size()); move++) {
			for (int occupiedSpace = 0; occupiedSpace < static_cast<int>(occupiedRange.size()); occupiedSpace++) {
				if (moveRange[move] == occupiedRange[occupiedSpace]) {
					if (movingColor == piecesOnBoard.at(occupiedRange[occupiedSpace]).getPieceColor()) {
						updateKingDangerZone(movingColor, moveRange[move]);
						return 0;
					}
					else if (piecesOnBoard.at(occupiedRange[occupiedSpace]).getPieceType() == PieceType::King) {
						legalMoves.push_back(moveRange[move]);
						updateKingDangerZone(movingColor, moveRange[move]);
						encounteredKing = true;
					}
					else if (not encounteredKing) {
						legalMoves.push_back(moveRange[move]);
						return 0;
					}
					else {
						return 0;
					}
				}
			}
			legalMoves.push_back(moveRange[move]);
			updateKingDangerZone(movingColor, moveRange[move]);
		}
		return 0;
	}



	void ChessBoard::updateLegalMoveVectorKing(const Position& movingPiece,
		const std::vector<std::vector<Position>>& moveRange,
		const std::vector<Position>& occupiedRange,
		std::vector<Position>& legalMoves) {

		Color movingColor = piecesOnBoard.at(movingPiece).getPieceColor();
		for (int move = 0; move < static_cast<int>(moveRange[0].size()); move++) {
			for (int occupiedSpace = 0; occupiedSpace < static_cast<int>(occupiedRange.size()); occupiedSpace++) {
				if (moveRange[0][move] == occupiedRange[occupiedSpace]) {
					if (movingColor == piecesOnBoard.at(occupiedRange[occupiedSpace]).getPieceColor()
						|| piecesOnBoard.at(occupiedRange[occupiedSpace]).getPieceType() == PieceType::King) {
						updateKingDangerZone(movingColor, moveRange[0][move]);
					}
					else {
						legalMoves.push_back(moveRange[0][move]);
					}
				}
			}
			if (isSpaceEmpty(moveRange[0][move])) {
				legalMoves.push_back(moveRange[0][move]);
				updateKingDangerZone(movingColor, moveRange[0][move]);

			}
		}

		//castling
		if (not piecesOnBoard.at(movingPiece).getHasPieceMoved()) {
			Position castleRight1(movingPiece.getColumn() + 1, movingPiece.getRow());
			Position castleRight2(movingPiece.getColumn() + 2, movingPiece.getRow());
			Position rookRight(movingPiece.UPPER_COLUMN_BOUND, movingPiece.getRow());
			if (isSpaceEmpty(castleRight1) && isSpaceEmpty(castleRight2)
				&& not isSpaceEmpty(rookRight) && not piecesOnBoard.at(rookRight).getHasPieceMoved()) {
				legalMoves.push_back(castleRight2);
			}
			Position castleLeft1(movingPiece.getColumn() - 1, movingPiece.getRow());
			Position castleLeft2(movingPiece.getColumn() - 2, movingPiece.getRow());
			Position rookLeft(movingPiece.LOWER_COLUMN_BOUND, movingPiece.getRow());
			if (isSpaceEmpty(castleLeft1) && isSpaceEmpty(castleLeft2)
				&& not isSpaceEmpty(rookLeft) && not piecesOnBoard.at(rookLeft).getHasPieceMoved()) {
				legalMoves.push_back(castleLeft2);
			}
		}
	}



	bool ChessBoard::castleKing(const Position& initialPosition, const Position& destination, const bool isToTheRight) {
		Color color = piecesOnBoard.at(initialPosition).getPieceColor();
		int addTo;
		int rookColumn;
		if (isToTheRight) { 
			addTo = 1;
			rookColumn = Position::UPPER_COLUMN_BOUND;
		} 
		else { 
			addTo = -1; 
			rookColumn = Position::LOWER_COLUMN_BOUND;
		}

		Position nextTo(initialPosition.getColumn() + addTo, initialPosition.getRow());
		if (not isInKingDangerZone(color, initialPosition)
			&& movePiece(initialPosition, nextTo)
			&& movePiece(nextTo, destination)) {

			Position rookOriginalPosition(rookColumn, initialPosition.getRow());
			removePieceFromBoard(rookOriginalPosition);
			ChessPiece newRook(nextTo, PieceType::Rook, color, 1);
			piecesOnBoard[nextTo] = newRook;
			pieceLocations.push_back(nextTo);
			return true;
		}
		return false;
	}
}
