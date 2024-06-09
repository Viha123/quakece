#include "movegen.hpp"
#include "../../Headers/engine.hpp"
#include "../../utils.hpp"
#include "../Board.hpp"
#include "../move.hpp"
#include <iostream>
#include <vector>
namespace Engine {
std::vector<int> getMoveForPiece(Board &board, int num) {
  board.display();
  std::vector<int> moves = {};
  Board::Square square = board.getSquare(num);
  Board::State state = board.getState();
  int row = utils::getRank(num);
  int col = utils::getFile(num);
  Piece piece = square.piece;
  std::cout << "Beginning of move availability!!!" << std::endl;
  // if this is a sliding piece then here are the available moves for that
  // piece.
  int mailBoxIndex = mailbox64[num];

  if ((square.type == state.turn) &&
      (isSlide[square.piece] || square.piece == n)) {
    // go until all directions until end of board (mailbox returns -1)

    for (auto offset : directionOffsets[square.piece]) {
      for (int i = 1; i <= 8; i++) {
        if ((square.piece == k or square.piece == n) and i > 1) {
          break;
        }
        int targetIndex = mailbox[(offset)*i + mailBoxIndex]; // test
        if ((targetIndex != -1) && (targetIndex != num) &&
            (board.getSquare(targetIndex).type != state.turn)) {
          //&& mailbox[offset + mailBoxIndex]
          // std::cout << targetIndex << " " <<
          // board.getSquare(targetIndex).type << std::endl;
          if ((board.getSquare(targetIndex).type != state.turn &&
               board.getSquare(targetIndex).type != none)) {
            // std::cout << "opp side" << std::endl;
            moves.push_back(targetIndex);
            break;
          }
          moves.push_back(targetIndex);

        } else {
          break;
        }
      }
    }
  }

  if ((square.type == state.turn) && square.piece == p) {
    // std::cout << "PAWN!!!" << std::endl;
    // std::vector<int> possibleMoves = {};
    int pawnOffset[2][2] = {{-8, -16}, {8, 16}}; // 0: white, or 1: black
    if ((row == 1 && square.type == black) ||
        (row == 6 && square.type == white)) {
      // pawn can move 2 steps
      if (board.board[num + pawnOffset[square.type][0]].piece == e) {
        moves.push_back(num + pawnOffset[square.type][0]);
        if (board.board[num + pawnOffset[square.type][1]].piece == e) {
          moves.push_back(num + pawnOffset[square.type][1]);
        }
      }
    } else {
      if (board.board[num + pawnOffset[square.type][0]].piece == e) {
        moves.push_back(num + pawnOffset[square.type][0]);
      }
    }
    // check diagonal capture:
    int pawnCaptureOffset[2][2] = {{-11, -9},
                                   {9, 11}}; // 0 for white and 1 for black
    for (int offset = 0; offset < 2; offset += 1) {
      int targetIndex =
          mailbox[mailbox64[num] + pawnCaptureOffset[square.type][offset]];
      std::cout << targetIndex << std::endl;
      if ((targetIndex != -1) &&
          (square.type != board.board[targetIndex].type) &&
          (board.board[targetIndex].type != none)) {
        std::cout << targetIndex << std::endl;
        moves.push_back(targetIndex);
      }

      // check enpessant capture possibility:

      else if ((targetIndex == state.enpessant) && (square.jumpCount == 2)) {
        moves.push_back(targetIndex);
      }
    }
  }
  // check castle possibility
  if (square.type == state.turn and square.piece == k) {
    if (square.type == black) {
      // shift right 2 and 3 times
      if (((state.castle_flag >> 2) & 0b0001) == 1) { // black king castle
        moves.push_back(num + 2);
      }
      if (((state.castle_flag >> 3) & 0b0001) == 1) { // black queen castle
        moves.push_back(num - 2);
      }
    }
    if (square.type == white) {
      // shift right 2 and 3 times
      std::cout <<((state.castle_flag >> 1) & 0b0010)<< std::endl;

      if (((state.castle_flag >> 0) & 0b0001) == 1) { // white king castle
        moves.push_back(num + 2);
      }
      if (((state.castle_flag >> 1) & 0b0001) == 1) { // white queen castle

        moves.push_back(num - 2);
      }
    }
  }

  return moves;
}
std::vector<Move> getPsuedoLegalMoves() {
  std::vector<Move> moves;

  return moves;
}
} // namespace Engine