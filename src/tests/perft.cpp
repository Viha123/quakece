#include "../../engine/Board.hpp"
#include "../../engine/move.hpp"
#include "../../engine/movegen/movegen.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include "../../Headers/gui.hpp"
namespace Tests {
int perft(Engine::Board &board, int depth) {
  if (depth == 0) {
    return 1;
  }
  std::vector<Engine::Move> moves = Engine::getLegalMoves(board);
  // std::cout << "Turn: " << board.gameStateHistory.back()->turn << std::endl;
  int nodes = 0;
  for (auto move : moves) {
    board.makeMove(move);
    int node = perft(board, depth - 1);
    nodes += node;
    board.unmakeMove(move);
  }

  return nodes;
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

  for(int i = 0; i < 128; i ++) {
    Engine::Board perftBoard(fenStrings[i]);// initial start position
    for(int j = 0; j < 4; j ++) {
      int nodes = perft(perftBoard, j+1);
      if (nodes == depthValues[i][j]) {
        std::cout << GREEN << "Test Case " << i << " depth: " << j + 1 <<  RESET << std::endl;
      }
      else {
        std::cout << RED << "Test Case " << i << " depth: " << j + 1 <<  " got: " << nodes << " need: " << depthValues[i][j] << RESET << std::endl;
      }
    }
  }
  return true;
}
} // namespace Tests