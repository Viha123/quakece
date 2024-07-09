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
#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "../../engine/eval.hpp"
guiDriver::guiDriver()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"),
      fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
      guiBoard(fen), board(fen) {
  initialize_char_to_piece();
  window.draw(guiBoard); // only rerender when required
  window.display();
}
guiDriver::guiDriver(Color color)
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"),
      fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
      guiBoard(fen), board(fen) {
  initialize_char_to_piece();
  window.draw(guiBoard); // only rerender when required
  window.display();
  player_type = color;
  computer_type = color == white ? black : white;
}
guiDriver::guiDriver(std::string fen)
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"), fen(fen),
      guiBoard(fen), board(fen) {
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
                                     FixedStack<Engine::Move, 256> moves,
                                     int piece_from, int piece_to) {
  bool foundPiece = false;
  for (int i = 0; i < moves.size(); i++) {
    auto move = moves[i];
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
void guiDriver::play2() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && options == false &&
            board.gameStateHistory.peek().turn == player_type) {
          piece_from = handleSquareClick(event, moves);
          // std::cout << piece_from << std::endl;
        } else if (event.mouseButton.button == sf::Mouse::Left &&
                   options == true &&
                   board.gameStateHistory.peek().turn == player_type) {
          // the player is clicking on a MOVE_TO OR AN INVALID MOVE.
          bool validMove = makeMoveOnDisplay(event, board, moves);
          if (validMove) {
            // std::cout << "computer making move" << std::endl;
            // computer play a move
            int weval = Engine::evaluation(player_type, board);
            std::cout << "EVAL WHITE: " << weval << std::endl;
            Engine::Move move = Engine::pickRandomMove(board, computer_type);
            board.makeMove(move);
            guiBoard.updateMove(move);
            updateWindow(window, guiBoard);
            int beval = Engine::evaluation(computer_type, board);

            // std::cout << "computer made move" << std::endl;
          }

        } else if (event.mouseButton.button == sf::Mouse::Right) {
          handleRightClickUnmake(board);
        }
      }
    }
  }
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
          piece_from = handleSquareClick(event, moves);
        } else if (event.mouseButton.button == sf::Mouse::Left &&
                   options == true) {
          // the player is clicking on a MOVE_TO OR AN INVALID MOVE.
          makeMoveOnDisplay(event, board, moves);

        } else if (event.mouseButton.button == sf::Mouse::Right) {
          handleRightClickUnmake(board);
        }
      }
    }
  }
}
int guiDriver::handleSquareClick(sf::Event event,
                                 FixedStack<Engine::Move, 256> &moves) {
  guiBoard.clearPossibleMoves();
  piece_from =
      guiBoard.getPieceClicked(event.mouseButton.x, event.mouseButton.y);

  moves.clear();
  Engine::getLegalMovesForPiece(board, piece_from, moves);

  guiBoard.highlightPossibleMoves(moves);

  options = true;
  updateWindow(window, guiBoard);
  return piece_from;
}
void guiDriver::handleRightClickUnmake(Engine::Board &board) {
  if (board.history.size() == 0 or board.gameStateHistory.size() == 0) {
    throw std::out_of_range("cannot unmake a move that is not made");
  }
  Engine::Move moveToUnmake = board.history.peek();
  moveToUnmake.printMove();

  // board.display();
  board.unmakeMove(moveToUnmake);
  updateWindow(window, guiBoard);
}

bool guiDriver::makeMoveOnDisplay(sf::Event event, Engine::Board &board,
                                  FixedStack<Engine::Move, 256> &moves) {
  options = false;
  // check if player is clicking on a valid move:
  int piece =
      guiBoard.getPieceClicked(event.mouseButton.x, event.mouseButton.y);

  Piece promoted_piece = e;
  auto currentState = board.gameStateHistory.peek();
  // board.displayState(currentState);
  if (utils::getRank(piece) == 7 and currentState.turn == black and
      board.board[piece_from].piece == p) {
    handle_gui_promotion(promoted_piece, moves, piece_from, piece);
  }
  if (utils::getRank(piece) == 0 and currentState.turn == white and
      board.board[piece_from].piece == p) {
    handle_gui_promotion(promoted_piece, moves, piece_from, piece);
  }
  for (int j = 0; j < moves.size(); j++) {
    auto i = moves[j];
    if (piece_from == i._move_from && i._move_to == piece) {
      if (promoted_piece == i._toPromote) {
        piece_to = piece;
        board.makeMove(i);
        guiBoard.clearPossibleMoves();
        // i.printMove();
        guiBoard.updateMove(i);
        // Engine::Move::history.push_back(&i);
        updateWindow(window, guiBoard);
        board.display();

        auto &latestState = board.gameStateHistory.peek();
        std::cout << "before legal moves" << std::endl;

        board.displayState(latestState);
        // board.display();
        allMoves.clear();
        Engine::getLegalMoves(board, allMoves);

        if (allMoves.size() == 0) {
          std::cout << "CHECKMATE" << std::endl;
          std::cout << latestState.turn << " "
                    << "LOSES" << std::endl;
          // window.close();
        }
        return true;
      }
    }
  }
  return false;
}