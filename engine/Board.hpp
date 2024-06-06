#pragma once
#include "../Headers/engine.hpp"
#include <array>
#include <string>
namespace Engine {
class Board {
public:
  struct Square {
    Color type; // piece color
    Piece piece;
    char c; // for display purposes
  };
  struct State {
    int8_t castle_flag = 0b0000; // 1: wk, 2: wq, 4: bk, 8: bq
    int8_t enpessant = -1;
    Color turn = white; // white to move
  };
  State state; 
  std::array<Square, 64>
      board{};
  Board();
  Board(std::string fen);
  void display(); // display board for testing purposes
  State getState();
private:
  void generateBoardFromFen(
      std::string fen); // updates board array and also initializes all the
                        // other variables such as enpessant, castle rights etc.
  void initialize_remainding_parameters(
      std::string remaining); // this initializes castle enassatns and turns.
};
} // namespace Engine
