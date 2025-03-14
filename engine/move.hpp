#pragma once
#include "../Headers/engine.hpp"
#include <cstdint>
#include <iostream>

// #include <vector>
namespace Engine {
class Move {
public:
  Move() = default;
  Move(uint8_t from, uint8_t to, bool isCastle, bool isPromotion,
       bool isCapture, Piece toPromote, Piece capturedPiece);
  uint8_t _move_from;
  uint8_t _move_to;
  uint8_t _isCastle;
  bool _isPromotion;
  bool _isCapture;
  Piece _toPromote;
  Piece _capturedPiece;

  void printMove(std::ostream &out = std::cout) const;
  void printInChess();
  // static std::vector<Move*> history;
  // static std::vector<Move*> getMoveHistory();
  bool operator==(Move other);
};
} // namespace Engine
