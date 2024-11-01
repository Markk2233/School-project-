#include <iostream>
#include <algorithm> 
#include <string>

using namespace std;

class Board {
public:

  static const int BOARD_SIZE = 4;
  char playing_board[BOARD_SIZE][BOARD_SIZE];

  bool playerX = true;

  int num = 0;
  int num1 = 0;
  void StartGame() {
    for (int i = 0; i < BOARD_SIZE; i++) {
      num1 += 1;
      for (int j = 0; j < BOARD_SIZE; j++) {
        num += 1;
        if (i == 0) {
          playing_board[i][j] = '0' + num;      
        }
        else if (j == 0) {
          playing_board[i][j] = '0' + num1;
        }
        else {
          playing_board[i][j] = ' ';
        }
        cout << playing_board[i][j] << " ";
      }
      cout << endl;
    }

    cout << endl;
  }

  int CheckStatus() {
    int FreeSpaces = 9;

    for (int i = 1; i < BOARD_SIZE; i++) {
      for (int j = 1; j < BOARD_SIZE; j++) {
        if (playing_board[i][j] != ' ') {
          FreeSpaces--;
        }
      }
    }
    return FreeSpaces;
  }

  void PrintBoard() {

    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        cout << playing_board[i][j] << " ";
      }
      cout << endl;
    }
    cout << endl;
  }

  void MakeMove() {

    int i;
    int j;

    if (playerX) {
      cout << "1 player makes move " << endl;
    }
    else {
      cout << "2 player makes move " << endl;
    }

    cout << "coordinates for row: ";
    cin >> i;
    cout << "coordinates for column: ";
    cin >> j;
    cout << endl;
 
    if (playerX && playing_board[i-1][j-1] == ' ') {
      playing_board[i - 1][j - 1] = 'X';
      playerX = false;
    }
    else if (!playerX && playing_board[i-1][j-1] == ' ') {
      playing_board[i - 1][j - 1] = '0';
      playerX = true;
    }
    else {
      cout << "this cell is already occupied" << endl << endl;
    }

    PrintBoard();

  }

  char CheckWin() {

    for (int i = 1; i < BOARD_SIZE; i++) {
      if (playing_board[i][1] != ' ' && playing_board[i][1] == playing_board[i][2] && playing_board[i][1] == playing_board[i][3]) {
        return playing_board[i][1];
      }
    }
    for (int j = 1; j < BOARD_SIZE; j++) {
      if (playing_board[1][j] != ' ' && playing_board[1][j] == playing_board[2][j] && playing_board[1][j] == playing_board[3][j]) {
        return playing_board[1][j];
      }
    }

    if (playing_board[2][2] != ' ' && playing_board[1][1] == playing_board[2][2] && playing_board[1][1] == playing_board[3][3]) {
      return playing_board[0][0];
    }
    else if (playing_board[2][2] != ' ' && playing_board[3][1] == playing_board[2][2] && playing_board[3][1] == playing_board[1][3]) {
      return playing_board[0][1];
    }

    return ' ';
  } 

};

int main()
{
  Board board1;

  cout << "Starting game" << endl;

    board1.StartGame();

    char winner = ' ';

  while (winner == ' ' && board1.CheckStatus() != 0) {
    board1.MakeMove();
    winner = board1.CheckWin();

    if (winner != ' '  && !board1.playerX) {
      cout << "Wir haben einen Gewinner: 1 Spieler\a" << endl;
      break;
    } else if (winner != ' ' && board1.playerX) {
      cout << "Wir haben einen Gewinner: 2 Spieler\a" << endl;
      break;
    }
    else if (board1.CheckStatus() == 0) {
      cout << "DRAW!\a";
    }

  }

}
