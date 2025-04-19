#include <array>
#include <cassert>
#include <cctype>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
// #include <vector>

#include "../Headers/engine.hpp"
#include "../utils.hpp"
#include "Board.hpp"
namespace Engine {
Board::Board() {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  history = {};
  // state = std::make_unique<State>();
  gameStateHistory.push(state);
  generateBoardFromFen(fen);
  // populatePieceList(white);
  initializeZobristHashing();
  generateZobristKey();
}

Board::Board(std::string fen) {
  history = {};
  // gameStateHistory = {};
  gameStateHistory.push(state);
  generateBoardFromFen(fen);
  initializeZobristHashing();
  generateZobristKey();

  // initializePieceArrays();
  // populatePieceList();
}
Board::Board(const Board &old_board) {
  // std::cout << "Copying board..." << std::endl;

  this->board = old_board.board;
  this->history = old_board.history;
  this->gameStateHistory = old_board.gameStateHistory;
  this->pieceList = old_board.pieceList;
  this->zobristHash = old_board.zobristHash;
  this->zobristKey = old_board.zobristKey;
  this->kingIndexes = old_board.kingIndexes;
  this->state = old_board.state;
  this->w_king_side = old_board.w_king_side;
  this->w_queen_side = old_board.w_queen_side;
  this->b_king_side = old_board.b_king_side;
  this->b_queen_side = old_board.b_queen_side;
  this->emptySquare = old_board.emptySquare;

  // std::cout << "Board copied successfully!" << std::endl;
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
        kingIndexes[white] = i;
        break;
      }
      case 'k': {
        board[i].piece = k;
        board[i].type = black;
        kingIndexes[black] = i;

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
  // white pieces
  int count = 0;
  for (int i = 0; i < 64; i++) {
    if (board[i].type == color) {
      pieceList[color][count] = i;
      // std::cout << color << " " << count << " " << pieceList[color][count] <<
      // std::endl;
      count += 1;
    }
  }
  // std::cout << +color << " " << count << std::endl;
  if (count > 16) {
    display();
    // for(auto i : pieceList[color]) {
    //   std::cout << i << std::endl;
    // }
    // std::cout << count << std::endl;
    for (int i = 0; i < history.size(); i++) {
      Move m = history[i];
      m.printInChess();
    }
    std::cout << toFenString() << std::endl;
  }
  assert(count <= 16);

  if (count < 16) {
    pieceList[color][count] = -1; // signal end of piece count
  }
}
void Board::makeMove(
    Move &move) { // updates the board representation given the move
  bool enpessantToggled = false;
  bool enpessantCapture = false;
  state = gameStateHistory.peek();

  State newState = state;

  Piece pieceFrom = board[move._move_from].piece;
  Piece pieceTo = board[move._move_to].piece;
  Square &move_to_square = board[move._move_to];
  Square &move_from_square = board[move._move_from];
  Color currType = move_from_square.type;
  Color oppType = currType == white ? black : white;

  zobristKey ^=
      zobristHash
          .pieces[pieceFrom][currType]
                 [move._move_from]; // removing piece from the Move_from square
  Piece endPiece = move._isPromotion ? move._toPromote : pieceFrom;
  zobristKey ^=
      zobristHash.pieces[endPiece][currType]
                        [move._move_to]; // adding piece to Move_to square

  if (move._isCapture && pieceTo != r && !move._isPromotion && pieceFrom != k &&
      pieceFrom != r) { // simply replace the thing that was previously at that
                        // box
    if (pieceTo == e && move_from_square.type == black) {
      // enpessant capture
      board[move._move_to - 8] = emptySquare;
      enpessantCapture = true;
    } else if (pieceTo == e && move_from_square.type == white) {
      board[move._move_to + 8] = emptySquare;
      enpessantCapture = true;
    }
    move_to_square = move_from_square;
    move_from_square = emptySquare;
  } else if (move._isCastle) {

    int file = utils::getFile(move._move_to);

    if (file == 6) {
      // king side castle
      // turn of the respective castle
      move_to_square = move_from_square;
      move_from_square = emptySquare;

      board[move._move_to - 1] = board[move._move_to + 1];
      board[move._move_to + 1] = emptySquare;

      if (move_to_square.type == white) {
        // do not allow white castle
        newState.castle_flag &= 0b1100;
      }
      if (move_to_square.type == black) {
        // do not allow black castle
        newState.castle_flag &= 0b0011;
      }
    }
    if (file == 2) { // queen side castle

      move_to_square = move_from_square;
      move_from_square = emptySquare;
      board[move._move_to + 1] = board[move._move_to - 2];
      board[move._move_to - 2] = emptySquare;

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
    }
    kingIndexes[move_to_square.type] = move._move_to;
  } else if (move._isPromotion) {
    // std::cout << "make move saw promotion" << std::endl;
    Square newSquare = {.type = move_from_square.type,
                        .piece = move._toPromote,
                        .c = pieceReps[move_from_square.type][move._toPromote]};
    move_to_square = newSquare;
    move_from_square = emptySquare;
    if (move._capturedPiece == r) {
      handleCastleToggle(move, newState);
    }
    // display();
  } else if (pieceFrom == r || (move._isCapture && pieceTo == r)) {
    // std::cout << "HERE in rook capture" << std::endl;
    move_to_square = move_from_square;
    move_from_square = emptySquare;
    if (pieceFrom == k) {
      kingIndexes[currType] = move._move_to;
      if (move_to_square.type == black) {
        newState.castle_flag &= 0b0011;
      }
      if (move_to_square.type == white) {
        newState.castle_flag &= 0b1100;
      }
    }
    handleCastleToggle(move, newState);
  } else if (pieceFrom == k) {
    move_to_square = move_from_square;
    move_from_square = emptySquare;

    if (move_to_square.type == black) {
      newState.castle_flag &= 0b0011;
    }
    if (move_to_square.type == white) {
      newState.castle_flag &= 0b1100;
    }
    kingIndexes[currType] = move._move_to;
  }

  // if double jump then mark enpessant
  else if (pieceFrom == p && !move._isPromotion) {

    move_to_square = move_from_square;
    move_from_square = emptySquare;

    // if pawn and if pawn did a double jump
    if (move._move_from - move._move_to == -16) { // black double jump
      newState.enpessant = move._move_to - 8;
      enpessantToggled = true;
    } else if (move._move_from - move._move_to == 16) {
      newState.enpessant = move._move_to + 8;
      enpessantToggled = true;
    }

  }
  // every other regular movement simply replace the piece
  else if (pieceFrom != p && !move._isPromotion) {
    // move_to becomes move_from and then move-from becomes empty
    move_to_square = move_from_square;
    move_from_square = emptySquare;
  }
  // handles promootion.

  if (enpessantToggled == false) {
    newState.enpessant = -1;
  } else {

    zobristKey ^= zobristHash.en_pessant[newState.enpessant];
  }
  if (move._isCapture && !enpessantCapture) {
    zobristKey ^=
        zobristHash
            .pieces[pieceTo][oppType][move._move_to]; // remove this from there.
  } else if (move._isCapture && enpessantCapture) {
    int offset = currType == white ? 8 : -8;
    zobristKey ^=
        zobristHash
            .pieces[p][oppType]
                   [move._move_to + offset]; // remove enpessant captured piece
  }
  zobristKey ^= zobristHash.side;
  history.push(move);
  zobristDisableCastle(newState.castle_flag); // only does it once
  gameStateHistory.push(newState);
  toggleTurn();
}

void Board::zobristDisableCastle(const uint8_t &new_castle_flag) {
  uint8_t remaining_castle =
      (~new_castle_flag) &
      gameStateHistory.peek()
          .castle_flag; // compare new and old, and make sure you ONLY toggle
                        // when castle flag is differnt.
  for (int i = 0; i < 4; i++) {
    if ((remaining_castle & (1 << i)) == pow(2, i)) {
      zobristKey ^= zobristHash.castle_rights[i]; // turn off once and for all.
    }
  }
}
void Board::handleCastleToggle(Move &move, State &newState) {

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

    newState.castle_flag &= 0b1011;
  }
}
void Board::unmakeMove(Move &move) {
  // std::cout << "Move to unmake: " << std::endl;
  // display();
  if (gameStateHistory.peek().enpessant != -1) {

    zobristKey ^= zobristHash.en_pessant[gameStateHistory.peek().enpessant];
    // std::cout << "after enpessant move unmade" << zobristKey << std::endl;
  }

  auto currentCastleState = gameStateHistory.peek().castle_flag;
  history.pop();
  gameStateHistory.pop();
  auto previousCastleState = gameStateHistory.peek().castle_flag;
  // std::cout << "Before Castle Un Make: " << zobristKey << std::endl;
  for (int i = 0; i < 4; i++) {
    if ((currentCastleState & (1 << i)) != (previousCastleState & (1 << i))) {
      // restore the old state
      // std::cout << "difference: " << i << std::endl;
      zobristKey ^= zobristHash.castle_rights[i];
    }
  }
  // std::cout << "After Castle Un Make " << zobristKey << std::endl;

  Square &move_to_square = board[move._move_to];
  Square &move_from_square = board[move._move_from];
  Color oppType = move_to_square.type == white ? black : white;
  Color currType = move_to_square.type;
  Piece endPiece = move._isPromotion ? move._toPromote : move_to_square.piece;
  Piece restorePiece = move._isPromotion ? p : move_to_square.piece;
  zobristKey ^= zobristHash.pieces[endPiece][currType]
                                  [move._move_to]; // remove piece from Move_to
  zobristKey ^= zobristHash.pieces[restorePiece][currType]
                                  [move._move_from]; // add piece to Move_from
  // std::cout << " after zobrist move unmade" << zobristKey << std::endl;
  if (move._isCapture && move._move_to != state.enpessant) {
    zobristKey ^= zobristHash.pieces[move._capturedPiece][oppType]
                                    [move._move_to]; // add this to move_t.
  }
  if (move._isCapture && move._move_to == state.enpessant) {
    int offset = currType == white ? 8 : -8;
    zobristKey ^=
        zobristHash
            .pieces[move._capturedPiece][oppType][move._move_to + offset];
  }
  auto &state =
      gameStateHistory.peek(); // state before the move that was deleted
  zobristKey ^= zobristHash.side;
  if (move._isCapture &&
      move._move_to != state.enpessant) { // enpessant unmake would be wrong
    if (!move._isPromotion) {

      move_from_square = move_to_square;
      if (move_to_square.piece == k) {
        kingIndexes[currType] = move._move_from;
      }

    } else {
      // is promotion and is capture
      move_from_square.c = pieceReps[currType][p];
      move_from_square.piece = p;
      move_from_square.type = move_to_square.type;
    }
    move_to_square.c = pieceReps[oppType][move._capturedPiece];

    move_to_square.piece = move._capturedPiece;
    move_to_square.type = oppType;

  } else if (move._isCapture && move._move_to == state.enpessant &&
             state.enpessant != -1) {
    // slightly different unmake
    // restore state
    move_from_square = board[move._move_to]; // moving pawn back to where it was
    move_to_square = emptySquare;            // move to becomes an empty square

    int offset = currType == white ? 8 : -8;
    board[move._move_to + offset].c =
        pieceReps[oppType][p]; // the square below or above gets the
                               // pawn/captured piece back.
    board[move._move_to + offset].piece = p;
    board[move._move_to + offset].type = oppType;

  } else if (move._isCastle) {
    // castle is not possible go back to king and rook being wehre they were and
    // restoring state

    if (move._move_to == 62 || move._move_to == 6) {
      // unmake the king by offset -2 and rook by + 2
      move_from_square = move_to_square;
      move_to_square = emptySquare;
      // // rook by + 2;

      board[move._move_to + 1] = board[move._move_to - 1];
      board[move._move_to - 1] = emptySquare;

    } else if (move._move_to == 58 || move._move_to == 2) {
      // unmake the king by offset -2 and rook by + 2
      // std::cout << "unmaking castle" << std::endl;
      // display();
      move_from_square = move_to_square;
      move_to_square = emptySquare;

      board[move._move_to - 2] = board[move._move_to + 1];
      board[move._move_to + 1] = emptySquare;
    }
    kingIndexes[currType] = move._move_from;
    // castle move undo, zobrist key

  } else if (move._isPromotion && !move._isCapture) {
    // std::cout << "not covered yet" << std::endl;
    Square pawnSquare = {
        .type = state.turn, .piece = p, .c = state.turn == white ? 'P' : 'p'};
    move_from_square = pawnSquare;
    move_to_square = emptySquare;

  } else {
    // quiet move.
    if (move_to_square.piece == k) {
      kingIndexes[move_to_square.type] = move._move_from;
    }
    move_from_square = move_to_square;
    move_to_square = emptySquare;
  }
}

void Board::generateZobristKey() {
  for (int i = 0; i < 64; i++) {
    if (board[i].piece != n) {
      Piece p = board[i].piece;
      Color c = board[i].type;
      zobristKey ^= zobristHash.pieces[p][c][i];
    }
  }
  for (int k = 0; k < 4; k++) {
    if ((gameStateHistory.peek().castle_flag & (1 << k)) == int(pow(2, k))) {
      zobristKey ^= zobristHash.castle_rights[k];
    }
  }
  if (gameStateHistory.peek().enpessant != -1) {
    zobristKey ^= zobristHash.en_pessant[gameStateHistory.peek().enpessant];
  }
  zobristKey ^= zobristHash.side;
}

void Board::initializeZobristHashing() {
  // every array in the Zobrist struct should consist of random numbers
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<long> dis;
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 6; j++) {
      zobristHash.pieces[j][Color::white][i] =
          dis(gen); // empty is not a piecetype and therefore isn't hashed.
      zobristHash.pieces[j][Color::black][i] = dis(gen);
    }
    zobristHash.en_pessant[i] = dis(gen);
  }
  // zobristHash.en_pessant/ = dis(gen);
  for (int k = 0; k < 4; k++) {
    zobristHash.castle_rights[k] = dis(gen);
  }
  zobristHash.side = dis(gen);
  // printZobristHashStruct();
}
void Board::printZobristHashStruct() {
  std::cout << "Zobrist Hash Values:\n";

  // Print piece positions
  for (int pieceType = 0; pieceType < 6; ++pieceType) {
    for (int color = 0; color < 2; ++color) {
      for (int square = 0; square < 64; ++square) {
        std::cout << "Piece Type " << pieceType << ", Color " << color
                  << ", Square " << square << ": "
                  << zobristHash.pieces[pieceType][color][square] << "\n";
      }
    }
  }

  // Print en passant squares
  for (int square = 0; square < 64; ++square) {
    std::cout << "En Passant Square " << square << ": "
              << zobristHash.en_pessant[square] << "\n";
  }

  // Print side to move
  std::cout << "Side to Move: " << zobristHash.side << "\n";
}
inline void Board::toggleTurn() {
  auto &s = gameStateHistory.peek();

  s.turn = s.turn == white ? black : white;
}
// Board::State Board::getState() { return state; }
void Board::initialize_remainding_parameters(std::string remaining) {
  std::string turn = remaining.substr(0, remaining.find(" "));
  remaining = remaining.substr(remaining.find(" ") + 1);
  std::string castling = remaining.substr(0, remaining.find(" ") + 1);
  remaining = remaining.substr(remaining.find(" ") + 1);
  auto &state = gameStateHistory.peek();
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
void Board::display(std::ostream &out) const {
  for (int i = 0; i < 64; i++) {
    if (i % 8 == 0) {
      out << std::endl;
    }
    out << board[i].c << " ";
  }
  out << std::endl;
}
void Board::displayState(State &state) {
  std::cout << "Castle Flag: " << static_cast<int>(state.castle_flag) << '\n';
  std::cout << "Enpessant: " << static_cast<int>(state.enpessant) << '\n';
  std::cout << "Turn: " << (state.turn == white ? "white" : "black") << '\n';

  std::cout << '\n';
}
} // namespace Engine
