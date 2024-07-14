#pragma once
#include "../../engine/move.hpp"
#include "../../engine/Board.hpp"
class Computer {
  public:
  Computer();
  Engine::Move makeMove(Engine::Board& board);
  Color m_side;
  void setSide(Color side);

};