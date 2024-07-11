#include "../utils.hpp"
#include "Board.hpp"
#include "eval.hpp"
#include "movegen/movegen.hpp"
#include <limits>
namespace Engine {
int negamax(Board &board, int depth, int &nodes) { // returns score
  if (depth == 0) {
    nodes +=1;
    return evaluation(board.gameStateHistory.peek().turn, board);
  }
  int max = std::numeric_limits<int>::min();
  int score = max;
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    score = -negamax(board, depth - 1, nodes);
    board.unmakeMove(move);
    if (score > max) {

      max = score;
    }
  }
  return max;
}
Move negamaxRoot(Board &board, int depth, int &nodes) {
  int max = std::numeric_limits<int>::min();
  int score = max;
  Move toMake;
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    score = -negamax(board, depth - 1, nodes);  
    board.unmakeMove(move);
    if (score > max) {
      toMake = move;
      max = score;
    }
  }
  return toMake;
}
int alphabeta(int alpha, int beta, int depth, Board &board, int &nodes) {
  if (depth == 0) {
    nodes +=1;
    return evaluation(board.gameStateHistory.peek().turn, board);
  }
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  orderMoves(moves, board);
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    int score = -alphabeta(-beta, -alpha, depth - 1, board, nodes);
    board.unmakeMove(move);
    if (score >= beta) {
      // std::cout << score << beta << std::endl;
      return beta;
    }
    if (score > alpha) {
      alpha = score;
    }
  }
  // std::cout << "here" << std::endl;
  return alpha;
}

Move alphabetaroot(Board &board, int depth, int &nodes) {
  FixedStack<Move, 256> moves;
  int alpha = -1000000000; //-inf
  int beta = 1000000000;
  getLegalMoves(board, moves);
  orderMoves(moves, board);
  Move toMake;
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    int score = -alphabeta(-beta, -alpha, depth - 1, board, nodes);
    board.unmakeMove(move);
    if (score >= beta) { 
      //the move is very good, so the opponent will anyways avoid thsi
      return move;
    }
    if (score > alpha) {
      // better than what we have so far, acts as max
      alpha = score;
      toMake = move;
    }
  }
  return toMake;
}

} // namespace Engine