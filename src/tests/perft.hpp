#include <iostream>
#include "../../engine/Board.hpp"

namespace Tests {
  int perft(Engine::Board& board, int depth); 
  void divide(int depth, std::string fen);
  bool testCases();
  void timeTest();
}