#include "movegen.hpp"
#include "../../Headers/engine.hpp"
#include "../../utils.hpp"
#include "../Board.hpp"
#include "../move.hpp"
#include <iostream>
#include <vector>
namespace Engine {
std::vector<Move> getMoveForPiece(Board &board, int num) {
  board.display();
  std::cout << "piece getting move for: " << board.getSquare(num).piece
            << std::endl;
  std::vector<Move> moves = {};
  Board::Square square = board.getSquare(num);
  int row = utils::getRank(num);
  int col = utils::getFile(num);
  Piece piece = square.piece;
  std::cout << "Beginning of move availability!!!" << std::endl;
  // if this is a sliding piece then here are the available moves for that
  // piece.
  int mailBoxIndex = mailbox64[num];
  Board::State *currentState = board.gameStateHistory.back();
  // board.displayState(currentState);
  if ((square.type == currentState->turn) &&
      (isSlide[square.piece] || square.piece == n)) {
    // go until all directions until end of board (mailbox returns -1)

    for (auto offset : directionOffsets[square.piece]) {
      for (int i = 1; i <= 8; i++) {
        if ((square.piece == k or square.piece == n) and i > 1) {
          break;
        }
        int targetIndex = mailbox[(offset)*i + mailBoxIndex]; // test
        if ((targetIndex != -1) && (targetIndex != num) &&
            (board.getSquare(targetIndex).type != currentState->turn)) {

          if ((board.getSquare(targetIndex).type != currentState->turn &&
               board.getSquare(targetIndex).type != none)) {
            // capture
            Move m(num, targetIndex, false, false, true, e,
                   board.getSquare(targetIndex).piece);
            moves.push_back(m);
            break;
          }
          Move m(num, targetIndex, false, false, false, e, e);
          moves.push_back(m);
        } else {
          break;
        }
      }
    }
  }
  // TODO: pawn promotion test
  if ((square.type == currentState->turn) && square.piece == p) {
    // std::cout << "PAWN!!!" << std::endl;
    // std::vector<int> possibleMoves = {};
    int pawnOffset[2][2] = {{-8, -16}, {8, 16}}; // 0: white, or 1: black
    if ((row == 1 && square.type == black) ||
        (row == 6 && square.type == white)) {
      // pawn can move 2 steps
      if (board.board[num + pawnOffset[square.type][0]].piece == e) {
        Move m(num, num + pawnOffset[square.type][0], false, false, false, e,
               e);
        moves.push_back(m);
        if (board.board[num + pawnOffset[square.type][1]].piece == e) {
          Move m(num, num + pawnOffset[square.type][1], false, false, false, e,
                 e);
          moves.push_back(m);
        }
      }

    } else {
      // generate promotion possibility:
      if (square.type == black && row == 6 && board.board[num + 8].piece == e) {
        // std::cout << "black promotion possibility" << std::endl;
        // Piece promotions[4] = {q, n, b, r};
        // for (auto pr : promotions) {
        //   Move m(num, num + 8, false, true, false, pr);
        //   moves.push_back(m);
        // }
        handlePromotions(moves, num, num + 8, e);

      } else if (square.type == white && row == 1 &&
                 board.board[num - 8].piece == e) {
        // std::cout << "white promotion possibility" << std::endl;
        handlePromotions(moves, num, num - 8, e);

      } else if (board.board[num + pawnOffset[square.type][0]].piece == e) {
        Move m(num, num + pawnOffset[square.type][0], false, false, false, e,
               e);
        moves.push_back(m);
      }
    }
    // check diagonal capture:
    int pawnCaptureOffset[2][2] = {{-11, -9},
                                   {9, 11}}; // 0 for white and 1 for black
    for (int offset = 0; offset < 2; offset += 1) {
      int targetIndex =
          mailbox[mailbox64[num] + pawnCaptureOffset[square.type][offset]];

      if ((targetIndex != -1) &&
          (square.type != board.board[targetIndex].type) &&
          (board.board[targetIndex].type != none)) {
        // std::cout << "this is true" << std::endl;
        if (utils::getRank(targetIndex) == 0 && square.type == white) {
          // Move m(num, targetIndex, false, true, true, e);
          // moves.push_back(m);
          handlePromotions(moves, num, targetIndex,
                           board.board[targetIndex].piece);
        } else if (utils::getRank(targetIndex) == 7 && square.type == black) {
          handlePromotions(moves, num, targetIndex,
                           board.board[targetIndex].piece);

        } else {
          Move m(num, targetIndex, false, false, true, e,
                 board.board[targetIndex].piece);
          moves.push_back(m);
        }
      }

      // check enpessant capture possibility:
      std::cout << "currentstate enpessant" << +currentState->enpessant
                << " target index" << targetIndex << " jumpcounts"
                << currentState->jumpCounts[num] << std::endl;
      if ((targetIndex == +currentState->enpessant) &&
          (currentState->jumpCounts[num] == 2)) {
        Move m(num, targetIndex, false, false, true, e,
               p); // if target index does not have any piece and capture is
                   // true then assume enpessant
        moves.push_back(m);
      }
    }
  }
  // check castle possibility
  if (square.type == currentState->turn and square.piece == k) {
    Move m1(num, num + 2, true, false, false, e, e);
    Move m2(num, num - 2, true, false, false, e, e);
    if (square.type == black) {
      // shift right 2 and 3 times

      if (board.board[num + 1].piece == e && board.board[num + 2].piece == e &&
          ((currentState->castle_flag >> 2) & 0b0001) ==
              1) { // black king castle
        moves.push_back(m1);
      }
      if (board.board[num - 1].piece == e && board.board[num - 2].piece == e &&
          board.board[num - 3].piece == e &&
          ((currentState->castle_flag >> 3) & 0b0001) ==
              1) { // black queen castle
        moves.push_back(m2);
      }
    }
    if (square.type == white) {
      // shift right 2 and 3 times
      // std::cout << ((currentState->castle_flag >> 1) & 0b0010) << std::endl;

      if (board.board[num + 1].piece == e && board.board[num + 2].piece == e &&
          ((currentState->castle_flag >> 0) & 0b0001) ==
              1) { // white king castle
        moves.push_back(m1);
      }
      if (board.board[num - 1].piece == e && board.board[num - 2].piece == e &&
          board.board[num - 3].piece == e && // 3 things between rook and king
          ((currentState->castle_flag >> 1) & 0b0001) ==
              1) { // white queen castle

        moves.push_back(m2);
      }
    }
  }

  return moves;
}
void handlePromotions(std::vector<Move> &moves, int numFrom, int numTo,
                      Piece capturedPiece) {
  Piece promotions[4] = {q, n, b, r};
  bool isCapture = capturedPiece == e ? false : true;

  for (auto pr : promotions) {
    Move m(numFrom, numTo, false, true, isCapture, pr, capturedPiece);
    moves.push_back(m);
  }
}
std::vector<Move> getPsuedoLegalMoves() {
  std::vector<Move> moves;

  return moves;
}
} // namespace Engine