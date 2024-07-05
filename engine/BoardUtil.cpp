#include <sstream> // For std::ostringstream
#include "Board.hpp"
namespace Engine {

std::string Board::toFenString() {
  std::ostringstream fen;
  int emptyCount = 0;
  auto& lastState = *gameStateHistory.back();
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {
      Square sq = getSquare(rank * 8 + file);
      if (sq.piece == e) { 
        ++emptyCount;
      } else {
        if (emptyCount > 0) {
          fen << emptyCount;
          emptyCount = 0;
        }
        char pieceChar = sq.c; // Implement this function based on your Piece and Color enums
        fen << pieceChar;
      }
    }
    if (emptyCount > 0) {
      fen << emptyCount;
      emptyCount = 0;
    }
    if (rank < 7) fen << '/';
  }

  // Active color
  fen << ' ' << (lastState.turn == white ? 'w' : 'b');

  // Castling availability
  fen << ' ';
  if (lastState.castle_flag == 0) {
    fen << '-';
  } else {
    if (lastState.castle_flag & 0b0001) fen << 'K';
    if (lastState.castle_flag & 0b0010) fen << 'Q';
    if (lastState.castle_flag & 0b0100) fen << 'k';
    if (lastState.castle_flag & 0b1000) fen << 'q';
  }

  // En passant target square
  fen << ' ' << (lastState.enpessant == -1 ? "-" : squareToNotation(lastState.enpessant)); // Implement squareToNotation based on your system

  // Assuming halfmove and fullmove counters are not tracked, use placeholders
  fen << " 0 1";

  return fen.str();
}


// Helper function to convert square index to algebraic notation for en passant
std::string Board::squareToNotation(int square) {
  // Assuming square is an integer from 0 to 63
  int rank = 8 - (square / 8);
  char file = 'a' + (square % 8);
  return std::string(1, file) + std::to_string(rank);
}

} // namespace Engine