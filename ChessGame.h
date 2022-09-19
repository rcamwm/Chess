#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <cstdlib>
#include <iostream>
#include <string>
#include "ComputerOpponent.h"
using namespace std;

namespace chess {
	enum class PlayerModes { PvP, PvC, CvC, sandbox };

	class ChessGame {
	public:
		ChessGame();
		static bool getContinuePlaying();

	private:
		static bool willContinuePlaying;

		PlayerModes decidePlayerMode();
		Color decidePlayer1Color();
		bool decideToContinuePlaying();
		PieceType decideUpgradedPawn();
		
		void standardGameSetup(ChessBoard& board, const Color& player1Color);
		void playerVsPlayer(ChessBoard& board);
		void playerVsComputer(ChessBoard& board, const Color& player1Color);
		void computerVsComputer(ChessBoard& board);
		void sandbox(ChessBoard& board);

		void humanPlayerMove(ChessBoard& board, const Color& playerColor);
		void computerMove(ChessBoard& board, const Color& computerColor);
		void playerOrComputerMove(ChessBoard& board, const Color& moveColor, const bool& isPlayer);

		string getColorString(const Color& color);

		void printBoard(ChessBoard& board);
		Position getLetterPosition(const char* coordinate, const bool& canExit = 0);
		int getColumnNumber(const char& letter); // For inputting a letter A-H to represent the columns
		char getColumnLetter(const int& column); // the opposite ^ lol
		void checkmateStalemateOutput(const ChessBoard& board, const Color& player);
	};
}

#endif