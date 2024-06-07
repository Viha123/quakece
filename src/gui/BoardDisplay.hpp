#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <string>
#include "../../Headers/gui.hpp"

using namespace std;
class BoardDisplay : public sf::Drawable
{
  public: 
    struct Square {
      int num = 0; //num to represent square
      piece_values piece = nopiece; //5 bit number to represent piece on square
      unique_ptr<sf::Sprite> sprite = nullptr;
      bool clicked = false;
      bool possibleMove = false;
    };
    
    //constructor is default add different fenString
    BoardDisplay();
    BoardDisplay(string fen);

    int getPieceClicked(int mouseX, int mouseY);
    void highlightPossibleMoves(vector<int> nums);
    void clearPossibleMoves();
    // array<sf::Sprite, 13> sprites; //vectors to store all sprites after they are displayed
    
  private:
    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    void parseFenString(string fenString); //parses fenString and fills the board representation accordingly
    void generateSprite(string path, int xPos, int yPos, piece_values value);
    vector<unique_ptr<sf::Texture>> textures; // this stores the pointers to the textures 
    array<array<Square, 8>, 8> boardOutWardRepresentation;
    int prevRowClicked = -1;
    int prevColClicked = -1; 
};