#include "../Headers/engine.hpp"
#include "move.hpp"
#include <iostream>
#include <vector>

namespace Engine {
  Move::Move(int from, int to, bool isCastle, bool isPromotion, bool isCapture, Piece toPromote, Piece capturedPiece) {
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
  void Move::printMove() {
    std::cout << "From: " << _move_from <<
    " To: " <<_move_to <<
    " Is Capture? " << _isCapture << 
    " Captured Piece " << _capturedPiece <<
    " Castle? " << _isCastle <<
    " Promotion? " << _isPromotion <<
    " Promoted Piece " << _toPromote << std::endl;
  }

}