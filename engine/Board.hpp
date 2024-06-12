#pragma once
#include "../Headers/engine.hpp"
#include <array>
#include <string>
#include "move.hpp"
namespace Engine {
class Board {
public:
  struct Square {
    Color type; // piece color
    Piece piece;
    char c; // for display purposes
    int jumpCount; // if this piece is a pawn then we need to count its jump to see if it can enpassant possibility
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
  Square getSquare(int num); //get piece at index
  Square emptySquare = {
    .type = none,
    .piece = e,
    .c = '.',
    .jumpCount = 0
  };
  void makeMove(Move move); //make move and update the board with the result of that move
  void unmakeMove(Move move); //if move results in a potential check from opp side, you must undo the move.
  void toggleTurn();
private:
  void generateBoardFromFen(
      std::string fen); // updates board array and also initializes all the
                        // other variables such as enpessant, castle rights etc.
  void initialize_remainding_parameters(
      std::string remaining); // this initializes castle enassatns and turns.
};
} // namespace Engine
