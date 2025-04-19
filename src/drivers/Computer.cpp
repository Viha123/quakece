#include "Computer.hpp"
#include "../../engine/Board.hpp"
#include "../../engine/move.hpp"
#include "../../engine/search.hpp"
#include <chrono>
#include <iostream>
#include <limits>

Computer::Computer() = default;

void Computer::setSide(Color side) { m_side = side; }
Engine::Move Computer::makeMove(Engine::Board &board) {
  int nodes = 0;
  int depth = 5;
  int quiesc_depth = 0;
  // detect middle and end game and change depth based on that.
  auto start = std::chrono::high_resolution_clock::now();
  // Engine::Move move =
  //     Engine::alphabetaroot(board, depth, nodes,
  //                           quiesc_depth); // without transposition tables

  Engine::Move best_move =
      Engine::alphabetaroot_pp(board, depth, nodes, quiesc_depth);
  int alpha = std::numeric_limits<int>::min() + 1;
  int beta = std::numeric_limits<int>::max() - 1;
  // Engine::Move best_move;
  // int plyFromRoot = 0;

  // int b_score;
  // Engine::TranspositionTable tt;
  // int best_score =
  //     Engine::alphabeta_tt(board, plyFromRoot, depth, alpha, beta, nodes,
  //                          quiesc_depth, best_move, best_score, tt);

  auto end = std::chrono::high_resolution_clock::now();
  auto duratio =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  best_move.printMove();
  board.makeMove(best_move);
  board.display();
  std::cout << nodes << " " << duratio.count() << std::endl;
  return best_move;
}