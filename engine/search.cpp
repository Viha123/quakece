#include "Board.hpp"
#include "eval.hpp"
#include "movegen/movegen.hpp"
#include "search.hpp"
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
int quiescense_search(int alpha, int beta, Board &board, int &nodes, int line, std::array<Move, 16> &pv) {
  nodes+=1;
  
  int score = evaluation(board.gameStateHistory.peek().turn, board);

  if (score >= beta) {
    return beta;
  }
  if (alpha <= score) {
    alpha = score;
  }
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  if(moves.size() == 0) {
    if(kingInCheck(board, board.gameStateHistory.peek().turn)) {
      return -1000000000;
    } else {
      return 0;
    }
  }
  orderMoves(moves, board);
  for(int i = 0; i < moves.size(); i ++) {
    if (getMoveScore(moves[i], board) < 5) { //this is some sort of capture move
      //keep iterative searching until its really done
      Move move = moves[i];
      board.makeMove(move);
      int s = quiescense_search(-beta, -alpha, board, nodes, line, pv);
      board.unmakeMove(move);
      if (s >= beta) {
        return beta;
      }
      if (score > alpha) {
        alpha = score;
      }
    } else {
      break;
    }
    
  }
  return alpha;
}
int alphabeta(int alpha, int beta, int depth, Board &board, int &nodes, int line, std::array<Move, 16> &pv) {
  if (depth == 0) {
    nodes +=1;
    return quiescense_search(alpha, beta, board, nodes, line, pv);
  }
  FixedStack<Move, 256> moves;
  getLegalMoves(board, moves);
  orderMoves(moves, board);
  std::array<Move, 16> localPv = {};
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
    int score = -alphabeta(-beta, -alpha, depth - 1, board, nodes, line + 1, localPv);
    board.unmakeMove(move);
    if (score >= beta) {
      // std::cout << score << beta << std::endl;
      return beta;
    }
    if (score > alpha) {
      alpha = score;
      pv[0] = move;
      for (int j = 0; j < depth - 1; j ++) { // copy old principal variation to the current one. because each step is built on the previous steps
        pv[j + 1] = localPv[j];
      }
      // std::cout << line << " ";
      // move.printInChess(); //this is the move to make
    }
  }
  // std::cout << "here" << std::endl;
  return alpha;
}

Move alphabetaroot(Board &board, int depth, int &nodes) { //max depth will be 16 at all times, so there can be only 16 pv moves
  FixedStack<Move, 256> moves;
  int alpha = -1000000000; //-inf
  int beta = 1000000000;
  getLegalMoves(board, moves);
  orderMoves(moves, board);
  std::array<Move, 16> pv = {};

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
    std::array<Move, 16> localPV = {};
    int score = -alphabeta(-beta, -alpha, depth - 1, board, nodes, 1, localPV);
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
      pv[0] = toMake;
      // std::cout << depth << " ";
      // move.printInChess();s = quiescense_search //this is the move to make
      for(int j = 0; j < depth-1; j ++) {
        pv[j + 1] = localPV[j];
      }
    }
  }
  for(int i = 0; i < depth; i ++) {
    pv[i].printInChess();
  }
  return toMake;
}

} // namespace Engine