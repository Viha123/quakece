#include <iostream>
#include "../../engine/Board.hpp"

namespace Tests {
  int perft(Engine::Board& board, int depth); 
  bool testCases();
  void timeTest();
}