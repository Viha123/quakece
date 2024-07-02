#pragma once
#include "../../Headers/engine.hpp"
#include <vector>
#include "../move.hpp"
#include "../Board.hpp"
namespace Engine {
  //2 different types of inputs:
  //return list of available moves given board and given index of board pressed 
  //for example if w or black rook is pressed give available list of moves
  std::vector<Move> getLegalMoves(Board& board); //gets all moves given board 
  std::vector<Move> getPsuedoLegalMoves(Board& board);
  void handlePromotions(std::vector<Move> &moves, int numFrom, int numTo, Piece capturedPiece);
  // std::vector<Move> getSliderMoves(Board& board); //Rook, bishop, queen, king
  // std::vector<Move> getJumpMoves(Board& board); //
  std::vector<Move> getMoveForPiece(Board& board, int num); // this is only for the gui and easier testing. 
  //then conver thsi into slider, jump pseudolegal moves etc
  std::vector<Move> getLegalMovesForPiece(Board& board, int num);
  
  int findKingIndex(Board& board, Color color);
  bool kingInCheck(Board& board, Color color);
  void handleKingCheck(Board &board, int offset, Move& move, Color color, std::vector<Move>& legal);
}