#include "ChessGame.h"

namespace chess {
	bool ChessGame::willContinuePlaying = false;

	ChessGame::ChessGame() {
		Color player1Color = decidePlayer1Color();

		ChessBoard board(player1Color);
        
		standardGameSetup(board, player1Color);
		playerVsComputer(board, player1Color);
        //sandbox(board);
		
        willContinuePlaying = decideToContinuePlaying();
	}



	bool ChessGame::getContinuePlaying() {
		return willContinuePlaying;
	}
	
	
	
	Color ChessGame::decidePlayer1Color() {
		Color colorSelect;
		int selection = 0;
		while (selection == 0) {
            //system("cls"); // Windows
            system("clear"); // Mac
			cout << "Select Player One color." << endl;
			cout << "1. White" << endl;
			cout << "2. Black" << endl;
			cout << "Enter a single integer: ";
			cin >> selection;
			cin.ignore();

			switch (selection) {
			case 1:
				colorSelect = Color::white;
				break;
			case 2:
				colorSelect = Color::black;
				break;
			default:
				selection = 0;
				break;
			}
		}
		return colorSelect;
	}



	PieceType ChessGame::decideUpgradedPawn() {
		PieceType newType;
		int selection = 0;
		while (selection == 0) {
			cout << "Select which piece to upgrade your pawn to" << endl;
			cout << "1. Bishop" << endl;
			cout << "2. Knight" << endl;
			cout << "3. Rook" << endl;
			cout << "4. Queen" << endl;
			cout << "Enter a single integer: ";
			cin >> selection;
			cin.ignore();

			switch (selection) {
			case 1:
				newType = PieceType::Bishop;
				break;
			case 2:
				newType = PieceType::Knight;
				break;
			case 3:
				newType = PieceType::Rook;
				break;
			case 4:
				newType = PieceType::Queen;
				break;
			default:
				selection = 0;
				break;
			}
		}
		return newType;
	}



	bool ChessGame::decideToContinuePlaying() {
		bool continuePlaying;
		int selection = 0;
		while (selection == 0) {
            //system("cls"); // Windows
            system("clear"); // Mac
			cout << "Would you like to play again?" << endl;
			cout << "1. Yes" << endl;
			cout << "2. No" << endl;
			cout << "Enter a single integer: ";
			cin >> selection;
			cin.ignore();

			switch (selection) {
			case 1:
				continuePlaying = true;
				break;
			case 2:
				continuePlaying = false;
				break;
			default:
				selection = 0;
				break;
			}
		}
		return continuePlaying;
	}



	void ChessGame::standardGameSetup(ChessBoard& board, const Color& player1Color) {
		Color player2Color = ChessBoard::getOppositeColor(player1Color);
		Color colorArray[2] = { player1Color, player2Color };
		int rowArray[2] = { Position::LOWER_ROW_BOUND, Position::UPPER_ROW_BOUND };

		int k, q;
		if (player1Color == Color::white) { q = 4; k = 5; }
		else { k = 4; q = 5; }

		for (int arrayIndex = 0; arrayIndex < 2; arrayIndex++) {
			board.addPieceToBoard(1, rowArray[arrayIndex], PieceType::Rook, colorArray[arrayIndex]);
			board.addPieceToBoard(2, rowArray[arrayIndex], PieceType::Knight, colorArray[arrayIndex]);
			board.addPieceToBoard(3, rowArray[arrayIndex], PieceType::Bishop, colorArray[arrayIndex]);
			board.addPieceToBoard(q, rowArray[arrayIndex], PieceType::Queen, colorArray[arrayIndex]);
			board.addPieceToBoard(k, rowArray[arrayIndex], PieceType::King, colorArray[arrayIndex]);
			board.addPieceToBoard(6, rowArray[arrayIndex], PieceType::Bishop, colorArray[arrayIndex]);
			board.addPieceToBoard(7, rowArray[arrayIndex], PieceType::Knight, colorArray[arrayIndex]);
			board.addPieceToBoard(8, rowArray[arrayIndex], PieceType::Rook, colorArray[arrayIndex]);
		}

		for (int pawn = Position::LOWER_COLUMN_BOUND; pawn <= Position::UPPER_COLUMN_BOUND; pawn++) {
			board.addPieceToBoard(pawn, Position::LOWER_ROW_BOUND + 1, PieceType::Pawn, player1Color);
			board.addPieceToBoard(pawn, Position::UPPER_ROW_BOUND - 1, PieceType::Pawn, player2Color);
		}

		board.updateBoardLegalMoves();
	}



	void ChessGame::playerVsComputer(ChessBoard& board, const Color& player1Color) {
		bool continueGame = true;
		bool whiteCanContinue = true;
		bool blackCanContinue = true;

		bool whiteIsHuman;
		if (player1Color == Color::white) {
			whiteIsHuman = true;
		}
		else {
			whiteIsHuman = false;
		}
		bool blackIsHuman = not whiteIsHuman;

		while (continueGame) {
			if (whiteCanContinue) {
				playerOrComputerMove(board, Color::white, whiteIsHuman);
				blackCanContinue = board.areMovesAvailable(Color::black);
			}
			else {
				printBoard(board);
				continueGame = false;
				checkmateStalemateOutput(board, Color::white);
				break;
			}

			if (blackCanContinue) {
				playerOrComputerMove(board, Color::black, blackIsHuman);
				whiteCanContinue = board.areMovesAvailable(Color::white);
			}
			else {
				printBoard(board);
				continueGame = false;
				checkmateStalemateOutput(board, Color::black);
				break;
			}
		}
		cout << "Press enter to continue.";
		cin.ignore();
		cin.get();
	}



	void ChessGame::sandbox(ChessBoard& board) {
		printBoard(board);
		char currentPositionInput[3];
		char destinationInput[3];
		Position exitCode(-69, 69);
		bool cont = true;
		while (cont) {
			cout << endl << "Enter 0 twice to exit sandbox mode." << endl;

			cout << "Which piece would you like to move? ";
			cin >> currentPositionInput;
			Position currentPosition = getLetterPosition(currentPositionInput, 1);

			cout << "Where would you like to move it? ";
			cin >> destinationInput;
			Position destination = getLetterPosition(destinationInput, 1);

			if (currentPosition == exitCode && destination == exitCode) {
				cont = false;
			}

			if (not board.isSpaceEmpty(currentPosition) && board.movePiece(currentPosition, destination)) {

				if (board.canPromoteLastMovedPawn()) {
					board.promoteLastMovedPiece(decideUpgradedPawn());
				}

				printBoard(board);

				
				if (board.isInCheck(Color::white)) {
					cout << "White king in check!" << endl;
				}
				if (board.isInCheck(Color::black)) {
					cout << "Black king in check!" <<endl;
				}
			}
		}
	}



	void ChessGame::humanPlayerMove(ChessBoard& board, const Color& playerColor) {
		bool loop = true;
		while (loop) {
			char currentPositionInput[3];
			char destinationInput[3];

			printBoard(board);
			cout << getColorString(playerColor) << "'s turn." << endl;
			if (board.isInCheck(playerColor)) {
				cout << getColorString(playerColor) << " king in check!" << endl;
			}

			cout << "Which piece would you like to move? ";
			cin >> currentPositionInput;
			cin.sync();
			Position currentPosition = getLetterPosition(currentPositionInput);

			cout << "Where would you like to move it? ";
			cin >> destinationInput;
			cin.sync();
			Position destination = getLetterPosition(destinationInput);

			if (not board.isSpaceEmpty(currentPosition)
				&& board.getPiece(currentPosition).getPieceColor() == playerColor
				&& board.movePiece(currentPosition, destination)) {

				if (board.canPromoteLastMovedPawn()) {
					board.promoteLastMovedPiece(decideUpgradedPawn());
				}
				loop = false;
			}
		}
	}



	void ChessGame::computerMove(ChessBoard& board, const Color& computerColor) {
		ComputerOpponent opponent(board, computerColor);
		printBoard(board);
		cout << getColorString(computerColor) << "'s turn." << endl;
		if (board.isInCheck(computerColor)) {
			cout << getColorString(computerColor) << " king in check!" << endl;
		}
		std::pair<Position, Position> computerMove = opponent.getNextMove();
		board.movePiece(computerMove.first, computerMove.second);
		if (board.canPromoteLastMovedPawn()) {
			board.promoteLastMovedPiece(PieceType::Queen);
		}
		cout << "Press enter to continue.";
		cin.get();
	}



	void ChessGame::playerOrComputerMove(ChessBoard& board, const Color& moveColor, const bool& isHuman) {
		if (isHuman) {
			humanPlayerMove(board, moveColor);
			cin.ignore();
		}
		else {
			computerMove(board, moveColor);
		}
	}
	
	
	
	string ChessGame::getColorString(const Color& color) {
		if (color == Color::white) {
			return "White";
		}
		else {
			return "Black";
		}
	}



	void ChessGame::printBoard(ChessBoard& board) {
		//system("cls"); // Windows
        system("clear"); // Mac
		std::vector<Position> pieceLocations = board.getPieceLocations();

		std::vector<Position>::iterator it;
		for (int row = Position::UPPER_ROW_BOUND; row >= Position::LOWER_ROW_BOUND - 1; row--) {
			if (row > Position::LOWER_ROW_BOUND - 1) {
				cout << row << " ";
			}
			else {
				cout << "  ";
			}

			for (int column = Position::LOWER_COLUMN_BOUND; column <= Position::UPPER_COLUMN_BOUND; column++) {
				Position check(column, row);
				if (row > Position::LOWER_ROW_BOUND - 1) {
					it = find(pieceLocations.begin(), pieceLocations.end(), check);
					if (it != pieceLocations.end()) {
						if (board.getPiece(*it).getPieceColor() == Color::black) {
							cout << "|B";
						}
						else {
							cout << "|W";
						}

						PieceType pieceType = board.getPiece(check).getPieceType();
						switch (pieceType) {
						case PieceType::Pawn:
							cout << "p";
							break;
						case PieceType::Bishop:
							cout << "B";
							break;
						case PieceType::Knight:
							cout << "N";
							break;
						case PieceType::Rook:
							cout << "R";
							break;
						case PieceType::Queen:
							cout << "Q";
							break;
						case PieceType::King:
							cout << "K";
							break;
						default:
							break;
						}
					}
					else {
						if ((column + row) % 2 == 0) {
							cout << "|[]";
						}
						else {
							cout << "|  ";
						}
					}
				}
				else {
					cout << " " << getColumnLetter(column) << " ";
				}
			}
			if (row > Position::LOWER_COLUMN_BOUND - 1) {
				cout << "|" << std::endl;
			}
			else { cout << std::endl; }
		}
	}



	Position ChessGame::getLetterPosition(const char* coordinate, const bool& canExit) {
		if (canExit && coordinate[0] == '0') {
			Position exitCode(-69, 69);
			return exitCode;
		}

		char colChar = coordinate[0];
		int col = getColumnNumber(colChar);
		int row = coordinate[1] - '0';

		Position position(col, row);
		return position;
	}



	int ChessGame::getColumnNumber(const char& letter) {
		char upp = std::toupper(letter);
		switch (upp) {
		case 'A':
			return 1;
			break;
		case 'B':
			return 2;
			break;
		case 'C':
			return 3;
			break;
		case 'D':
			return 4;
			break;
		case 'E':
			return 5;
			break;
		case 'F':
			return 6;
			break;
		case 'G':
			return 7;
			break;
		case 'H':
			return 8;
			break;
		default:
			return 0;
		}
	}



	char ChessGame::getColumnLetter(const int& column) {
		switch (column) {
		case 1:
			return 'A';
			break;
		case 2:
			return 'B';
			break;
		case 3:
			return 'C';
			break;
		case 4:
			return 'D';
			break;
		case 5:
			return 'E';
			break;
		case 6:
			return 'F';
			break;
		case 7:
			return 'G';
			break;
		case 8:
			return 'H';
			break;
		default:
			return ' ';
			break;
		}
	}



	void ChessGame::checkmateStalemateOutput(const ChessBoard& board, const Color& player) {
		if (board.isInCheck(player)) {
			cout << getColorString(player) << " checkmate!" << endl;
		}
		else {
			cout << getColorString(player) << " stalemate!" << endl;
		}
	}

}
