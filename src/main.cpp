#include "../Headers/gui.hpp"
#include "../engine/Board.hpp"
#include "../engine/move.hpp"
#include "../engine/movegen/movegen.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "gui/BoardDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <vector>
using namespace std;
void updateWindow(sf::RenderWindow &window, BoardDisplay &guiBoard);
vector<Engine::Move *> Engine::Move::history = {};
int main(int argc, char *argv[]) { // 2, 1, c -> completely manual, manual 1
                                   // player or computer vs computer

  string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Engine::Board board(fen);
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "CHESS GUI");
  cout << argv[1] << endl;
  BoardDisplay guiBoard(fen);
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

            moves = Engine::getMoveForPiece(board, piece_from);
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
            for (auto i : moves) {
              if (piece_from == i._move_from && i._move_to == piece) {
                piece_to = piece;
                board.display();
                cout << piece_from << " " << piece_to << endl;
                board.makeMove(i);
                guiBoard.clearPossibleMoves();
                guiBoard.updateMove(i);
                updateWindow(window, guiBoard);
                board.display();
              }
            }
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