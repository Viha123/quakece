// draws 64 alternating squares and puts the pieces on it.
// by default it will use the starting fen string
// otherwise it will use the provided fen string.

#include "BoardDisplay.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <iostream>
#include "../../Headers/gui.hpp"
using namespace std;
BoardDisplay::BoardDisplay() {
  //nothing
}
void BoardDisplay::parseFenString(string fenString) {}
void BoardDisplay::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      int xPos = BOX * file;
      int yPos = BOX * rank;
      cout << "xpos : " << xPos << endl;
      cout << "ypos : " << yPos << endl;
      sf::RectangleShape rect(sf::Vector2f(BOX, BOX));
      rect.setPosition(xPos, yPos);
      // Square s;
      // s.num = rank * 8 + file;
      // s.piece = 0; //change this later
      // boardOutWardRepresentation[rank][file] = s;
      if ((rank + file) % 2 == 0) {
        // square should be white
        rect.setFillColor(sf::Color(sf::Color::White));
      }
      else{
        rect.setFillColor(sf::Color::Black);
      }
      target.draw(rect, states);
    }
  }
}
