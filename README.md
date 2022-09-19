# Chess

## About 
Command line interface chess game I programmed from scratch as a personal project during summer 2021.  
The computer opponent decides its moves with a Minimax algorithm.

## How to Install and Run
Run the following commands:
```bash
make
./Chess
```

## How to Play
Select which piece you'd like to move by entering a letter from A-H and a number from 1-8.  
Do the same with where you would like to move it to.

For example, if you're playing as White and enter ```f1```, followed by ```c4```
```
8 |BR|BN|BB|BQ|BK|BB|  |BR|
7 |Bp|Bp|Bp|Bp|Bp|Bp|Bp|Bp|
6 |  |[]|  |[]|  |BN|  |[]|
5 |[]|  |[]|  |[]|  |[]|  |
4 |  |[]|  |[]|Wp|[]|  |[]|
3 |[]|  |[]|  |[]|  |[]|  |
2 |Wp|Wp|Wp|Wp|  |Wp|Wp|Wp|
1 |WR|WN|WB|WQ|WK|WB|WN|WR|
   A  B  C  D  E  F  G  H 
White's turn.
Which piece would you like to move? f1
Where would you like to move it? c4
```
Then the piece at ```F1``` (a Bishop) will move to position ```C4```.
```
8 |BR|BN|BB|BQ|BK|BB|  |BR|
7 |Bp|Bp|Bp|Bp|Bp|Bp|Bp|Bp|
6 |  |[]|  |[]|  |BN|  |[]|
5 |[]|  |[]|  |[]|  |[]|  |
4 |  |[]|WB|[]|Wp|[]|  |[]|
3 |[]|  |[]|  |[]|  |[]|  |
2 |Wp|Wp|Wp|Wp|  |Wp|Wp|Wp|
1 |WR|WN|WB|WQ|WK|  |WN|WR|
   A  B  C  D  E  F  G  H 
Black's turn.
Press enter to continue.
```
*note: commands are case insensitive.* 

## Symbol Guide:
* B = Black 
* W = White
* p = Pawn 
* R = Rook
* N = Knight
* B = Bishop
* Q = Queen
* K = King
* [] indicates a black tile 
