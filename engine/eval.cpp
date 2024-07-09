#include "eval.hpp"
#include "Board.hpp"
#include "movegen/movegen.hpp"
namespace Engine {
int evaluation(Color color2move, Board &board) {
  int multiplier = color2move == white ? 1 : -1;
  board.populatePieceList(white);
  board.populatePieceList(black);
  // FixedStack<Move, 256> whiteMobility;
  // getLegalMoves(board, whiteMobility);
  // FixedStack<Move, 256> blackMobility;
  // getLegalMoves(board, blackMobility);
  // int mobilityWeight = (whiteMobility.size() - blackMobility.size()); //not
  // sure what this value should be. Will need tuning. std::cout <<
  // mobilityWeight << std::endl;
  int whiteMaterial = 0;
  int blackMaterial = 0;
  for(int i = 0; i < 64; i ++) {
    if(board.board[i].type == white) {
      whiteMaterial += evaluationWeightsMaterial[board.board[i].piece];
    }
    if(board.board[i].type == black) {
      blackMaterial -= evaluationWeightsMaterial[board.board[i].piece];

    }
  }

  return ((whiteMaterial + blackMaterial)) * multiplier;
}
}; // namespace Engine