#pragma once
#include "../Headers/engine.hpp"
#include <cstdint>
// #include <vector>
namespace Engine {
class Move {
public:
  Move() = default;
  Move(uint8_t from, uint8_t to, bool isCastle, bool isPromotion, bool isCapture,
       Piece toPromote, Piece capturedPiece);
  uint8_t _move_from;
  uint8_t _move_to;
  uint8_t _isCastle;
  bool _isPromotion;
  bool _isCapture;
  Piece _toPromote;
  Piece _capturedPiece;

  void printMove();
  // static std::vector<Move*> history;
  // static std::vector<Move*> getMoveHistory(); 
};
} // namespace Engine
