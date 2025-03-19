#include "../utils.hpp"
#include "./FixedStack.hpp"
#include "drivers/guiDriver.hpp"
#include "tests/perft.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <bits/stdc++.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;

int main(int argc, char *argv[]) { // 2, 1, c -> completely manual, manual 1
                                   // player or computer vs computer
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  std::ofstream debugFile("src/crashes/zobrist_debug.txt");

  // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
  // 3r4/3r4/3k4/8/8/3K4/8/8 w -- - 0 1
  //  r3k1r1/Rppp1ppp/1b3nbN/nPB5/B1P1P3/q4N2/Pp1P2PP/R2Q1RK1 w q - 0 1 //after
  //  h8g8

  if (*argv[1] == '2') {
    guiDriver gui(fen);
    gui.play();
  } else if (*argv[1] == '1') {
    // gui play against computer
    guiDriver gui(black, fen); // player play as white
    gui.play2();
  } else if (*argv[1] == 'u') {
    // implement UCI protocol

  } else if (*argv[1] == 'p') {

    // Tests::testCases();
    // do perft testing
    Engine::Board perftBoard(fen); // initial start position
    // int depth = *argv[2] - '0';
    // std::ostream file("crashes/perft_zobrist.txt");
    for (int d = 1; d <= 5; d++) {
      clock_t start, end;
      start = clock();
      perftBoard.display(debugFile);

      // unsync the I/O of C and C++.
      ios_base::sync_with_stdio(false);
      int nodes = Tests::perft(perftBoard, d, debugFile);
      debugFile << YELLOW << nodes << RESET << std::endl;
      // std::cout << perftBoard.toFenString() << std::endl;
      end = clock();
      double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
      debugFile << "Time taken by d:" << d << " is " << fixed << time_taken
           << setprecision(5);
      debugFile << " sec " << endl;
    }
  } else if (*argv[1] == 'd') {
    int depth = *argv[2] - '0';
    Engine::Board perftBoard(fen);
    perftBoard.display();
    int nodes = Tests::perft(perftBoard, depth, debugFile);
    std::cout << YELLOW << nodes << RESET << std::endl;
    Tests::divide(depth, fen, debugFile);
  } else if (*argv[1] == 't') { // Perft Suite tests
    Tests::testCases();
  } else if (*argv[1] == 'z') { // Zobrist Hashing testing
    // std::string whiteQueenSideCastleTest =
    // "r1bqkb1r/ppp1pppp/2n2n2/3p4/3P1B2/2NQ1N2/PPP1PPPP/R3KB1R b KQkq - 0 1";
    std::string testBoard =
        "4k3/8/8/8/8/8/8/4K2R w K - 0 1"; // testing enpessant capture
    Engine::Board board(testBoard);
    board.displayState(board.gameStateHistory.peek());
    cout << board.zobristKey << endl;
    Engine::Move move(63,7,false, false, false, e, e);
    // Engine::Move a5b6(24,17,false, false, true, Piece::e, Piece::p);
    // Engine::Move a2a4(48, 32, false, false, false, Piece::e, Piece::e);
    // Engine::Move e1c1(60, 58, true, false, false, Piece::e, Piece::e);
    // Engine::Move move(1, 16, false, false, false, Piece::e, Piece::e);
    board.makeMove(move);
    // board.makeMove(move);
    // board.makeMove(a2a4);
    board.display();
    board.displayState(board.gameStateHistory.peek());
    cout << board.zobristKey << endl;
    // board.unmakeMove(a2a4);
    // board.unmakeMove(move);
    board.unmakeMove(move);
    board.display();
    board.displayState(board.gameStateHistory.peek());
    cout << board.zobristKey << endl;

    // cout << "<______________________________>" << endl;
    // cout << board.zobristKey << endl;
    // long key = board.zobristKey;
    // key ^= board.zobristHash.pieces[0][0][48];
    // key ^= board.zobristHash.pieces[0][0][32];
    // cout << key << endl;
    // key ^= board.zobristHash.pieces[0][0][48];
    // key ^= board.zobristHash.pieces[0][0][32];
    // cout << key << endl;

  } else if (*argv[1] == 's') { // FixedStack testing
    // std::size_t sizearr = 10;
    FixedStack<int, 10> mystack;
    for (int i = 0; i < 5; i++) {
      mystack.push(i);
      cout << mystack.peek() << std::endl;
    }
    std::cout << mystack.size() << std::endl;

    cout << mystack.peek() << std::endl;
    mystack.pop();
    mystack.pop();
    mystack.push(100);
    mystack.pop();
    if (mystack.size() > 5) {
      cout << "yea " << std::endl;
    }
    for (uint i = 0; i < mystack.size(); i++) {
      cout << mystack.peek() << std::endl;
      mystack.pop();
    }
  } else {
    cout << "Mode not accepted" << *argv[1] << endl;
  }
  (void)argc;
  return 0;
}