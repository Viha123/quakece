#include "../../Headers/gui.hpp"
#include "../../engine/Board.hpp"
#include "../../engine/move.hpp"
#include "../../engine/movegen/movegen.hpp"
#include "../../utils.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
namespace Tests {
int perft(Engine::Board &board, int depth) {

  std::vector<Engine::Move> moves = Engine::getLegalMoves(board);
  if (depth == 1) {
    // std::cout << board.gameStateHistory.back()->turn << std::endl;
    // assert(board.gameStateHistory.back()->turn == 0);
    return moves.size();
  }
  // Engine::Move lastMove = *board.history.back();
  // lastMove.printMove();
  // std::cout << moves.size() << std::endl;
  int nodes = 0;
  for (auto move : moves) {
    board.makeMove(move);
    // std::cout << utils::convertToChessNotation(move._move_from) << " " <<
    // utils::convertToChessNotation(move._move_to) << " " << std::endl;
    int node = perft(board, depth - 1);
    nodes += node;
    board.unmakeMove(move);
  }
  // std::cout << moves.size() << std::endl;
  return nodes;
}
void divide(int depth, std::string fen) {
  // starter
  Engine::Board perftBoard(fen);
  std::vector<Engine::Move> moves = Engine::getLegalMoves(perftBoard);
  // std::cout << "board created" << std::endl;
  for (auto move : moves) {
    // if (move._move_from == 6 && move._move_to == 21) {
    //   perftBoard.makeMove(move);
    //   std::string fen = perftBoard.toFenString();
    //   std::cout << fen << std::endl;
    //   break;
    // }
    // perftBoard.display();
    // if (move._move_from == 7) {
    //   perftBoard.display();

    //   perftBoard.makeMove(move);
    //   // std::cout << utils::convertToChessNotation(move._move_from)
    //   //           << utils::convertToChessNotation(move._move_to) << " " <<
    //   //           std::endl;
    //   perftBoard.display();
    //   std::string fen = perftBoard.toFenString();
    //   std::cout << fen << std::endl;
    //   // perftBoard.display();
    //   int nodes = perft(perftBoard, depth - 1);
    //   std::cout << utils::convertToChessNotation(move._move_from)
    //             << utils::convertToChessNotation(move._move_to) << " " <<
    //             nodes
    //             << std::endl;
    //   perftBoard.unmakeMove(move);
    // } else {
    perftBoard.makeMove(move);
    // std::cout << utils::convertToChessNotation(move._move_from)
    //           << utils::convertToChessNotation(move._move_to) << " " <<
    //           std::endl;
    // perftBoard.display();
    std::string fen = perftBoard.toFenString();
    std::cout << fen << std::endl;
    // perftBoard.display();
    int nodes = perft(perftBoard, depth - 1);
    std::cout << utils::convertToChessNotation(move._move_from)
              << utils::convertToChessNotation(move._move_to) << " " << nodes
              << std::endl;
    perftBoard.unmakeMove(move);
    // perftBoard.display();
    // }
  }
}
bool testCases() {
  // open file and read form it
  // read all lines and create a board from the fen string and get the first 6
  // depths of each assert all numbers are correct

  std::array<std::string, 128> fenStrings;
  std::array<std::array<int, 5>, 128> depthValues;
  std::ifstream myfile;
  myfile.open("src/tests/perft_suite.txt");
  if (myfile.is_open()) {

    std::string example;
    int c = 0;
    while (std::getline(myfile, example)) {
      std::string fen = example.substr(0, example.find(" ;"));
      // std::cout << fen << std::endl;
      fenStrings[c] = fen;

      std::string depths =
          example.substr(example.find(" ;") +
                         2); // 2 because that is the length of the delimiter
      // std::cout << depths << std::endl;
      int i = 0;
      while (depths.length() != 0) {
        std::string token = depths.substr(0, depths.find(" ;") + 2);
        // std::cout << token << std::endl;
        if (token.size() <= 2) {
          break;
        }
        std::string actualNumber =
            token.substr(token.find(" ") + 1, token.find(" ;") - 3);
        depthValues[c][i] = stoi(actualNumber);
        depths = depths.substr(depths.find(" ;") + 2);
        i += 1;
      }
      c += 1;
    }
    myfile.close();
  } else {
    std::cout << "unable to open file" << std::endl;
  }

  for (int i = 0; i < 128; i++) {
    Engine::Board perftBoard(fenStrings[i]); // initial start position
    for (int j = 0; j < 4; j++) {
      int nodes = perft(perftBoard, j + 1);
      if (nodes == depthValues[i][j]) {
        std::cout << GREEN << "Test Case " << i << " depth: " << j + 1 << RESET
                  << std::endl;
      } else {
        std::cout << RED << "Test Case " << i << " depth: " << j + 1
                  << " got: " << nodes << " need: " << depthValues[i][j]
                  << RESET << std::endl;
      }
    }
  }
  return true;
}
} // namespace Tests