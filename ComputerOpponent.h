#ifndef COMPUTER_OPPONENT_H
#define COMPUTER_OPPONENT_H

#include <cstdlib>
#include <ctime>
#include <limits>

#include "ChessBoard.h"

namespace chess {
	

	class ComputerOpponent {
	public:
		ComputerOpponent(const ChessBoard& rootBoard, const Color& computerPieceColor);
		~ComputerOpponent() { delete root; }
		std::pair<Position, Position> getNextMove();

		static const int CHECKMATE_VALUE = 5000;
		static const int LOSE_VALUE = std::numeric_limits<int>::min() + CHECKMATE_VALUE;
		static const int WIN_VALUE = std::numeric_limits<int>::max() - CHECKMATE_VALUE;

	private:
		struct Node {
			ChessBoard board;
			std::vector<Node*> children;
			int value;

			Node(const ChessBoard& boardState) { board = boardState; }
			~Node() { for (auto it = children.begin(); it != children.end(); ++it) { delete* it; *it = nullptr; } }
			bool updateNode(const Position& initial, const Position & final) { return board.movePiece(initial, final, 1); }
		}* root = nullptr;
		
		Color computerColor;
		Color getMovingColor(const bool& isComputer);

		int initialDepth;
		int kingCrimson(Node* node, const int& depth, const bool& isComputer, int lowPrune, int highPrune);
		std::vector<std::pair<Position, Position>> getPossibleMovesThisTurn(const ChessBoard& board, const bool& isComputer);

		int calculateBoardValue(const ChessBoard& board);
		int getPieceValue(const PieceType& type);
		int getMultiplier(const Color& pieceColor);

		int maximumValue(const int& comparandOne, const int& comparandTwo);
		int minimumValue(const int& comparandOne, const int& comparandTwo);
	};
}

#endif
