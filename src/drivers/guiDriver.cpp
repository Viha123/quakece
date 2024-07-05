#include "guiDriver.hpp"
#include "../../Headers/engine.hpp"
#include "../../Headers/gui.hpp"
#include "../../engine/Board.hpp"
#include "../../engine/move.hpp"
#include "../../engine/movegen/movegen.hpp"
#include "../../utils.hpp"
#include "../gui/BoardDisplay.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

guiDriver::guiDriver()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"),
      fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
      guiBoard(fen) , 
      board(fen) {
  initialize_char_to_piece();
  window.draw(guiBoard); // only rerender when required
  window.display();
}
guiDriver::guiDriver(std::string fen)
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"),
      fen(fen),
      guiBoard(fen) , 
      board(fen) {
  initialize_char_to_piece();
  window.draw(guiBoard); // only rerender when required
  window.display();
}
void guiDriver::updateWindow(sf::RenderWindow &window, BoardDisplay &guiBoard) {
  window.clear();
  window.draw(guiBoard);
  window.display();
}
void guiDriver::initialize_char_to_piece() {
  char_to_piece['q'] = q;
  char_to_piece['Q'] = q;
  char_to_piece['n'] = n;
  char_to_piece['N'] = n;
  char_to_piece['b'] = b;
  char_to_piece['B'] = b;
  char_to_piece['R'] = r;
  char_to_piece['r'] = r;
}

void guiDriver::handle_gui_promotion(Piece &promoted_piece,
                                     std::vector<Engine::Move> moves,
                                     int piece_from, int piece_to) {
  bool foundPiece = false;
  for (auto move : moves) {
    if (piece_from == move._move_from && piece_to == move._move_to) {
      char promotion;

      std::cout << "Q for queen, N for knight, B for bishop, R for "
                   "rook promotion"
                << std::endl;
      std::cin >> promotion;

      promoted_piece = char_to_piece[promotion];
      foundPiece = true;
      break;
    }
  }
  promoted_piece = foundPiece ? promoted_piece : e;
}
void guiDriver::play() {

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && options == false) {
          guiBoard.clearPossibleMoves();
          piece_from = guiBoard.getPieceClicked(event.mouseButton.x,
                                                event.mouseButton.y);

          moves.clear();
          Engine::getLegalMovesForPiece(board, piece_from, moves);
          

          guiBoard.highlightPossibleMoves(moves);

          options = true;
          updateWindow(window, guiBoard);
        } else if (event.mouseButton.button == sf::Mouse::Left &&
                   options == true) {
          // the player is clicking on a MOVE_TO OR AN INVALID MOVE.

          options = false;
          // check if player is clicking on a valid move:

          int piece = guiBoard.getPieceClicked(event.mouseButton.x,
                                               event.mouseButton.y);
          Piece promoted_piece = e;
          auto& currentState = board.gameStateHistory.back();
          // board.displayState(currentState);
          if (utils::getRank(piece) == 7 and currentState->turn == black and
              board.board[piece_from].piece == p) {
            handle_gui_promotion(promoted_piece, moves, piece_from, piece);
          }
          if (utils::getRank(piece) == 0 and currentState->turn == white and
              board.board[piece_from].piece == p) {
            handle_gui_promotion(promoted_piece, moves, piece_from, piece);
          }

          for (auto i : moves) {
            if (piece_from == i._move_from && i._move_to == piece) {
              if (promoted_piece == i._toPromote) {
                piece_to = piece;
                // board.display();
                board.makeMove(i);
                std::cout << "_________________________________" << std::endl;
                guiBoard.clearPossibleMoves();
                // i.printMove();
                guiBoard.updateMove(i);
                // Engine::Move::history.push_back(&i);
                updateWindow(window, guiBoard);

                board.display();


                auto& latestState =
                    board.gameStateHistory.back();
                std::cout << "before legal moves" << std::endl;

                board.displayState(latestState);
                // board.display();
                allMoves.clear();
                Engine::getLegalMoves(board, allMoves);
                // std::cout << "after legal moves" << std::endl;

                // board.display();

                if(allMoves.size() == 0) {
                  std::cout << "CHECKMATE" << std::endl;
                  std::cout << latestState->turn << " " << "LOSES" << std::endl;
                  // window.close();
                }
              }
            }
          }

        } else if (event.mouseButton.button == sf::Mouse::Right) {
          if (board.history.size() == 0 or board.gameStateHistory.size() == 0) {
            throw std::out_of_range("cannot unmake a move that is not made");
          }
          Engine::Move moveToUnmake = *board.history.back();
          moveToUnmake.printMove();
          
          // board.display();
          board.unmakeMove(moveToUnmake);
          updateWindow(window, guiBoard);
          // board.display();
        }
      }
    }
    // else if (*argv[1] == '1') { //main will call this instead
    //   if (event.type == sf::Event::MouseButtonPressed) {
    //     if (event.mouseButton.button == sf::Mouse::Left) {
    //     }
    //   }
    // }
  }
}

// void guiDriver::handleRightClickUnmake() {
//   guiBoard.clearPossibleMoves();
//   int piece =
//       guiBoard.getPieceClicked(event.mouseButton.x, event.mouseButton.y);

//   moves = Engine::getMoveForPiece(board, piece);

//   guiBoard.highlightPossibleMoves(moves);

//   updateWindow(window, guiBoard);
// }