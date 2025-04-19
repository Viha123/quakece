#pragma once
#include "Board.hpp"
#include "TranspositionTable.hpp"
#include <omp.h>
namespace Engine {
int negamax(Board &board, int depth, int &nodes); // returns score
Move negamaxRoot(Board &board, int depth,
                 int &nodes); // returns best move calls negamax
Move alphabetaroot(Board &board, int depth, int &nodes, int &quiesc_depth);

Move alphabetaroot_pp(Board &board, int depth, int &nodes,
                      int &quiesc_depth); // comapre the speedup betwene this
                                          // function and alphabeta root

int quiescense_search(int alpha, int beta, Board &board, int &quiesc_depth,
                      int line, std::array<Move, 16> &pv);

int alphabeta(int alpha, int beta, int depth, Board &board, int &nodes,
              int line, std::array<Move, 16> &pv, int &quiesc_depth);

int alphabeta_tt(Board &board, int plyFromRoot, int plyRemaining, int alpha,
                 int beta, int &nodes, int &quiesc_depth, Move &best_move,
                 int &best_score, TranspositionTable &tt);

} // namespace Engine