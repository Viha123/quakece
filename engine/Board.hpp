#pragma once
#include "../Headers/engine.hpp"
#include <array>
#include <string>
namespace Engine {
class Board {
public:
  struct Square {
    Color type; //piece color
    Piece piece;
    char c; //for display purposes
  };
  int16_t castle_flag = 0b1111; // 1: wk, 2: wq, 4: bk, 8: bq
  int8_t enpessant_white = 0b0; // all of them are not allowed in the start
  int8_t enpassant_black = 0b0;
  Color turn = white; // white to move
  std::array<Square, 64> board{};
  Board();
  Board(std::string fen);
  void display(); //display board for testing purposes
private:
  void generateBoardFromFen(std::string fen); //updates board array and also initializes all the other variables such as enpessant, castle rights etc.
};
} // namespace Engine
