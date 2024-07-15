#include "Computer.hpp"
#include "../../engine/Board.hpp"
#include "../../engine/move.hpp"
#include "../../engine/search.hpp"
Computer::Computer() = default;

void Computer::setSide(Color side) {
  m_side = side;
}
Engine::Move Computer::makeMove(Engine::Board &board) {
  int nodes = 0;
  int depth = 5;
  //detect middle and end game and change depth based on that. 
  Engine::Move move = Engine::alphabetaroot(board, depth, nodes);
  
  move.printMove();
  board.makeMove(move);

  board.display();
  std::cout << nodes << std::endl;
  return move;
}