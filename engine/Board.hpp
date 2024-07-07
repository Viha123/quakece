#pragma once
#include "../Headers/engine.hpp"
#include "move.hpp"
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <unordered_set>
#include "../src/FixedStack.hpp"
// #include <vector>
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

  std::array<Square, 64> board{};
  Board();
  Board(std::string fen);
  FixedStack<Move, 512> history;
  FixedStack<State, 512> gameStateHistory; // keeps track of game state
  std::array<std::array<int, 16>, 2> pieceList;
  // std::array<std::unordered_set<int>,2> pieceSets; //0 for white 1 for black
  
public:
  void makeMove(Move &move);  // make move and update the board with the result
  void unmakeMove(Move &move); // undo the move.
  void toggleTurn();
  void displayState(State& state);
  void display();            // display board for testing purposes
  Square getSquare(int num); // get piece at index
  void populatePieceList(Color color);
  std::string toFenString();
  std::string squareToNotation(int square);
  void populatePieceSet();
  // int whiteKingIndex = 60;
  // int blackKingIndex = 4;
  std::array<int, 2> kingIndexes = {60,4};
private:
  State state;
  Square emptySquare = {.type = none, .piece = e, .c = '.'};
  void generateBoardFromFen(
      std::string fen); // updates board array and also initializes all the
                        // other variables such as enpessant, castle rights etc.
  void initialize_remainding_parameters(
      std::string remaining); // this initializes castle enassatns and turns.
  void handleCastleToggle(Move& move, State& newState);
};
} // namespace Engine
