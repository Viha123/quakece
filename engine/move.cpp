#include "../Headers/engine.hpp"
#include "move.hpp"
#include <iostream>
#include "../utils.hpp"
// #include <vector>

namespace Engine {
  Move::Move(uint8_t from, uint8_t to, bool isCastle, bool isPromotion, bool isCapture,
       Piece toPromote, Piece capturedPiece) {
    _move_from = from;
    _move_to = to;
    _isCastle = isCastle;
    _isPromotion = isPromotion;
    _isCapture = isCapture;
    _toPromote = toPromote;
    _capturedPiece = capturedPiece;
    // history.push_back(this);
  }
  // std::vector<Move*> Move::getMoveHistory() {
  //   return history;
  // }
  void Move::printMove(std::ostream& out) const{
    out << "From: " << int(_move_from) <<
    " To: " <<int(_move_to) <<
    " Is Capture? " << _isCapture << 
    " Captured Piece " << int(_capturedPiece) <<
    " Castle? " << _isCastle <<
    " Promotion? " << _isPromotion <<
    " Promoted Piece " << int(_toPromote) << std::endl;
  }
  void Move::printInChess() {
    std::cout << utils::convertToChessNotation(_move_from)
                 << utils::convertToChessNotation(_move_to) << " " <<
                 std::endl;
  }
  bool Move::operator==(Move other) {
    if(_isCapture == other._isCapture && _move_to == other._move_to && _move_from == other._move_from) {
      return true;
    }
    return false;
  }
}