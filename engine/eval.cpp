#include "eval.hpp"
#include "Board.hpp"
#include "movegen/movegen.hpp"
namespace Engine {
int evaluation(Color color2move, Board &board) {
  int multiplier = color2move == white ? 1 : -1;
  FixedStack<Move, 256> whiteMobility;
  getLegalMovesForColor(board, whiteMobility, white);
  FixedStack<Move, 256> blackMobility;
  getLegalMovesForColor(board, blackMobility, black);
  // std::cout << whiteMobility.size() << " BLACK MOBILITY: " << blackMobility.size() << std::endl;
  int mobilityWeight = (whiteMobility.size() - blackMobility.size()); //not
  // sure what this value should be. Will need tuning. 
  // std::cout << mobilityWeight << std::endl;
  int whiteMaterial = 0;
  int blackMaterial = 0;
  for(int i = 0; i < 64; i ++) {
    if(board.board[i].type == white) {
      whiteMaterial += evaluationWeightsMaterial[board.board[i].piece] + pieceSquareTables[board.board[i].type][board.board[i].piece][i];
    }
    if(board.board[i].type == black) {
      blackMaterial -= evaluationWeightsMaterial[board.board[i].piece] + pieceSquareTables[board.board[i].type][board.board[i].piece][i];
    }
  }
  
  // goal is to reduce opposite king mobility
  if(whiteMaterial < 4000) { //white is in endgame
    int whiteKingIndex = board.kingIndexes[white];
    whiteMaterial += -pieceSquareTables[white][k][whiteKingIndex] + kingEndGame[white][whiteKingIndex];
  }
  if(blackMaterial > -4000) { //black is in endgame
    int blackKingIndex = board.kingIndexes[black];
    // std::cout << "black is in endgame" << std::endl;

    whiteMaterial -= -pieceSquareTables[black][k][blackKingIndex] + kingEndGame[black][blackKingIndex];
  }

  return ((whiteMaterial + blackMaterial) + mobilityWeight*50) * multiplier;
}
}; // namespace Engine