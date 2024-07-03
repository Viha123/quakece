#include <iostream>
#include "../../engine/Board.hpp"
#include "../../engine/move.hpp"
#include "../../engine/movegen/movegen.hpp"

namespace Tests {
  int perft(Engine::Board& board, int depth) {
    if(depth == 0) {
      return 1;
    }
    std::vector<Engine::Move> moves = Engine::getLegalMoves(board);
    // std::cout << "Turn: " << board.gameStateHistory.back()->turn << std::endl;
    int nodes = 0;
    for(auto move : moves) {
      board.makeMove(move);
      
      int node = perft(board, depth - 1);
      nodes += node;
      board.unmakeMove(move);
    }
    
    return nodes;
  }
}