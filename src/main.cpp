#include "../utils.hpp"
#include "drivers/guiDriver.hpp"
#include "tests/perft.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <bits/stdc++.h>
#include <cassert>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) { // 2, 1, c -> completely manual, manual 1
                                   // player or computer vs computer
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
  //  r3k1r1/Rppp1ppp/1b3nbN/nPB5/B1P1P3/q4N2/Pp1P2PP/R2Q1RK1 w q - 0 1 //after
  //  h8g8
  if (*argv[1] == '2') {
    guiDriver gui(fen);
    gui.play();
  } else if (*argv[1] == '1') {
    // gui play against computer
  } else if (*argv[1] == 'p') {

    // Tests::testCases();
    // do perft testing
    Engine::Board perftBoard(fen); // initial start position
    // int depth = *argv[2] - '0';
    for (int d = 1; d <= 4; d++) {
      clock_t start, end;
      start = clock();

      // unsync the I/O of C and C++.
      ios_base::sync_with_stdio(false);
      int nodes = Tests::perft(perftBoard, d);
      std::cout << YELLOW << nodes << RESET << std::endl;
      end = clock();
      double time_taken = double(end - start)/double(CLOCKS_PER_SEC);
      cout << "Time taken by d:" << d << " is " << fixed << time_taken
           << setprecision(5);
      cout << " sec " << endl;
    }

    // Tests::divide(depth, fen);
    // std::string str = utils::convertToChessNotation(19);
    // cout << str << endl;
  } else {
    cout << "Mode not accepted" << endl;
  }
}