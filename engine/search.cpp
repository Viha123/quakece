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
      int s = quiescense_search(-beta, -alpha, board, quiesc_depth, line, pv);
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
    return quiescense_search(-alpha, -beta, board, quiesc_depth, line, pv);
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
  for (int i = 0; i < depth; i++) {
    pv[i].printInChess();
  }
  return toMake;
}
int alphabeta_tt(
    Board &board, int plyFromRoot, int plyRemaining, int alpha, int beta,
    int &nodes, int &quiesc_depth, Move &best_move, int &best_score,
    TranspositionTable &tt) { // do not worry about getting pv for now.
  std::array<Move, 16> pv = {};
  // std::cout << "in alphabeta tt" << std::endl;
  // const int immediateMateScore = 10000;
  // if (plyFromRoot > 0) {
  //   // gone through more than 1 test
  //   alpha =
  //       std::max(alpha,
  //                -immediateMateScore +
  //                    plyFromRoot); // if we are about to get checkmated, then
  //                                  // chooses the value that is farthest from
  //                                  // the checkmate, like keep doing your best
  //                                  // even though you know your gonna lose.
  //   beta = std::min(beta, immediateMateScore - plyFromRoot);
  //   if (alpha >= beta) {
  //     std::cout << alpha << " " << beta << std::endl;
  //     return alpha;
  //   }
  // }
  int tt_result =
      tt.lookupEvaluation(board.zobristKey, plyRemaining, alpha, beta);
  // std::cout << tt_result << std::endl; 
  if (tt_result != -1) {
    if (plyFromRoot == 0) {
      // it's the beginning
      // this is if we directly use the TT move
      std::cout << "here" << std::endl;
      best_move = tt.get(board.zobristKey)->move;
      best_score = tt.get(board.zobristKey)->eval;
    }
    // std::cout << "found eval" << std::endl;
    return tt_result;
  }
  if (plyRemaining == 0) {
    int quiesc_depth = 0;
    int evaluation = quiescense_search(-beta, -alpha, board, quiesc_depth, 0, pv);
    return evaluation;
  }
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  orderMoves(moves, board);
  // order move such that the TT Move is first

  if (tt_result != -1) { // this can be made better
    // swapping the TT result
    Move tt_evaluated_move = tt.get(board.zobristKey)->move;
    Move temp = moves[0];
    moves[0] = tt_evaluated_move;
    moves.push(temp);
  }

  if (moves.size() == 0) {
    std::cout << "HERE" << std::endl;
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
    // int score =
    //     -alphabeta_tt(-beta, -alpha, ++plyFromRoot, board, nodes, 1, localPV,
    //                   quiesc_depth, tt, move_found, possible_tt_move);
    board.unmakeMove(move);

    // std::cout << "Move: ";
    // move.printInChess();
  
    std::cout << "Score: " << score << ", Alpha: " << alpha
              << ", Beta: " << beta << std::endl;

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
      // std::cout << "flagged as alpha" << std::endl;

      // move_found = true; // Mark that a better move was found
      std::cout << score << std::endl;
      pv[0] = best_move;
      tt.store(board.zobristKey, plyRemaining, flag, score, 0, best_move_in_pos);
      for (int j = 0; j < plyRemaining - 1; j++) {
        pv[j + 1] = localPV[j];
      }
      if (plyFromRoot == 0) {
        best_move = move;

      }
    }
  }
  tt.store(board.zobristKey, plyRemaining, flag, alpha, 0, best_move_in_pos);
  return alpha;
}

// Move alphabeta_tt_root(Board &board, int depth, int &nodes, int
// &quiesc_depth,
//                        TranspositionTable &tt) {
//   FixedStack<Move, 256> moves;
//   int alpha = std::numeric_limits<int>::min(); // -inf
//   int beta = std::numeric_limits<int>::max();
//   getLegalMoves(board, moves);
//   orderMoves(moves, board);
//   std::array<Move, 16> pv = {};

//   if (moves.size() == 0) {
//     if (kingInCheck(board, board.gameStateHistory.peek().turn)) {
//       throw std::out_of_range("Checkmate");
//     } else {
//       throw std::length_error("Stalemate");
//     }
//   }

//   Move toMake = moves[0]; // Default to the first move

//   bool move_found = false;

//   for (int i = 0; i < moves.size(); i++) {
//     Move move = moves[i];
//     Move possible_tt_move;
//     board.makeMove(move);
//     std::array<Move, 16> localPV = {};
//     int score = -alphabeta_tt(-beta, -alpha, depth - 1, board, nodes, 1,
//                               localPV, quiesc_depth, tt, move_found,
//                               possible_tt_move);
//     board.unmakeMove(move);

//     // std::cout << "Move: ";
//     // move.printInChess();
//     if (move_found) {
//       toMake = possible_tt_move;
//     }
//     std::cout << "Score: " << score << ", Alpha: " << alpha
//               << ", Beta: " << beta << std::endl;

//     if (score >= beta) {
//       return toMake; // Beta cutoff
//     }
//     if (score > alpha) {
//       alpha = score;
//       toMake = move;
//       // move_found = true; // Mark that a better move was found
//       pv[0] = toMake;
//       for (int j = 0; j < depth - 1; j++) {
//         pv[j + 1] = localPV[j];
//       }
//     }
//   }

//   // // Fallback to the first move if no better move was found
//   // if (!move_found) {
//   //   toMake = moves[0];
//   // }

//   for (int i = 0; i < depth; i++) {
//     pv[i].printInChess();
//   }
//   toMake.printInChess();
//   return toMake;
// }
// int alphabeta_tt(int alpha, int beta, int depth, Board &board, int &nodes,
//                  int line, std::array<Move, 16> &pv, int &quiesc_depth,
//                  TranspositionTable &tt, bool &found_move,
//                  Move &best_move_from_tt) {
//   if (depth == 0) {
//     nodes += 1;
//     return quiescense_search(alpha, beta, board, quiesc_depth, line, pv);
//   }

//   long current_zobrist_key = board.zobristKey;
//   auto entry = tt.get(current_zobrist_key);

//   // Use the transposition table entry if it exists and is valid
//   if (entry && entry.value().depth >= depth) {
//     best_move_from_tt = entry.value().move;
//     found_move = true;

//     if (entry.value().flag == Flag::exact) {
//       return entry.value().eval;
//     } else if (entry.value().flag == Flag::alpha) {
//       return std::max(alpha, entry.value().eval);
//     } else if (entry.value().flag == Flag::beta) {
//       return std::min(beta, entry.value().eval);
//     }
//   }

//   FixedStack<Move, 256> moves;
//   getLegalMoves(board, moves);
//   orderMoves(moves, board);

//   std::array<Move, 16> localPv = {};
//   if (moves.size() == 0) {
//     if (kingInCheck(board, board.gameStateHistory.peek().turn)) {
//       return std::numeric_limits<int>::min() + depth; // Checkmate
//     } else {
//       return 0; // Stalemate
//     }
//   }

//   int original_alpha = alpha;
//   int best_score = std::numeric_limits<int>::min();
//   // If a move was found in the transposition table, evaluate it first
//   if (found_move) {
//     // std::cout << best_move_from_tt.printMove() << std::endl;
//     best_move_from_tt.printInChess();
//     auto it = std::find(moves.begin(), moves.end(), best_move_from_tt);
//     if (it != moves.end()) {
//       std::iter_swap(moves.begin(), it); // Move it to the front
//     }
//     std::cout << "the first move is now: ";
//     moves[0].printInChess();
//   }
//   Move best_move = moves[0]; // Default to the first move

//   for (int i = 0; i < moves.size(); i++) {
//     Move move = moves[i];
//     board.makeMove(move);
//     int score =
//         -alphabeta_tt(-beta, -alpha, depth - 1, board, nodes, line + 1,
//         localPv,
//                       quiesc_depth, tt, found_move, best_move_from_tt);
//     board.unmakeMove(move);

//     if (score >= beta) {
//       // Beta cutoff
//       tt.store(current_zobrist_key, depth, Flag::beta, beta, 0, move);
//       return beta;
//     }
//     if (score > alpha) {
//       alpha = score;
//       best_move = move;
//       pv[0] = move;
//       for (int j = 0; j < depth - 1; j++) {
//         pv[j + 1] = localPv[j];
//       }
//     }
//     best_score = std::max(best_score, score);
//   }

//   // Store the result in the transposition table
//   Flag flag = (alpha > original_alpha) ? Flag::exact : Flag::alpha;
//   tt.store(current_zobrist_key, depth, flag, best_score, 0, best_move);

//   return alpha;
// }

} // namespace Engine