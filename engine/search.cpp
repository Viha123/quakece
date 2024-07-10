#include "../utils.hpp"
#include "Board.hpp"
#include "eval.hpp"
#include "movegen/movegen.hpp"
#include <limits>
namespace Engine {
int negamax(Board &board, int depth) { // returns score
  if (depth == 0) {
    return evaluation(board.gameStateHistory.peek().turn, board);
  }
  int max = std::numeric_limits<int>::min();
  int score = max;
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    score = -negamax(board, depth - 1);
    // std::cout << +board.gameStateHistory.peek().turn << " " << score <<
    // std::endl;
    board.unmakeMove(move);
    if (score > max) {

      max = score;
    }
  }
  return max;
}
Move negamaxRoot(Board &board, int depth) {
  int max = std::numeric_limits<int>::min();
  int score = max;
  Move toMake;
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    score = -negamax(board, depth - 1);
    board.unmakeMove(move);
    if (score > max) {
      // std::cout << utils::convertToChessNotation(move._move_from)
      //         << utils::convertToChessNotation(move._move_to) << " " << score
      //         << std::endl;
      toMake = move;
      max = score;
    }
  }
  return toMake;
}
int alphabeta(int alpha, int beta, int depth, Board &board) {
  if (depth == 0) {
    return evaluation(board.gameStateHistory.peek().turn, board);
  }
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    int score = -alphabeta(-beta, -alpha, depth - 1, board);
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

Move alphabetaroot(Board &board, int depth) {
  FixedStack<Move, 256> moves;
  int alpha = -1000000000; //-inf
  int beta = 1000000000;
  getLegalMoves(board, moves);
  Move toMake;
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    // toMake = move;
    board.makeMove(move);
    int score = -alphabeta(-beta, -alpha, depth - 1, board);
    // std::cout << +board.gameStateHistory.peek().turn << " " << score <<
    // std::endl;
    // std::cout << score << std::endl;
    board.unmakeMove(move);
    if (score >= beta) {
      return move;
    }
    if (score > alpha) {
      // std::cout << "HER" << std::endl;
      alpha = score;
      toMake = move;
    }
  }
  return toMake;
}

} // namespace Engine