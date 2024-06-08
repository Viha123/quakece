#include "Board.hpp"
#include "../Headers/engine.hpp"
#include <array>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../utils.hpp"
namespace Engine {
Board::Board() {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  generateBoardFromFen(fen);
}

Board::Board(std::string fen) { generateBoardFromFen(fen); }
void Board::generateBoardFromFen(std::string fen) {
  std::string token = fen.substr(0, fen.find(" ")); // part 1 of string
  std::string remaining = fen.substr(fen.find(" ") + 1);
  initialize_remainding_parameters(remaining);
  int row = 0;
  int i = 0;
  while (token.length() != 0) {
    row += 1;
    if (row > 8) {
      break;
    }
    int end = token.find("/");
    std::string trow = token.substr(0, end);
    token = token.substr(end + 1);

    for (char c : trow) {
      if (std::isalpha(c)) {
        board[i].c = c;
      } else {
        board[i].c = '.';
      }

      switch (c) {
      case 'B': {
        board[i].piece = b;
        board[i].type = white;
        break;
      }
      case 'b': {
        board[i].piece = b;
        board[i].type = black;
        break;
      }
      case 'P': {
        board[i].piece = p;
        board[i].type = white;
        break;
      }
      case 'p': {
        board[i].piece = p;
        board[i].type = black;
        break;
      }
      case 'N': {
        board[i].piece = n;
        board[i].type = white;
        break;
      }
      case 'n': {
        board[i].piece = n;
        board[i].type = black;
        break;
      }
      case 'R': {
        board[i].piece = r;
        board[i].type = white;
        break;
      }
      case 'r': {
        board[i].piece = r;
        board[i].type = black;
        break;
      }
      case 'Q': {
        board[i].piece = q;
        board[i].type = white;
        break;
      }
      case 'q': {
        board[i].piece = q;
        board[i].type = black;
        break;
      }
      case 'K': {
        board[i].piece = k;
        board[i].type = white;
        break;
      }
      case 'k': {
        board[i].piece = k;
        board[i].type = black;
        break;
      }
      default: { // is an integer and need to skip squares
        int intv = c - '0';

        for (int tc = 0; tc < intv; tc += 1) {
          board[i + tc].piece = e;
          board[i + tc].type = none;
          board[i + tc].c = '.';
        }
        i += intv - 1;
        break;
      }
      }
      i += 1;
    }
  }
}
Board::State Board::getState() { return state; }
void Board::initialize_remainding_parameters(std::string remaining) {
  std::string turn = remaining.substr(0, remaining.find(" "));
  remaining = remaining.substr(remaining.find(" ") + 1);
  std::string castling = remaining.substr(0, remaining.find(" ") + 1);
  remaining = remaining.substr(remaining.find(" ") + 1);
  if (turn[0] == 'w') {
    state.turn = white;
  } else {
    state.turn = black;
  }
  // std::cout << castling.find("-1") << std::endl;
  if (castling.find("k") >= 0 and castling.find("k") < 4) { // black king side
    // std::cout << "black king side!!!" << std::endl;

    state.castle_flag |= 0b0100;
  }
  if (castling.find("K") >= 0 and castling.find("K") < 4) { // white king side
    // std::cout << "white king side!!!" << std::endl;

    state.castle_flag |= 0b0001;
  }
  if (castling.find("q") >= 0 and castling.find("q") < 4) { // black queen side
    // std::cout << "black queen side!!!" << std::endl;

    state.castle_flag |= 0b1000;
  }
  if (castling.find("Q") >= 0 and castling.find("Q") < 4) { // white queen side
    // std::cout << "white queen side!!!" << std::endl;

    state.castle_flag |= 0b0010;
  }
  // en pessant
  std::string en_string = remaining.substr(0, remaining.find(" "));
  std::cout << en_string << std::endl;
  if (en_string.find("-") == 0) {
    state.enpessant = -1;
  } else {
    // need to make a utils.cpp to convert chess notation to numbers
    // TODO
    state.enpessant = utils::getNumFromStr(en_string);
    std::cout << +state.enpessant << std::endl;
  }
}
Board::Square Board::getSquare(int num) {
  if (num < 0 or num > 63) {
    throw std::invalid_argument("index must be between 0 and 63");
  } else {
    return board[num];
  }
}
void Board::display() {
  for (int i = 0; i < 64; i++) {
    if (i % 8 == 0) {
      std::cout << std::endl;
    }
    std::cout << board[i].c << " ";
  }
  std::cout << std::endl;
}
} // namespace Engine
