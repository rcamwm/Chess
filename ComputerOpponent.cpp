#include "ComputerOpponent.h"

namespace chess {
	ComputerOpponent::ComputerOpponent(const ChessBoard& rootBoard, const Color& computerPieceColor) {
		root = new Node(rootBoard);
		computerColor = computerPieceColor;
		initialDepth = 3;
	}



	std::pair<Position, Position> ComputerOpponent::getNextMove() {
		std::pair<Position, Position> nextMove;
		int ideal = kingCrimson(root, initialDepth, true, LOSE_VALUE, WIN_VALUE);
		for (auto child = root->children.begin(); child != root->children.end(); ++child) {
			if (ideal == (*child)->value) {
				LastMove idealMove = (*child)->board.getLastMove();
				nextMove.first = idealMove.initial;
				nextMove.second = idealMove.final;
				break;
			}
		}
		return nextMove;
	}


	
	Color ComputerOpponent::getMovingColor(const bool& isComputer) {
		if (isComputer) {
			return computerColor;
		}
		else {
			return ChessBoard::getOppositeColor(computerColor);
		}
	}



	int ComputerOpponent::kingCrimson(ComputerOpponent::Node* node, const int& depth, const bool& isComputer, int lowPrune, int highPrune) {
		if (depth == 0) {
			node->value = calculateBoardValue(node->board);
		}
		else if (isComputer) {
			int max = LOSE_VALUE;
			std::vector<std::pair<Position, Position>> allMoves = getPossibleMovesThisTurn(node->board, isComputer);
			for (auto move = allMoves.begin(); move != allMoves.end(); ++move) {
				Node *child = new Node(node->board);
				if (child->updateNode(move->first, move->second)) {
					node->children.push_back(child);
					int eval = kingCrimson(child, depth - 1, not isComputer, lowPrune, highPrune);
					max = maximumValue(max, eval);
					lowPrune = maximumValue(lowPrune, eval);
					if (highPrune <= lowPrune) {
						break;
					}
				}
			}
			if (max == LOSE_VALUE && node->board.isInCheck(getMovingColor(not isComputer))) {
				std::cout << "is it working";
				max -= CHECKMATE_VALUE;
			}
			node->value = max;
		}
		else {
			int min = WIN_VALUE;
			std::vector<std::pair<Position, Position>> allMoves = getPossibleMovesThisTurn(node->board, isComputer);
			for (auto move = allMoves.begin(); move != allMoves.end(); ++move) {
				Node* child = new Node(node->board);
				if (child->updateNode(move->first, move->second)) {
					node->children.push_back(child);
					int eval = kingCrimson(child, depth - 1, not isComputer, lowPrune, highPrune);
					min = minimumValue(min, eval);
					highPrune = minimumValue(lowPrune, eval);
					if (highPrune <= lowPrune) {
						break;
					}
				}
			}
			if (min == WIN_VALUE) {
				std::cout << "is it working";
				min += CHECKMATE_VALUE;
			}
			node->value = min;
		}
		return node->value;
	}



	std::vector<std::pair<Position, Position>> ComputerOpponent::getPossibleMovesThisTurn(const ChessBoard& board, const bool& isComputer) {
		std::vector<std::pair<Position, Position>> allMoves;
		std::vector<Position> allPieceLocations = board.getPieceLocations();
		for (auto piece = allPieceLocations.begin(); piece != allPieceLocations.end(); ++piece) {
			ChessPiece arbitraryPiece = board.getPiece(*piece);
			std::vector<Position> allLegalMoves = arbitraryPiece.getLegalMoves();
			if (allLegalMoves.size() > 0 && arbitraryPiece.getPieceColor() == getMovingColor(isComputer)) {
				std::pair<Position, Position> initialFinal;
				initialFinal.first = *piece;
				for (auto move = allLegalMoves.begin(); move != allLegalMoves.end(); ++move) {
					initialFinal.second = *move;
					allMoves.push_back(initialFinal);
				}
			}
		}
		return allMoves;
	}



	int ComputerOpponent::calculateBoardValue(const ChessBoard& board) {
		int boardValue = 0;
		std::vector<Position> positions = board.getPieceLocations();
		for (int index = 0; index < static_cast<int>(positions.size()); index++) {
			int addValue = getPieceValue(board.getPiece(positions[index]).getPieceType());
			int multiplier = getMultiplier(board.getPiece(positions[index]).getPieceColor());
			boardValue += (addValue * multiplier);
		}
		return boardValue;
	}



	int ComputerOpponent::getPieceValue(const PieceType& type) {
		switch (type) {
		case PieceType::Pawn:
			return 20;
		case PieceType::Bishop:
			return 180;
		case PieceType::Knight:
			return 180;
		case PieceType::Rook:
			return 700;
		case PieceType::Queen:
			return 1000;
		case PieceType::King:
			return 1000;
		default:
			return 0;
		}
	}



	int ComputerOpponent::getMultiplier(const Color& pieceColor) {
		if (pieceColor == computerColor) {
			return 1;
		}
		else {
			return -1;
		}
	}



	int ComputerOpponent::maximumValue(const int& comparandOne, const int& comparandTwo) {
		if (comparandOne > comparandTwo) {
			return comparandOne;
		}
		else {
			return comparandTwo;
		}
	}



	int ComputerOpponent::minimumValue(const int& comparandOne, const int& comparandTwo) {
		if (comparandOne < comparandTwo) {
			return comparandOne;
		}
		else {
			return comparandTwo;
		}
	}


	/*int ComputerOpponent::maximumValue(const std::vector<int> values) {
		auto it = values.begin();
		int max = *it;
		++it;
		for (it; it != values.end(); ++it) {
			if (*it > max) {
				max = *it;
			}
		}
		return max;
	}



	int ComputerOpponent::minimumValue(const std::vector<int> values) {
		auto it = values.begin();
		int min = *it;
		++it;
		for (it; it != values.end(); ++it) {
			if (*it < min) {
				min = *it;
			}
		}
		return min;
	}*/


}