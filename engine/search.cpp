#include "../utils.hpp"
#include "Board.hpp"
#include "eval.hpp"
#include "movegen/movegen.hpp"
#include <cstddef>
#include <exception>
#include <limits>
#include <stdexcept>
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
  if(moves.size() == 0) {
    if(kingInCheck(board, board.gameStateHistory.peek().turn)) {
      return -1000000000;
    } else {
      return 0;
    }
  }
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
  if(moves.size() == 0) {
    if(kingInCheck(board, board.gameStateHistory.peek().turn)) {
      throw std::out_of_range("Checkmate");
    } else {
      throw std::length_error("Stalemate");
    }
  }
  Move toMake = moves[0];
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
      int lastIndex = board.history.size();
      if(lastIndex >= 4 and toMake == board.history[lastIndex - 4]) {
        break; //do not update toMake to be the next best move 
      }
      alpha = score;
      toMake = move;
    }
  }
  return toMake;
}

} // namespace Engine