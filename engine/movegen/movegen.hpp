#pragma once
#include "../../Headers/engine.hpp"
#include "../Board.hpp"
#include "../move.hpp"
// #include <vector>
namespace Engine {
// 2 different types of inputs:
// return list of available moves given board and given index of board pressed
// for example if w or black rook is pressed give available list of moves
void getLegalMoves(Board &board, FixedStack<Move,256>& allMoves); // gets all moves given board
std::vector<Move> getPsuedoLegalMoves(Board &board);
void handlePromotions(FixedStack<Move, 64> &moves, int numFrom, int numTo,
                      Piece capturedPiece);
// std::vector<Move> getSliderMoves(Board& board); //Rook, bishop, queen, king
// std::vector<Move> getJumpMoves(Board& board); //
// then conver thsi into slider, jump pseudolegal moves etc
void getLegalMovesForPiece(Board &board, int num, FixedStack<Move,256>& legal);
void getMoveForPiece(
    Board &board, int num,
    FixedStack<Move, 64>& moves); // this is only for the gui and easier testing.

int findKingIndex(Board &board, Color color);
bool kingInCheck(Board &board, Color color);
void handleKingCheck(Board &board, int offset, Move &move, Color color,
                     FixedStack<Move,256>& legal);
} // namespace Engine