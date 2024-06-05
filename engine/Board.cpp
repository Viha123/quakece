#include "Board.hpp"
#include "../Headers/engine.hpp"
#include <array>
#include <cctype>
#include <iostream>
#include <string>
namespace Engine {
Board::Board() {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  generateBoardFromFen(fen);
}

Board::Board(std::string fen) { generateBoardFromFen(fen); }
void Board::generateBoardFromFen(std::string fen) {
  std::string token = fen.substr(0, fen.find(" ")); // part 1 of string
  std::string remaining = fen.substr(fen.find(" ") + 1);
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
      std::cout << c << " " <<  trow << std::endl;
      if (std::isalpha(c)) {
        board[i].c = c;
      } else {
        board[i].c = '.';
      }
      std::cout << i << std::endl;

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
          std::cout << i + tc << std::endl;
          board[i + tc].piece = e;
          board[i + tc].type = none;
          board[i+tc].c = '.';
        }
        i += intv - 1;
        std::cout << i << std::endl;
        break;
      }
      }
      i += 1;
    }
  }
}
void Board::display() {
  for (int i = 0; i < 64; i++) {
    if(i % 8 == 0) {
      std::cout << std::endl;
    }
    std::cout << board[i].c << " ";
    
  }
}
} // namespace Engine
