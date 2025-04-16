#pragma once

#include <chrono>
#include "../../engine/move.hpp"
#include "../../engine/Board.hpp"
#include "../../engine/TranspositionTable.hpp"
class Computer {
  public:
  Computer();
  Engine::Move makeMove(Engine::Board& board);
  Color m_side;
  void setSide(Color side);
  Engine::TranspositionTable tt;

};