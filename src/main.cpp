#include "../Headers/gui.hpp"
#include "gui/BoardDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include "../engine/Board.hpp"
#include <vector>
#include "../engine/move.hpp"
#include "../engine/movegen/movegen.hpp"
using namespace std;
vector<Engine::Move*> Engine::Move::history = {};
int main() {
  string fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
  Engine::Board board(fen);
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "CHESS GUI");
  cout << "Window created" << endl;
  BoardDisplay guiBoard(fen);
  // BoardDisplay guiBoard;
  window.draw(guiBoard); // only rerender when required
  window.display();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          int piece = guiBoard.getPieceClicked(event.mouseButton.x, event.mouseButton.y);
          window.clear();
          window.draw(guiBoard);
          window.display();
          Engine::getMoveForPiece(board, piece);

        }
      }
    }
  }
}