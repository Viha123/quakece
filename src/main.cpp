#include <SFML/Window/Event.hpp>
#include <iostream>
#include "../Headers/gui.hpp"
#include <SFML/Graphics.hpp>
#include "gui/BoardDisplay.hpp"
using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS GUI");
  cout << "Window created" << endl;
  BoardDisplay guiBoard;
  window.draw(guiBoard); //only rerender when required 
  window.display();

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
  }
}