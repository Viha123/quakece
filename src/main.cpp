#include "../Headers/gui.hpp"
#include "../engine/Board.hpp"
#include "../engine/move.hpp"
#include "../engine/movegen/movegen.hpp"
#include "../utils.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "gui/BoardDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
using namespace std;
void updateWindow(sf::RenderWindow &window, BoardDisplay &guiBoard);
void initialize_char_to_piece();
// vector<Engine::Move *> Engine::Move::history = {};

std::unordered_map<char, Piece> char_to_piece;
void handle_gui_promotion(Piece &promoted_piece);
int main(int argc, char *argv[]) { // 2, 1, c -> completely manual, manual 1
                                   // player or computer vs computer

  string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Engine::Board board(fen);
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "CHESS GUI");
  // cout << argv[1] << endl;
  BoardDisplay guiBoard(fen);
  initialize_char_to_piece();
  // BoardDisplay guiBoard;
  window.draw(guiBoard); // only rerender when required
  window.display();
  vector<Engine::Move> moves;
  bool options = false;
  int piece_from = -1;
  int piece_to = -1;

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (*argv[1] ==
          '2') { // state machine for when 2 people will be playing chess
        if (event.type == sf::Event::MouseButtonPressed) {
          if (event.mouseButton.button == sf::Mouse::Left && options == false) {
            guiBoard.clearPossibleMoves();
            piece_from = guiBoard.getPieceClicked(event.mouseButton.x,
                                                  event.mouseButton.y);

            moves = Engine::getLegalMovesForPiece(board, piece_from);
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
            Engine::Board::State *currentState = board.gameStateHistory.back();
            // board.displayState(currentState);
            if (utils::getRank(piece) == 7 and currentState->turn == black and
                board.board[piece_from].piece == p) {
              handle_gui_promotion(promoted_piece);
            }
            if (utils::getRank(piece) == 0 and currentState->turn == white and
                board.board[piece_from].piece == p) {
              handle_gui_promotion(promoted_piece);
            }
            for (auto i : moves) {
              if (piece_from == i._move_from && i._move_to == piece) {
                if (promoted_piece == i._toPromote) {
                  piece_to = piece;
                  // board.display();
                  board.makeMove(i);
                  guiBoard.clearPossibleMoves();
                  // i.printMove();
                  guiBoard.updateMove(i);
                  // Engine::Move::history.push_back(&i);
                  updateWindow(window, guiBoard);

                  // board.display();
                  board.toggleTurn();
                
                  Engine::Board::State *latestState =
                      board.gameStateHistory.back();
                  board.displayState(latestState);
                }
              }
            }

          } else if (event.mouseButton.button == sf::Mouse::Right) {
            if (board.history.size() == 0 or
                board.gameStateHistory.size() == 0) {
              throw std::out_of_range("cannot unmake a move that is not made");
            }
            Engine::Move moveToUnmake = *board.history.back();
            
            board.unmakeMove(moveToUnmake);
            updateWindow(window, guiBoard);
            board.display();
            
          }
        }

      } else if (*argv[1] == '1') {
        if (event.type == sf::Event::MouseButtonPressed) {
          if (event.mouseButton.button == sf::Mouse::Left) {
            guiBoard.clearPossibleMoves();
            int piece = guiBoard.getPieceClicked(event.mouseButton.x,
                                                 event.mouseButton.y);

            moves = Engine::getMoveForPiece(board, piece);

            guiBoard.highlightPossibleMoves(moves);

            updateWindow(window, guiBoard);
          }
        }
      }
    }
  }
}
void updateWindow(sf::RenderWindow &window, BoardDisplay &guiBoard) {
  window.clear();
  window.draw(guiBoard);
  window.display();
}
void initialize_char_to_piece() {
  char_to_piece['q'] = q;
  char_to_piece['Q'] = q;
  char_to_piece['n'] = n;
  char_to_piece['N'] = n;
  char_to_piece['b'] = b;
  char_to_piece['B'] = b;
  char_to_piece['R'] = r;
  char_to_piece['r'] = r;
}

void handle_gui_promotion(Piece &promoted_piece) {
  char promotion;

  std::cout << "Q for queen, N for knight, B for bishop, R for "
               "rook promotion"
            << std::endl;
  std::cin >> promotion;
  promoted_piece = char_to_piece[promotion];
}