#include "../Headers/gui.hpp"
#include "gui/BoardDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include "../engine/Board.hpp"
using namespace std;

int main() {

  Engine::Board board("4k2r/6r1/8/8/8/8/3R4/R3K3 w Qk - 0 1");
  board.display();
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                          "CHESS GUI");
  cout << "Window created" << endl;
  BoardDisplay guiBoard("4k2r/6r1/8/8/8/8/3R4/R3K3 w Qk - 0 1");
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
          piece_values piece = guiBoard.getPieceClicked(event.mouseButton.x, event.mouseButton.y);
          window.clear();
          window.draw(guiBoard);
          window.display();
        }
      }
    }
  }
}