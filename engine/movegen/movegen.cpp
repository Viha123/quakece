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
  int rank = utils::getRank(num);
  int file = utils::getFile(num);
  Piece piece = square.piece;
  std::cout << "Beginning of move availability!!!" << std::endl;
  // if this is a sliding piece then here are the available moves for that
  // piece.
  if ((square.type == state.turn) && (isSlide[square.piece])) {
    // go until all directions until end of board (mailbox returns -1)
    int mailBoxIndex = mailbox64[num];

    for (auto offset : directionOffsets[square.piece]) {
      for (int i = 1; i <= 8; i++) {
        int targetIndex = mailbox[(offset)*i + mailBoxIndex]; // test
        if ((targetIndex != -1) && (targetIndex != num) &&
            (board.getSquare(targetIndex).type != state.turn)) {
          //&& mailbox[offset + mailBoxIndex]
          std::cout << targetIndex << " " << board.getSquare(targetIndex).type << std::endl;
          if((board.getSquare(targetIndex).type != state.turn && board.getSquare(targetIndex).type != none)) {
            std::cout << "opp side" << std::endl;
            moves.push_back(targetIndex);
            break;
          }
          moves.push_back(targetIndex);
        
        }
        else {
          break;
        }
      }
    }
  }
  return moves;
}
} // namespace Engine