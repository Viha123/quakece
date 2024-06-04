#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include "../../Headers/gui.hpp"

using namespace std;
class BoardDisplay : public sf::Drawable
{
  public: 
    struct Square {
      int num = 0; //num to represent square
      piece_values piece = nopiece; //5 bit number to represent piece on square
      
    };
    array<array<Square, 8>, 8> boardOutWardRepresentation;
    //constructor is default add different fenString
    BoardDisplay();
    BoardDisplay(string fen);
    
  private:
    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    void parseFenString(string fenString); //parses fenString and fills the board representation accordingly
    void drawSprite(sf::RenderTarget& target, string path, int xPos, int yPos) const;
};