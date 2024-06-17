#include "Board.hpp"
#include "../Headers/engine.hpp"
// #include "move.hpp"
#include "../utils.hpp"
#include <array>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
namespace Engine {
Board::Board() {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  generateBoardFromFen(fen);
  history = {};
  gameStateHistory = {}; 
}

Board::Board(std::string fen) { 
  generateBoardFromFen(fen);
  history = {};
  gameStateHistory = {}; 

 }
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
          board[i + tc] = emptySquare;
        }
        i += intv - 1;
        break;
      }
      }
      i += 1;
    }
  }
}
void Board::makeMove(Move move) { //updates the board representation given the move 
  bool enpessantToggled = false;
  Piece pieceFrom = board[move._move_from].piece;
  Piece pieceTo = board[move._move_to].piece;
  if (move._isCapture && pieceTo != r && !move._isPromotion && pieceFrom != k) { // simply replace the thing that was previously at that
                         // box
    if (pieceTo == e &&
        board[move._move_from].type == black) {
      // enpessant capture
      board[move._move_to - 8] = emptySquare;
    } else if (pieceTo == e &&
               board[move._move_from].type == white) {
      board[move._move_to + 8] = emptySquare;
    }
    board[move._move_to] = board[move._move_from];
    if (pieceFrom == p) {
      board[move._move_to].jumpCount += 1;
    }
    board[move._move_from] = emptySquare;
    // board[move._move_from].piece = emptySquare;
    std::cout << pieceTo << std::endl;
  } else if (move._isCastle) {

    int file = utils::getFile(move._move_to);

    if (file == 6) {
      // king side castle
      // turn of the respective castle
      board[move._move_to] = board[move._move_from];
      board[move._move_from] = emptySquare;
      board[move._move_to - 1] = board[move._move_to + 1];
      board[move._move_to + 1] = emptySquare;
      if (board[move._move_to].type == white) {
        // flip bit to not allow white king castle
        state.castle_flag &= 0b1110;
      }
      if (board[move._move_to].type == black) {
        // flip bit to not allow black king castle
        state.castle_flag &= 0b1011;
      }
    }
    if (file == 2) { // queen side castle
      board[move._move_to] = board[move._move_from];
      board[move._move_from] = emptySquare;
      board[move._move_to + 1] = board[move._move_to - 2];
      board[move._move_to - 2] = emptySquare;
      if (board[move._move_to].type == white) {
        // flip bit to not allow white queen castle
        state.castle_flag &= 0b1101;
      }
      if (board[move._move_to].type == black) {
        // flip bit to not allow black queen castle
        state.castle_flag &= 0b0111;
      }
    }
  } else if(pieceFrom == k) {
    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
    if(board[move._move_to].type == black) {
      state.castle_flag &= 0b0011;
    }
    if(board[move._move_to].type == white) {
      state.castle_flag &= 0b1100;
    }
  } 
  else if (pieceFrom == r || (move._isCapture && pieceTo == r)) {
    std::cout << "HERE in rook capture" << std::endl;
    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
    if (move._move_from == 63 || move._move_to == 63) {
      // turn off white king side castle
      state.castle_flag &= 0b1110;
    }
    if (move._move_from == 56 || move._move_to == 56) {
      // turn off white queen side castle
      state.castle_flag &= 0b1101;
    }
    if (move._move_from == 0 || move._move_to == 0) {
      // turn off black queen side castle
      state.castle_flag &= 0b0111;
    }
    if (move._move_from == 7 || move._move_to == 7) {
      // turn off black king side castle
      state.castle_flag &= 0b1011;
    }
  }

  // if double jump then mark enpessant
  else if (pieceFrom == p && !move._isPromotion) {

    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
    // if pawn and if pawn did a double jump
    if (move._move_from - move._move_to == -16) { // black double jump
      board[move._move_to].jumpCount = 1;
      state.enpessant = move._move_to - 8;
      enpessantToggled = true;
    } else if (move._move_from - move._move_to == 16) {
      board[move._move_to].jumpCount = 1;
      state.enpessant = move._move_to + 8;
      enpessantToggled = true;
    } else if (move._move_from - move._move_to == -8) { // black single jump
      board[move._move_to].jumpCount += 1;
    } else if (move._move_from - move._move_to == 8) { // single jump
      board[move._move_to].jumpCount += 1;
    }
  }

  // every other regular movement simply replace the piece
  else if (pieceFrom != p && !move._isPromotion) {
    // move_to becomes move_from and then move-from becomes empty
    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
  }
  // handles promootion.
  else if (move._isPromotion) {
    std::cout << "make move saw promotion" << std::endl;
    Square newSquare = {
        .type = board[move._move_from].type,
        .piece = move._toPromote,
        .c = pieceReps[board[move._move_from].type][move._toPromote],
        .jumpCount = 0};
    board[move._move_to] = newSquare;
    board[move._move_from] = emptySquare;
  }
  if (enpessantToggled == false) {
    state.enpessant = -1;
  }
  std::cout << "enpessant " << +state.enpessant << std::endl;
  history.push_back(&move);
  gameStateHistory.push_back(&state); 
  // if promotion then turn pawn into queen, king, whatevs
}

void Board::unmakeMove(Move move, State state) {
  // if(move._isCapture) { //enpessant unmake would be wrong
  //   board[move._move_from] = board[move._move_to];
  //   board[move._move_to].c = pieceReps[board[move._move_from].type][move._capturedPiece];
  //   board[move._move_to].jumpCount = 
  // }
}
void Board::toggleTurn() {
  if (state.turn == black) {
    state.turn = white;
  } else if (state.turn == white) {
    state.turn = black;
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
