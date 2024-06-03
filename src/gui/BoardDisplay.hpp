#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <cinttypes>
#include <string>
using namespace std;
class BoardDisplay : public sf::Drawable
{
  public: 
    struct Square {
      int num; //num to represent square
      int piece; //5 bit number to represent piece on square
    };
    array<array<Square, 8>, 8> boardOutWardRepresentation;
    //constructor is default add different fenString
    BoardDisplay();
    
  private:
    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    void parseFenString(string fenString); //parses fenString and fills the board representation accordingly

};