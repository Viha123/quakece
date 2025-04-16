#include "guiDriver.hpp"
#include "../../Headers/engine.hpp"
#include "../../Headers/gui.hpp"
#include "../../engine/Board.hpp"
#include "../../engine/eval.hpp"
#include "../../engine/move.hpp"
#include "../../engine/movegen/movegen.hpp"
#include "../../engine/search.hpp"
#include "../../utils.hpp"
#include "../gui/BoardDisplay.hpp"
#include "Computer.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
guiDriver::guiDriver()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"),
      fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
      guiBoard(fen), board(fen), computerai() {
  initialize_char_to_piece();
  // guiBoard.add_text("White turn", INFO_X, INFO_Y, info);
  // updateWindow(window, guiBoard);
  window.draw(guiBoard); // only rerender when required
  window.display();
}
guiDriver::guiDriver(Color color)
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"),
      fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
      guiBoard(fen), board(fen), computerai() {
  initialize_char_to_piece();
  // guiBoard.add_text("White turn", INFO_X, INFO_Y, info);
  // updateWindow(window, guiBoard);
  window.draw(guiBoard); // only rerender when required
  window.display();
  player_type = color;
  computer_type = color == white ? black : white;
  computerai.setSide(computer_type);
}
guiDriver::guiDriver(Color color, std::string fen)
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"), fen(fen),
      guiBoard(fen), board(fen), computerai() {
  initialize_char_to_piece();
  // guiBoard.add_text("CHECKMATE", INFO_X, INFO_Y, info);
  // updateWindow(window, guiBoard);
  window.draw(guiBoard); // only rerender when required
  window.display();
  player_type = color;
  computer_type = color == white ? black : white;
  computerai.setSide(computer_type);
}
guiDriver::guiDriver(std::string fen)
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI"), fen(fen),
      guiBoard(fen), board(fen), computerai() {
  initialize_char_to_piece();
  // guiBoard.add_text("CHECKMATE", INFO_X, INFO_Y, info);
  // updateWindow(window, guiBoard);
  window.draw(guiBoard); // only rerender when required
  window.display();
}
void guiDriver::updateWindow(sf::RenderWindow &window, BoardDisplay &guiBoard) {
  window.clear();
  // guiBoard.add_text("Hello World");
  window.draw(info);
  window.draw(guiBoard);
  whiteTime.add_text(std::to_string(whiteTimer.count()), WHITETIME_X,
                     WHITETIME_Y);
  blackTime.add_text(std::to_string(blackTimer.count()), BLACKTIME_X,
                     BLACKTIME_Y);
  window.draw(whiteTime);
  window.draw(blackTime);

  window.display();
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
  // whiteClock.restart();
  // blackClock.restart();
  // const auto start{std::chrono::steady_clock::now()};
  auto start = std::chrono::steady_clock::now();
  bool whiteRunning = true;
  bool blackRunning = false;
  // Color playerTurn = white;
  while (window.isOpen()) {
    sf::Event event;

    auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds{now - start};
    start = now;
    if (whiteRunning) {
      whiteTimer -= elapsed_seconds;
      updateWindow(window, guiBoard);
    } else if (blackRunning) {
      blackTimer -= elapsed_seconds;
      updateWindow(window, guiBoard);
    }
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
          checkCheckMate();
          playerMoveMade = validMove;
          // playerTurn = playerTurn == black ? white : black;

          if (player_type == white && validMove) {
            whiteRunning = false;
            blackRunning = true;
          }
          if (player_type == black && validMove) {
            blackRunning = false;
            whiteRunning = true;
          }
          // const auto end{std::chrono::steady_clock::now()};
          // const std::chrono::duration<double> elapsed_seconds{end - start};
          // whiteTimer = playerTurn == white ? whiteTimer - elapsed_seconds :
          // whiteTimer; playerTurn = playerTurn == white ? black : white;
        } else if (event.mouseButton.button == sf::Mouse::Right) {
          handleRightClickUnmake(board);
        }
      }
      if (board.gameStateHistory.peek().turn == computer_type &&
          playerMoveMade) {
        playerMoveMade = false;
        try {
          Engine::Move computerMove = computerai.makeMove(board);
          auto now = std::chrono::steady_clock::now();
          const std::chrono::duration<double> elapsed_seconds{now - start};
          start = now;
          guiBoard.updateMove(computerMove);
          updateWindow(window, guiBoard);
          checkCheckMate();
          // playerTurn = playerTurn == black ? white : black;
          if (computer_type == white) {
            whiteTimer -= elapsed_seconds;
            updateWindow(window, guiBoard);
            whiteRunning = false;
            blackRunning = true;
          }
          if (computer_type == black) {
            blackTimer -= elapsed_seconds;
            updateWindow(window, guiBoard);
            blackRunning = false;
            whiteRunning = true;
          }
        } catch (const std::out_of_range &exc) {
          // std::cout << exc.what();
          std::cout << exc.what() << std::endl;
          info.add_text("CHECKMATE", INFO_X, INFO_Y);
          updateWindow(window, guiBoard);
          // window.close();
        } catch (const std::length_error &exc) {
          // std::cout << exc.what();
          std::cout << "\nclosing window" << std::endl;
          // window.close();
          info.add_text("STALEMATE", INFO_X, INFO_Y);
          updateWindow(window, guiBoard);
        }
      }
    }
  }
}
void guiDriver::play() {
  auto start = std::chrono::steady_clock::now();
  bool whiteRunning = true;
  bool blackRunning = false;
  while (window.isOpen()) {
    sf::Event event;
    auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds{now - start};
    start = now;
    if (whiteRunning) {
      whiteTimer -= elapsed_seconds;
      updateWindow(window, guiBoard);
    } else if (blackRunning) {
      blackTimer -= elapsed_seconds;
      updateWindow(window, guiBoard);
    }

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
          bool valid = makeMoveOnDisplay(event, board, moves);
          if (valid and board.gameStateHistory.peek().turn == black) {
            whiteRunning = false;
            blackRunning = true;
          }
          if (valid and board.gameStateHistory.peek().turn == white) {
            whiteRunning = true;
            blackRunning = false;
          }

          Engine::evaluation(board.gameStateHistory.peek().turn, board);

        } else if (event.mouseButton.button == sf::Mouse::Right) {
          handleRightClickUnmake(board);
        }
      }
    }
  }
}
void guiDriver::checkCheckMate() {
  auto &latestState = board.gameStateHistory.peek();

  // board.displayState(latestState);

  allMoves.clear();
  Engine::getLegalMoves(board, allMoves);
  std::string turn = latestState.turn == white ? "WHITE" : "BLACK";
  std::string loss = "LOSES";
  if (allMoves.size() == 0 && Engine::kingInCheck(board, latestState.turn)) {
    std::cout << "CHECKMATE " << std::endl;
    std::cout << +latestState.turn << " "
              << " LOSES" << std::endl;
    std::string checkmate = "CHECKMATE";

    std::string fullString = checkmate + turn + loss;
    info.add_text(fullString, INFO_X, INFO_Y);
    updateWindow(window, guiBoard);
    // std::cout << "\nclosing window" << std::endl;
    // window.close();
  } else if (allMoves.size() == 0) {
    std::cout << "STALEMATE " << std::endl;
    std::string checkmate = "STALEMATE";
    std::string fullString = checkmate + turn + loss;
    info.add_text(fullString, INFO_X, INFO_Y);
    updateWindow(window, guiBoard);
  }
}
int guiDriver::handleSquareClick(sf::Event event,
                                 FixedStack<Engine::Move, 256> &moves) {
  guiBoard.clearPossibleMoves();
  piece_from =
      guiBoard.getPieceClicked(event.mouseButton.x, event.mouseButton.y);

  moves.clear();
  Engine::getLegalMovesForPiece(board, piece_from, moves);
  FixedStack<Engine::Move, 256> test;
  Engine::getLegalMoves(board, test);
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
  moveToUnmake.printInChess();

  board.unmakeMove(moveToUnmake);
  updateWindow(window, guiBoard);
  board.display();
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
        // board.display();

        return true;
      }
    }
  }
  return false;
}