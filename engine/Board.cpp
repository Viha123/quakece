#include "Board.hpp"
#include "../Headers/engine.hpp"
// #include "move.hpp"
#include "../utils.hpp"
#include <array>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cassert>

namespace Engine {
Board::Board() {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  history = {};
  gameStateHistory = {};
  state = new State();
  gameStateHistory.push_back(state);
  generateBoardFromFen(fen);
}

Board::Board(std::string fen) {
  history = {};
  gameStateHistory = {};
  state = new State();
  gameStateHistory.push_back(state);
  generateBoardFromFen(fen);
  // initializePieceArrays();
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
void Board::populatePieceList(Color color) {
  //white pieces
  int count = 0;
  for(int i = 0; i < 64; i ++) {
    if (board[i].type == color) {
      pieceList[color][count] = i;
      // std::cout << color << " " << count << " " << pieceList[color][count] << std::endl;
      count += 1;
    }
  }
  assert(count <= 16); 
  if (count < 16) {
    pieceList[color][count] = -1; //signal end of piece count
  }

  
}
void Board::makeMove(
    Move &move) { // updates the board representation given the move
  bool enpessantToggled = false;
  state = gameStateHistory.back();
  auto *newState = new State(*state);
  // std::cout << "new state created ";
  // displayState(newState);
  Piece pieceFrom = board[move._move_from].piece;
  Piece pieceTo = board[move._move_to].piece;
  if (move._isCapture && pieceTo != r && !move._isPromotion &&
      pieceFrom != k) { // simply replace the thing that was previously at that
                        // box
    if (pieceTo == e && board[move._move_from].type == black) {
      // enpessant capture
      board[move._move_to - 8] = emptySquare;
    } else if (pieceTo == e && board[move._move_from].type == white) {
      board[move._move_to + 8] = emptySquare;
    }
    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
    // board[move._move_from].piece = emptySquare;
    // std::cout << pieceTo << std::endl;
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
        newState->castle_flag &= 0b1100;
      }
      if (board[move._move_to].type == black) {
        // flip bit to not allow black king castle
        newState->castle_flag &= 0b0011;
      }
    }
    if (file == 2) { // queen side castle
      board[move._move_to] = board[move._move_from];
      board[move._move_from] = emptySquare;
      board[move._move_to + 1] = board[move._move_to - 2];
      board[move._move_to - 2] = emptySquare;
      if (board[move._move_to].type == white) {
        // flip bit to not allow white queen castle
        newState->castle_flag &=
            0b1100; // turn off all castles for that color because once you
                    // castle once you cannot castle again.
      }
      if (board[move._move_to].type == black) {
        // flip bit to not allow black queen castle
        newState->castle_flag &= 0b0011;
      }
    }
  } else if (pieceFrom == k) {
    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
    if (board[move._move_to].type == black) {
      newState->castle_flag &= 0b0011;
    }
    if (board[move._move_to].type == white) {
      newState->castle_flag &= 0b1100;
    }
  } else if (move._isPromotion) {
    std::cout << "make move saw promotion" << std::endl;
    Square newSquare = {
        .type = board[move._move_from].type,
        .piece = move._toPromote,
        .c = pieceReps[board[move._move_from].type][move._toPromote]};
    board[move._move_to] = newSquare;
    board[move._move_from] = emptySquare;
    if (move._capturedPiece == r) {
      handleCastleToggle(move, newState);
    }
  } else if (pieceFrom == r || (move._isCapture && pieceTo == r)) {
    // std::cout << "HERE in rook capture" << std::endl;
    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
    handleCastleToggle(move, newState);
  }

  // if double jump then mark enpessant
  else if (pieceFrom == p && !move._isPromotion) {

    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
    // if pawn and if pawn did a double jump
    if (move._move_from - move._move_to == -16) { // black double jump
      // board[move._move_to].jumpCount = 1;
      newState->enpessant = move._move_to - 8;
      enpessantToggled = true;
    } else if (move._move_from - move._move_to == 16) {
      // board[move._move_to].jumpCount = 1;
      newState->enpessant = move._move_to + 8;
      enpessantToggled = true;
    }

  }

  // every other regular movement simply replace the piece
  else if (pieceFrom != p && !move._isPromotion) {
    // move_to becomes move_from and then move-from becomes empty
    board[move._move_to] = board[move._move_from];
    board[move._move_from] = emptySquare;
  }
  // handles promootion.

  if (enpessantToggled == false) {
    newState->enpessant = -1;
  }
  history.push_back(&move);
  // std::cout << "appending new state to history: " << std::endl;
  // displayState(newState);
  gameStateHistory.push_back(newState);
  // std::cout << "printing updated move" << std::endl;
  // history.back()->printMove();
  // if promotion then turn pawn into queen, king, whatevs
}
void Board::handleCastleToggle(Move move, State *newState) {
  if (move._move_from == 63 || move._move_to == 63) {
    // turn off white king side castle
    newState->castle_flag &= 0b1110;
  }
  if (move._move_from == 56 || move._move_to == 56) {
    // turn off white queen side castle
    newState->castle_flag &= 0b1101;
  }
  if (move._move_from == 0 || move._move_to == 0) {
    // turn off black queen side castle
    newState->castle_flag &= 0b0111;
  }
  if (move._move_from == 7 || move._move_to == 7) {
    // turn off black king side castle
    newState->castle_flag &= 0b1011;
  }
}
void Board::unmakeMove(Move move) {
  // std::cout << "Move to unmake: " << std::endl;
  history.pop_back();
  gameStateHistory.pop_back();
  Color oppType = board[move._move_to].type == white ? black : white;
  Color currType = board[move._move_to].type;
  // move.printMove();
  State *state =
      gameStateHistory.back(); // state before the move that was deleted
  if (move._isCapture &&
      move._move_to != state->enpessant) { // enpessant unmake would be wrong
    if (!move._isPromotion) {
      board[move._move_from] = board[move._move_to];
    } else {
      // is promotion and is capture
      board[move._move_from].c = pieceReps[currType][p];

      board[move._move_from].piece = p;
      board[move._move_from].type = board[move._move_to].type;
    }
    board[move._move_to].c = pieceReps[oppType][move._capturedPiece];

    // board[move._move_to].jumpCount = state->jumpCounts[move._move_to]; //not
    // sure if the usage of state here is correct
    board[move._move_to].piece = move._capturedPiece;
    board[move._move_to].type = oppType;

  } else if (move._isCapture && move._move_to == state->enpessant && state->enpessant != -1) {
    // slightly different unmake
    // restore state
    board[move._move_from] =
        board[move._move_to];           // moving pawn back to where it was
    board[move._move_to] = emptySquare; // move to becomes an empty square
    int offset = currType == white ? 8 : -8;
    board[move._move_to + offset].c =
        pieceReps[oppType][p]; // the square below or above gets the
                               // pawn/captured piece back.
    board[move._move_to + offset].piece = p;
    board[move._move_to + offset].type = oppType;
    std::cout << "unmaking enpessant" << std::endl;
    // displayState(state);
  } else if (move._isCastle) {
    // castle is not possible go back to king and rook being wehre they were and
    // restoring state
    std::cout << "unmaking castle" << std::endl;

    if (move._move_to == 62 || move._move_to == 6) {
      // unmake the king by offset -2 and rook by + 2
      board[move._move_from] = board[move._move_to];
      board[move._move_to] = emptySquare;
      // rook by + 2;
      board[move._move_to + 1] = board[move._move_to - 1];
      board[move._move_to - 1] = emptySquare;
    } else if (move._move_to == 58 || move._move_to == 2) {
      // unmake the king by offset -2 and rook by + 2
      board[move._move_from] = board[move._move_to];
      board[move._move_to] = emptySquare;
      // rook goes back 2 spots
      board[move._move_to - 2] = board[move._move_to + 1];
      board[move._move_to + 1] = emptySquare;
    }
  } else {
    // quiet move.
    board[move._move_from] = board[move._move_to];
    board[move._move_to] = emptySquare;
  }
}
void Board::toggleTurn() {
  State *s = gameStateHistory.back();
  if (s->turn == black) {
    s->turn = white;
  } else if (s->turn == white) {
    s->turn = black;
  }
}
// Board::State Board::getState() { return state; }
void Board::initialize_remainding_parameters(std::string remaining) {
  std::string turn = remaining.substr(0, remaining.find(" "));
  remaining = remaining.substr(remaining.find(" ") + 1);
  std::string castling = remaining.substr(0, remaining.find(" ") + 1);
  remaining = remaining.substr(remaining.find(" ") + 1);
  if (turn[0] == 'w') {
    state->turn = white;
  } else {
    state->turn = black;
  }
  // std::cout << castling.find("-1") << std::endl;
  if (castling.find("k") >= 0 and castling.find("k") < 4) { // black king side
    state->castle_flag |= 0b0100;
  }
  if (castling.find("K") >= 0 and castling.find("K") < 4) { // white king side
    state->castle_flag |= 0b0001;
  }
  if (castling.find("q") >= 0 and castling.find("q") < 4) { // black queen side
    state->castle_flag |= 0b1000;
  }
  if (castling.find("Q") >= 0 and castling.find("Q") < 4) { // white queen side
    state->castle_flag |= 0b0010;
  }
  // en pessant
  std::string en_string = remaining.substr(0, remaining.find(" "));
  // std::cout << en_string << std::endl;
  if (en_string.find("-") == 0) {
    state->enpessant = -1;
  } else {

    state->enpessant = utils::getNumFromStr(en_string);
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
void Board::displayState(State *state) {
  std::cout << "Castle Flag: " << static_cast<int>(state->castle_flag) << '\n';
  std::cout << "Enpessant: " << static_cast<int>(state->enpessant) << '\n';
  std::cout << "Turn: " << (state->turn == white ? "white" : "black") << '\n';

  std::cout << '\n';
}
} // namespace Engine
