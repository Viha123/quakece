#include "Board.hpp"
namespace Engine {
  int negamax(Board& board, int depth, int &nodes); //returns score
  Move negamaxRoot(Board& board, int depth, int &nodes); //returns best move calls negamax
  Move alphabetaroot(Board& board, int depth, int &nodes);
  int alphabeta(int alpha, int beta, int depth, Board& board, int &nodes);
}