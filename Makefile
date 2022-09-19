CC = g++ 
CFLAGS = -g -std=c++11
# LINKFLAGS = -lrt

all : Chess

Position : Position.cpp Position.h
	$(CC) $(CFLAGS) -c Position.cpp -o Position
ChessPiece : ChessPiece.cpp ChessPiece.h Position Position.h
	$(CC) $(CFLAGS) -c ChessPiece.cpp -o ChessPiece
ChessBoard : ChessBoard.cpp ChessBoard.h ChessPiece ChessPiece.h Position Position.h
	$(CC) $(CFLAGS) -c ChessBoard.cpp -o ChessBoard
ComputerOpponent : ComputerOpponent.cpp ComputerOpponent.h ChessBoard ChessBoard.h ChessPiece ChessPiece.h Position Position.h
	$(CC) $(CFLAGS) -c ComputerOpponent.cpp -o ComputerOpponent
ChessGame : ChessGame.cpp ChessGame.h ComputerOpponent ComputerOpponent.h ChessBoard ChessBoard.h ChessPiece ChessPiece.h Position Position.h
	$(CC) $(CFLAGS) -c ChessGame.cpp -o ChessGame
Chess : Chess.cpp ChessGame ChessGame.h ComputerOpponent ComputerOpponent.h ChessBoard ChessBoard.h ChessPiece ChessPiece.h Position Position.h
	$(CC) $(CFLAGS) Chess.cpp -o Chess ChessGame ComputerOpponent ChessBoard ChessPiece Position


