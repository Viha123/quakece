#include "search.hpp"
#include "Board.hpp"
#include "TranspositionTable.hpp"
#include "eval.hpp"
#include "movegen/movegen.hpp"
#include <limits>
#include <stdexcept>
namespace Engine {
int negamax(Board &board, int depth, int &nodes) { // returns score
  if (depth == 0) {
    nodes += 1;
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
int quiescense_search(int alpha, int beta, Board &board, int &quiesc_depth,
                      int line, std::array<Move, 16> &pv) {
  int score = evaluation(board.gameStateHistory.peek().turn, board);
  if (score >= beta) {
    // if (quiesc_depth > 2) {
    //   std::cout << "Quiesce Depth (too good to be true move): " <<
    //   quiesc_depth << std::endl; board.display();
    // }
    return beta;
  }
  if (alpha <= score) {
    alpha = score;
  }
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  if (moves.size() == 0) {
    if (kingInCheck(board, board.gameStateHistory.peek().turn)) {
      // if (quiesc_depth > 2) {
      //   std::cout << "Quiesce Depth (kingInCheck): " << quiesc_depth <<
      //   std::endl; board.display();
      // }

      return -1000000000;
    } else {
      // if (quiesc_depth > 2) {
      //   std::cout << "Quiesce Depth (Stalemate): " << quiesc_depth <<
      //   std::endl; board.display();
      // }

      return 0;
    }
  }
  orderMoves(moves, board);
  for (int i = 0; i < moves.size(); i++) {
    if (getMoveScore(moves[i], board) < 5) { // this is some sort of capture
                                             // move
      // keep iterative searching until its really done
      Move move = moves[i];
      board.makeMove(move);
      quiesc_depth += 1;
      int s = -quiescense_search(-beta, -alpha, board, quiesc_depth, line, pv);
      quiesc_depth -= 1;
      board.unmakeMove(move);
      if (s >= beta) {
        // if (quiesc_depth > 2) {
        //   std::cout << "Quiesce Depth (too good to be true move): " <<
        //   quiesc_depth << std::endl; board.display();
        // }
        return beta;
      }
      if (s > alpha) {
        alpha = s;
      }
    } else {
      break;
    }
  }
  // if (quiesc_depth > 2) {
  //   std::cout << "good move quiesce search: " << quiesc_depth << std::endl;
  //   board.display();
  // }

  return alpha;
}
int alphabeta(int alpha, int beta, int depth, Board &board, int &nodes,
              int line, std::array<Move, 16> &pv, int &quiesc_depth) {
  if (depth == 0) {
    nodes += 1;
    return quiescense_search(alpha, beta, board, quiesc_depth, line, pv);
  }
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  orderMoves(moves, board);
  std::array<Move, 16> localPv = {};
  if (moves.size() == 0) {
    if (kingInCheck(board, board.gameStateHistory.peek().turn)) {
      return -1000000000;
    } else {
      return 0;
    }
  }
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    int score = -alphabeta(-beta, -alpha, depth - 1, board, nodes, line + 1,
                           localPv, quiesc_depth);
    board.unmakeMove(move);
    if (score >= beta) {
      // std::cout << score << beta << std::endl;
      return beta;
    }
    if (score > alpha) {
      alpha = score;
      pv[0] = move;
      for (int j = 0; j < depth - 1;
           j++) { // copy old principal variation to the current one. because
                  // each step is built on the previous steps
        pv[j + 1] = localPv[j];
      }
      // std::cout << line << " ";
      // move.printInChess(); //this is the move to make
    }
  }
  // std::cout << "here" << std::endl;
  return alpha;
}

Move alphabetaroot(Board &board, int depth, int &nodes,
                   int &quiesc_depth) { // max depth will be 16 at all times, so
                                        // there can be only 16 pv moves
  FixedStack<Move, 256> moves;
  int alpha = -1000000000; //-inf
  int beta = 1000000000;
  getLegalMoves(board, moves);
  orderMoves(moves, board);
  std::array<Move, 16> pv = {};

  if (moves.size() == 0) {
    if (kingInCheck(board, board.gameStateHistory.peek().turn)) {
      throw std::out_of_range("Checkmate");
    } else {
      throw std::length_error("Stalemate");
    }
  }
  Move toMake = moves[0];
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    board.makeMove(move);
    std::array<Move, 16> localPV = {};
    int score = -alphabeta(-beta, -alpha, depth - 1, board, nodes, 1, localPV,
                           quiesc_depth);
    board.unmakeMove(move);
    if (score >= beta) {
      // the move is very good, so the opponent will anyways avoid thsi
      return move;
    }
    if (score > alpha) {
      // better than what we have so far, acts as max
      int lastIndex = board.history.size();
      if (lastIndex >= 4 and toMake == board.history[lastIndex - 4]) {
        break; // do not update toMake to be the next best move
      }
      alpha = score;
      toMake = move;
      pv[0] = toMake;
      // std::cout << depth << " ";
      // move.printInChess();s = quiescense_search //this is the move to make
      for (int j = 0; j < depth - 1; j++) {
        pv[j + 1] = localPV[j];
      }
    }
  }
  // for (int i = 0; i < depth; i++) {
  //   pv[i].printInChess();
  // }
  return toMake;
}
int alphabeta_tt(
    Board &board, int plyFromRoot, int plyRemaining, int alpha, int beta,
    int &nodes, int &quiesc_depth, Move &best_move, int &best_score,
    TranspositionTable &tt) { // do not worry about getting pv for now.
  std::array<Move, 16> pv = {};
  // std::cout << "in alphabeta tt" << std::endl;
  const int immediateMateScore = 10000;
  if (plyFromRoot > 0) {
    // gone through more than 1 test
    alpha =
        std::max(alpha,
                 -immediateMateScore +
                     plyFromRoot); // if we are about to get checkmated, then
                                   // chooses the value that is farthest from
                                   // the checkmate, like keep doing your
                                   // best
                                   // even though you know your gonna lose.
    beta = std::min(beta, immediateMateScore - plyFromRoot);
    if (alpha >= beta) {
      // std::cout << alpha << " " << beta << std::endl;
      return alpha;
    }
  }
  int tt_result =
      tt.lookupEvaluation(board.zobristKey, plyRemaining, alpha, beta);
  // std::cout << tt_result << std::endl;
  if (tt_result != -1) {
    if (plyFromRoot == 0) {
      // it's the beginning
      // this is if we directly use the TT move
      // std::cout << "here 1" << std::endl;
      best_move = tt.get(board.zobristKey)->move;
      best_score = tt_result;

      std::cout << "got move from tt " << std::endl;
    }
    // std::cout << plyFromRoot << " " << tt_result << std::endl;
    return tt_result;
  }
  if (plyRemaining == 0) {
    int quiesc_depth = 0;
    int evaluation =
        quiescense_search(alpha, beta, board, quiesc_depth, 0, pv);
    // std::cout << "Evaluation evaluated at: " << evaluation << std::endl;
    return evaluation;
  }
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  orderMoves(moves, board);
  // order move such that the TT Move is first

  if (tt_result != -1) { // this can be made better
    // swapping the TT result
    Move tt_evaluated_move = tt.get(board.zobristKey)->move;
    tt_evaluated_move.printMove();
    Move temp = moves[0];
    moves[0] = tt_evaluated_move;
    moves.push(temp);
  }

  if (moves.size() == 0 && plyFromRoot == 0) {
    // std::cout << "HERE" << std::endl;
    if (kingInCheck(board, board.gameStateHistory.peek().turn)) {
      throw std::out_of_range("Checkmate");
    } else {
      throw std::length_error("Stalemate");
    }
  }
  // Move toMake = moves[0]; // Default to the first move
  // best_move = moves[0];
  Move best_move_in_pos = moves[0];
  bool move_found = false;
  Flag flag = Flag::alpha;
  for (int i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Move possible_tt_move;
    board.makeMove(move);
    std::array<Move, 16> localPV = {};
    int score =
        -alphabeta_tt(board, plyFromRoot + 1, plyRemaining - 1, -beta, -alpha,
                      nodes, quiesc_depth, best_move, best_score, tt);
    board.unmakeMove(move);

    // std::cout << "Move: ";
    // move.printInChess();
    // if (plyFromRoot == 0) {
    //   std::cout << "Score: " << score << ", Alpha: " << alpha
    //             << ", Beta: " << beta << " plyfromroot: " << plyFromRoot
    //             << std::endl;
    // }

    if (score >= beta) {
      // best_move = move;
      flag = Flag::beta;
      // std::cout << "flagged as beta" << std::endl;
      tt.store(board.zobristKey, plyRemaining, flag, beta, 0, move);
      return beta; // Beta cutoff
    }
    if (score > alpha) {
      alpha = score;
      best_move_in_pos = move;
      flag = Flag::exact;

      pv[0] = move;
      tt.store(board.zobristKey, plyRemaining, flag, score, 0,
               best_move_in_pos);
      for (int j = 0; j < plyRemaining - 1; j++) {
        pv[j + 1] = localPV[j];
      }
      if (plyFromRoot == 0) {
        // std::cout << "getting move from for loop" << std::endl;
        best_move = best_move_in_pos;
      }
    }
  }

  tt.store(board.zobristKey, plyRemaining, flag, alpha, 0, best_move_in_pos);
  return alpha;
}

// Move alphabeta_tt_root(Board &board, int plyFromRoot, int plyRemaining,
//                        int alpha, int beta, int &nodes, int &quiesc_depth,
//                        Move &best_move, int &best_score,
//                        TranspositionTable &tt) {

// }

} // namespace Engine