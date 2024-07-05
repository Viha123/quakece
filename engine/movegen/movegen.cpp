#include "movegen.hpp"
#include "../../Headers/engine.hpp"
#include "../../utils.hpp"
#include "../Board.hpp"
#include "../move.hpp"
#include <iostream>
#include <vector>
#include <cassert>

namespace Engine {
std::vector<Move> getMoveForPiece(Board &board, int num) {
  // board.display();
  // std::cout << "piece getting move for: " << board.getSquare(num).piece
  //           << std::endl;
  std::vector<Move> moves = {};
  Board::Square square = board.getSquare(num);
  int row = utils::getRank(num);
  int col = utils::getFile(num);
  Piece piece = square.piece;
  // std::cout << "Beginning of move availability!!!" << std::endl;
  // if this is a sliding piece then here are the available moves for that
  // piece.
  int mailBoxIndex = mailbox64[num];
  auto& currentState = board.gameStateHistory.back();
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
            Board::Square targetSquare = board.getSquare(targetIndex);
          if ((targetSquare.type != currentState->turn &&
               targetSquare.type != none)) {
            // capture
            Move m(num, targetIndex, false, false, true, e,
                   targetSquare.piece);
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
      // std::cout << "currentstate enpessant" << +currentState->enpessant
      //           << " target index" << targetIndex << " jumpcounts"
      //           << currentState->jumpCounts[num] << std::endl;
      if (targetIndex == +currentState->enpessant && targetIndex != -1) {
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
        // moves.push_back(Move(num, num +1, false, false, false, e, e));
        moves.push_back(m1);
      }
      if (board.board[num - 1].piece == e && board.board[num - 2].piece == e &&
          board.board[num - 3].piece == e &&
          ((currentState->castle_flag >> 3) & 0b0001) ==
              1) { // black queen castle
        // moves.push_back(Move(num, num -1, false, false, false, e, e));
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
  // std::cout << "in handle promotions movegen " << numTo << std::endl;
  for (auto pr : promotions) {
    Move m(numFrom, numTo, false, true, isCapture, pr, capturedPiece);
    moves.push_back(m);
  }
}
void getLegalMoves(Board &board, std::vector<Move>& allMoves) {
  Color turn = board.gameStateHistory.back()->turn;
  // std::cout << turn << std::endl;
  board.populatePieceList(turn);
  // for(auto i : board.pieceList[turn]) {
  //   std::cout << i << " " << board.board[i].c;
  // }
  // std::cout << std::endl;
  for (int i = 0; i < 16; i++) {
    if (board.pieceList[turn][i] == -1) {
      break;
    }
    // std::cout << board.pieceList[turn][i] << std::endl;
    // board.display();
    // std::vector<Move> pieceMove =
    getLegalMovesForPiece(board, board.pieceList[turn][i], allMoves);
    // board.display();
    // allMoves.insert(allMoves.end(), pieceMove.begin(), pieceMove.end());
    // std::cout << pieceMove.size() << " Piece Number "
    //           << board.pieceList[turn][i] << std::endl;
  }
  assert(allMoves.size() <= 218);
  // std::cout << allMoves.size() << std::endl;
  // return allMoves;
}
void getLegalMovesForPiece(Board &board, int num, std::vector<Move>& legal) {
  std::vector<Move> pseudolegal = getMoveForPiece(board, num);
  // std::cout << "pseudo legal size: " << pseudolegal.size() << std::endl;
  // std::vector<Move> legal;
  Color color = board.board[num].type;
  for (auto move : pseudolegal) {

    // std::cout << "board num type: " << board.board[num].type << std::endl;
    if (!move._isCastle) {
      board.makeMove(move);
      // std::cout << "board display after hypothetical move made" << std::endl;
      // board.display();
      if (!kingInCheck(board, color)) {

        legal.push_back(move);
      }

      board.unmakeMove(move);
      // std::cout << "board display after move unmade" << std::endl;
      // board.display();
    } else {
      // move king by one spot depending on color and queen side
      // pop 2 moves in this case
      if (!kingInCheck(board, color)) {
        if (move._move_from - move._move_to == -2) {
          handleKingCheck(board, +1, move, color, legal);
        }
        if (move._move_from - move._move_to == 2) {
          handleKingCheck(board, -1, move, color, legal);
        }
      }
    }

  }

  // return legal;
}
void handleKingCheck(Board &board, int offset, Move &move, Color color,
                     std::vector<Move> &legal) {
  Move checkImmediateRight(move._move_from, move._move_from + offset, false,
                           false, false, e, e);
  board.makeMove(checkImmediateRight);
  if (!kingInCheck(board, color)) {
    // check actual move
    // std::cout << "KING NOT IMMEDIATELY RIGHT IN CHECK" << std::endl;
    // board.display();

    board.unmakeMove(checkImmediateRight);
    // std::cout << "KING immediately right unmade display:: " << std::endl;

    // // board.display();
    // std::cout << "KING make true castle:::: " << std::endl;

    board.makeMove(move);
    // board.display();
    if (!kingInCheck(board, color)) {
      legal.push_back(move);
    }
    board.unmakeMove(move);
  } else {

    board.unmakeMove(checkImmediateRight);
  }
}
bool kingInCheck(Board &board, Color color) {
  int kingIndex = findKingIndex(board, color);
  int mailBoxIndex = mailbox64[kingIndex];
  Color oppType = color == white ? black : white;

  std::array<Piece, 5> checkPieces = {n, b, r, q, k};
  for (auto piece : checkPieces) {

    for (int offset : directionOffsets[piece]) {

      for (int i = 1; i <= 8; i++) {

        if ((piece == k or piece == n) and i > 1) {
          break;
        }
        if (mailbox[mailBoxIndex + (offset * i)] != -1 &&
            board.board[mailbox[mailBoxIndex + (offset * i)]].type != color) {
          if (board.board[mailbox[mailBoxIndex + (offset * i)]].piece ==
              piece) {
            // std::cout << "checking piece num" << mailbox[mailBoxIndex +
            // (offset * i)] << std::endl;
            return true; // king is exposed
          } else if (board.board[mailbox[mailBoxIndex + (offset * i)]].type ==
                     oppType) {
            break;
          }
        } else {

          break;
        }
      }
    }
  }
  int pawnOfssets[2] = {9, 11};
  int multiplier = color == white ? -1 : 1;
  for (int os : pawnOfssets) {
    if (mailbox[mailBoxIndex + os * multiplier] != -1 &&
        board.board[mailbox[mailBoxIndex + os * multiplier]].type == oppType &&
        board.board[mailbox[mailBoxIndex + os * multiplier]].piece == p) {
      return true;
    }
  }
  // return false;
  return false; // not in check
}
int findKingIndex(Board &board, Color color) {
  // use a localkingindex variable to cache this value
  int i = 0;
  for (auto square : board.board) {
    if (square.piece == k and square.type == color) {
      return i;
    }
    i += 1;
  }
  return -1; // no king. should never happen
}
// later
std::vector<Move> getPsuedoLegalMoves() {
  std::vector<Move> moves;

  return moves;
}
} // namespace Engine