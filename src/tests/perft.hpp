#include <fstream>
#include <iostream>
#include "../../engine/Board.hpp"

namespace Tests {
  int perft(Engine::Board& board, int depth, std::ofstream &debugFile); 
  void divide(int depth, std::string fen, std::ofstream &debugFile);
  bool testCases();
  void timeTest();
}