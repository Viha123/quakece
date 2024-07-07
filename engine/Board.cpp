#include "Board.hpp"
#include "../Headers/engine.hpp"
// #include "move.hpp"
#include "../utils.hpp"
#include <array>
#include <cassert>
#include <cctype>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
namespace Engine {
Board::Board() {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  history = {};
  // state = std::make_unique<State>();

  gameStateHistory.push_back(state);
  generateBoardFromFen(fen);
  populatePieceSet();
}

Board::Board(std::string fen) {
  history = {};
  // gameStateHistory = {};
  gameStateHistory.push_back(state);
  generateBoardFromFen(fen);
  // initializePieceArrays();
  populatePieceSet();
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
void Board::populatePieceSet() {
  for (int i = 0; i < 64; i++) {
    if (board[i].type != none) {
      pieceSets[board[i].type].insert(i);
    }
  }
}
void Board::makeMove(
    Move &move) { // updates the board representation given the move
  bool enpessantToggled = false;
  state = gameStateHistory.back();

  State newState = state;
  // std::cout << "new state created ";
  // displayState(newState);
  Piece pieceFrom = board[move._move_from].piece;
  Piece pieceTo = board[move._move_to].piece;
  Square &move_to_square = board[move._move_to];
  Square &move_from_square = board[move._move_from];
  Color oppType = move_from_square.type == white ? black : white;
  Color currType = move_from_square.type;
  if (move._isCapture && pieceTo != r && !move._isPromotion && pieceFrom != k &&
      pieceFrom != r) { // simply replace the thing that was previously at that
                        // box
    if (pieceTo == e && move_from_square.type == black) {
      // enpessant capture
      // std::cout << "Making enpessant" << std::endl;
      board[move._move_to - 8] = emptySquare;
      pieceSets[oppType].erase(move._move_to - 8);
    } else if (pieceTo == e && move_from_square.type == white) {
      // std::cout << "Making enpessant" << std::endl;
      board[move._move_to + 8] = emptySquare;
      pieceSets[oppType].erase(move._move_to + 8);
    }
    move_to_square = move_from_square;
    move_from_square = emptySquare;
    pieceSets[currType].erase(move._move_from);
    if (pieceSets[oppType].count(move._move_to)) {
      pieceSets[oppType].erase(
          move._move_to); // if capture remove the other person's piece.
    }
    pieceSets[currType].insert(move._move_to);
    // board[move._move_from].piece = emptySquare;
    // std::cout << pieceTo << std::endl;
  } else if (move._isCastle) {

    int file = utils::getFile(move._move_to);

    if (file == 6) {
      // king side castle
      // turn of the respective castle
      move_to_square = move_from_square;
      move_from_square = emptySquare;
      pieceSets[currType].erase(move._move_from);
      pieceSets[currType].insert(move._move_to);
      board[move._move_to - 1] = board[move._move_to + 1];
      board[move._move_to + 1] = emptySquare;
      pieceSets[currType].erase(move._move_to + 1);
      pieceSets[currType].insert(move._move_to - 1);
      if (move_to_square.type == white) {
        // flip bit to not allow white king castle
        newState.castle_flag &= 0b1100;
      }
      if (move_to_square.type == black) {
        // flip bit to not allow black king castle
        newState.castle_flag &= 0b0011;
      }
    }
    if (file == 2) { // queen side castle
      // std::cout << "MAKE CASTLE" << std::endl;
      // display();
      move_to_square = move_from_square;
      move_from_square = emptySquare;
      pieceSets[currType].erase(move._move_from);
      pieceSets[currType].insert(move._move_to);
      board[move._move_to + 1] = board[move._move_to - 2];
      board[move._move_to - 2] = emptySquare;
      pieceSets[currType].erase(move._move_to - 2);
      pieceSets[currType].insert(move._move_to + 1);
      if (move_to_square.type == white) {
        // flip bit to not allow white queen castle
        newState.castle_flag &=
            0b1100; // turn off all castles for that color because once you
                    // castle once you cannot castle again.
      }
      if (move_to_square.type == black) {
        // flip bit to not allow black queen castle
        newState.castle_flag &= 0b0011;
      }
      // display();
    }
  } else if (pieceFrom == r || (move._isCapture && pieceTo == r)) {
    // std::cout << "HERE in rook capture" << std::endl;
    move_to_square = move_from_square;
    move_from_square = emptySquare;
    pieceSets[currType].erase(move._move_from);
    pieceSets[currType].insert(move._move_to);
    if (pieceSets[oppType].count(move._move_to)) {
      pieceSets[oppType].erase(
          move._move_to); // if capture remove the other person's piece.
    }
    // std::cout << "HERE in rook capture" << std::endl;
    handleCastleToggle(move, newState);
    // displayState(newState);
  } else if (pieceFrom == k) {
    move_to_square = move_from_square;
    move_from_square = emptySquare;
    pieceSets[currType].erase(move._move_from);
    pieceSets[currType].insert(move._move_to);
    if (pieceSets[oppType].count(move._move_to)) {
      pieceSets[oppType].erase(
          move._move_to); // if capture remove the other person's piece.
    }
    if (move_to_square.type == black) {
      newState.castle_flag &= 0b0011;
    }
    if (move_to_square.type == white) {
      newState.castle_flag &= 0b1100;
    }
  } else if (move._isPromotion) {
    // std::cout << "make move saw promotion" << std::endl;
    Square newSquare = {.type = move_from_square.type,
                        .piece = move._toPromote,
                        .c = pieceReps[move_from_square.type][move._toPromote]};
    move_to_square = newSquare;
    move_from_square = emptySquare;
    pieceSets[currType].erase(move._move_from);
    pieceSets[currType].insert(move._move_to);
    if (move._capturedPiece == r) {
      handleCastleToggle(move, newState);
    }
    // display();
  }

  // if double jump then mark enpessant
  else if (pieceFrom == p && !move._isPromotion) {

    move_to_square = move_from_square;
    move_from_square = emptySquare;
    pieceSets[currType].erase(move._move_from);
    pieceSets[currType].insert(move._move_to);
    // if pawn and if pawn did a double jump
    if (move._move_from - move._move_to == -16) { // black double jump
      // board[move._move_to].jumpCount = 1;
      newState.enpessant = move._move_to - 8;
      enpessantToggled = true;
    } else if (move._move_from - move._move_to == 16) {
      // board[move._move_to].jumpCount = 1;
      newState.enpessant = move._move_to + 8;
      enpessantToggled = true;
    }

  }
  // every other regular movement simply replace the piece
  else if (pieceFrom != p && !move._isPromotion) {
    // move_to becomes move_from and then move-from becomes empty
    move_to_square = move_from_square;
    move_from_square = emptySquare;
    pieceSets[currType].erase(move._move_from);
    pieceSets[currType].insert(move._move_to);
  }
  // handles promootion.

  if (enpessantToggled == false) {
    newState.enpessant = -1;
  }
  history.push_back(&move);
  // std::cout << "appending new state to history: " << std::endl;
  // displayState(newState);
  // std::cout << "length of gamestate before make" <<  gameStateHistory.size()
  // << std::endl;
  // displayState(gameStateHistory.back());

  // newState.turn = newState.turn == white ? black : white;
  gameStateHistory.push_back(newState);
  // displayState(gameStateHistory.back());
  // std::cout << "length of gamestate after make" <<  gameStateHistory.size()
  // << std::endl;

  toggleTurn();

  // std::cout << "printing updated move" << std::endl;
  // history.back()->printMove();
  // if promotion then turn pawn into queen, king, whatevs
}
void Board::handleCastleToggle(Move &move, State &newState) {
  // std::cout << "here" << std::endl;
  if (move._move_from == 63 || move._move_to == 63) {
    // turn off white king side castle
    newState.castle_flag &= 0b1110;
  }
  if (move._move_from == 56 || move._move_to == 56) {
    // turn off white queen side castle
    newState.castle_flag &= 0b1101;
  }
  if (move._move_from == 0 || move._move_to == 0) {
    // turn off black queen side castle
    newState.castle_flag &= 0b0111;
  }
  if (move._move_from == 7 || move._move_to == 7) {
    // turn off black king side castle
    // std::cout << "here??" << std::endl;
    newState.castle_flag &= 0b1011;
  }
}
void Board::unmakeMove(Move &move) {
  // std::cout << "Move to unmake: " << std::endl;
  // display();
  history.pop_back();
  gameStateHistory.pop_back();
  Square &move_to_square = board[move._move_to];
  Square &move_from_square = board[move._move_from];
  Color oppType = move_to_square.type == white ? black : white;
  Color currType = move_to_square.type;
  // move.printMove();\

  auto &state =
      gameStateHistory.back(); // state before the move that was deleted
  if (move._isCapture &&
      move._move_to != state.enpessant) { // enpessant unmake would be wrong
    if (!move._isPromotion) {
      move_from_square = move_to_square;
    } else {
      // is promotion and is capture
      move_from_square.c = pieceReps[currType][p];
      move_from_square.piece = p;
      move_from_square.type = move_to_square.type;
    }
    move_to_square.c = pieceReps[oppType][move._capturedPiece];

    // board[move._move_to].jumpCount = state->jumpCounts[move._move_to]; //not
    // sure if the usage of state here is correct
    move_to_square.piece = move._capturedPiece;
    move_to_square.type = oppType;
    pieceSets[currType].erase(move._move_to);
    pieceSets[currType].insert(move._move_from);
    pieceSets[oppType].insert(move._move_to);

  } else if (move._isCapture && move._move_to == state.enpessant &&
             state.enpessant != -1) {
    // slightly different unmake
    // restore state
    move_from_square = board[move._move_to]; // moving pawn back to where it was
    move_to_square = emptySquare;            // move to becomes an empty square
    pieceSets[currType].erase(move._move_to);
    pieceSets[currType].insert(move._move_from);
    int offset = currType == white ? 8 : -8;
    board[move._move_to + offset].c =
        pieceReps[oppType][p]; // the square below or above gets the
                               // pawn/captured piece back.
    board[move._move_to + offset].piece = p;
    board[move._move_to + offset].type = oppType;
    pieceSets[oppType].insert(move._move_to + offset);

    // std::cout << "unmaking enpessant" << std::endl;
    // displayState(state);
  } else if (move._isCastle) {
    // castle is not possible go back to king and rook being wehre they were and
    // restoring state
    // std::cout << "unmaking castle" << std::endl;

    if (move._move_to == 62 || move._move_to == 6) {
      // unmake the king by offset -2 and rook by + 2
      move_from_square = move_to_square;
      move_to_square = emptySquare;
      // rook by + 2;
      pieceSets[currType].erase(move._move_to);
      pieceSets[currType].insert(move._move_from);
      board[move._move_to + 1] = board[move._move_to - 1];
      board[move._move_to - 1] = emptySquare;
      pieceSets[currType].erase(move._move_to - 1);
      pieceSets[currType].insert(move._move_to + 1);
    } else if (move._move_to == 58 || move._move_to == 2) {
      // unmake the king by offset -2 and rook by + 2
      // std::cout << "unmaking castle" << std::endl;
      // display();
      move_from_square = move_to_square;
      move_to_square = emptySquare;
      pieceSets[currType].erase(move._move_to);
      pieceSets[currType].insert(move._move_from);
      // rook goes back 2 spots
      // std::cout << move._move_to - 2 << " " << move._move_to + 1 << " " <<
      // move._move_to + 1 << std::endl;
      board[move._move_to - 2] = board[move._move_to + 1];
      board[move._move_to + 1] = emptySquare;
      pieceSets[currType].erase(move._move_to + 1);
      pieceSets[currType].insert(move._move_to - 2);
      // display();
    }
  } else if (move._isPromotion && !move._isCapture) {
    // std::cout << "not covered yet" << std::endl;
    Square pawnSquare = {
        .type = state.turn, .piece = p, .c = state.turn == white ? 'P' : 'p'};
    move_from_square = pawnSquare;
    move_to_square = emptySquare;
    pieceSets[currType].erase(move._move_to);
    pieceSets[currType].insert(move._move_from);
  } else {
    // quiet move.
    move_from_square = move_to_square;
    move_to_square = emptySquare;
    pieceSets[currType].erase(move._move_to);
    pieceSets[currType].insert(move._move_from);
  }
}
inline void Board::toggleTurn() {
  auto& s = gameStateHistory.back();
  
  
  if(gameStateHistory.back().turn == white) {
    gameStateHistory.back().turn = black;
  } else {

    gameStateHistory.back().turn = white;

  }
  
}
// Board::State Board::getState() { return state; }
void Board::initialize_remainding_parameters(std::string remaining) {
  std::string turn = remaining.substr(0, remaining.find(" "));
  remaining = remaining.substr(remaining.find(" ") + 1);
  std::string castling = remaining.substr(0, remaining.find(" ") + 1);
  remaining = remaining.substr(remaining.find(" ") + 1);
  auto &state = gameStateHistory.back();
  if (turn[0] == 'w') {
    state.turn = white;
  } else {
    state.turn = black;
  }
  // std::cout << castling.find("-1") << std::endl;
  if (castling.find("k") >= 0 and castling.find("k") < 4) { // black king side
    state.castle_flag |= 0b0100;
  }
  if (castling.find("K") >= 0 and castling.find("K") < 4) { // white king side
    state.castle_flag |= 0b0001;
  }
  if (castling.find("q") >= 0 and castling.find("q") < 4) { // black queen side
    state.castle_flag |= 0b1000;
  }
  if (castling.find("Q") >= 0 and castling.find("Q") < 4) { // white queen side
    state.castle_flag |= 0b0010;
  }
  // en pessant
  std::string en_string = remaining.substr(0, remaining.find(" "));
  // std::cout << en_string << std::endl;
  if (en_string.find("-") == 0) {
    state.enpessant = -1;
  } else {

    state.enpessant = utils::getNumFromStr(en_string);
  }
}
Board::Square Board::getSquare(int num) {
  if (num < 0 or num > 63) {
    std::cout << num << std::endl;
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
void Board::displayState(State &state) {
  std::cout << "Castle Flag: " << static_cast<int>(state.castle_flag) << '\n';
  std::cout << "Enpessant: " << static_cast<int>(state.enpessant) << '\n';
  std::cout << "Turn: " << (state.turn == white ? "white" : "black") << '\n';

  std::cout << '\n';
}
} // namespace Engine
