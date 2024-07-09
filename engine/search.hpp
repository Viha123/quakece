#include "Board.hpp"
namespace Engine {
  int negamax(Board& board, int depth); //returns score
  Move negamaxRoot(Board& board, int depth); //returns best move calls negamax
  
}