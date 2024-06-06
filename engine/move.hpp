#pragma once
#include "../Headers/engine.hpp"
#include <vector>
namespace Engine {
class Move {
public:
  Move(int from, int to, bool isCastle, bool isPromotion, bool isCapture,
       Piece toPromote);
  int _move_from;
  int _move_to;
  bool _isCastle;
  bool _isPromotion;
  bool _isCapture;
  Piece _toPromote;
  static std::vector<Move*> history;
  static std::vector<Move*> getMoveHistory(); 
};
} // namespace Engine
