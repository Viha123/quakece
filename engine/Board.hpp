#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <string>

#include "move.hpp"

#include "../Headers/engine.hpp"
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

  // structures for Zobrist Hashing
  // std::array<std::array<>long[6][2][64] PIECES;
  struct Zobrist {
    std::array<std::array<std::array<long, 64>, 2>, 6>
        pieces; // [piece type (k,q,r,b,k,p)][black/white][squares]
    std::array<long, 64> en_pessant;   // 64 possible spots for enpessatant
    std::array<long, 4> castle_rights; // 1: wk, 2: wq, 4: bk, 8: bq
    long side;
  };
  Zobrist zobristHash;
  long zobristKey = 0;
  int w_king_side = 0;
  int w_queen_side = 1;
  int b_king_side = 3;
  int b_queen_side = 2;

public:
  void makeMove(Move &move);   // make move and update the board with the result
  void unmakeMove(Move &move); // undo the move.
  void toggleTurn();
  void displayState(State &state);
  void display(std::ostream &out =
                   std::cout) const; // display board for testing purposes
  Square getSquare(int num);         // get piece at index
  void populatePieceList(Color color);
  std::string toFenString();
  std::string squareToNotation(int square);
  void populatePieceSet();
  void generateZobristKey();
  void printZobristHashStruct();
  // int whiteKingIndex = 60;
  // int blackKingIndex = 4;
  std::array<int, 2> kingIndexes = {60, 4};

private:
  State state;
  Square emptySquare = {.type = none, .piece = e, .c = '.'};
  void generateBoardFromFen(
      std::string fen); // updates board array and also initializes all the
                        // other variables such as enpessant, castle rights etc.
  void initialize_remainding_parameters(
      std::string remaining); // this initializes castle enassatns and turns.
  void handleCastleToggle(Move &move, State &newState);
  void initializeZobristHashing();
  void zobristDisableBlackCastle();
  void zobristDisableWhiteCastle();
  void zobristDisableCastle(const uint8_t& new_castle_flag);
};
} // namespace Engine
