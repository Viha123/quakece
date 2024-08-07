#pragma once
#include "../../Headers/gui.hpp"
#include "../../engine/move.hpp"
#include "../FixedStack.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;
class BoardDisplay : public sf::Drawable {
public:
  struct Square {
    int num = 0;                  // num to represent square
    piece_values piece = nopiece; // 5 bit number to represent piece on square
    unique_ptr<sf::Sprite> sprite = nullptr;
    bool clicked = false;
    bool possibleMove = false;
  };

  // constructor is default add different fenString
  BoardDisplay();
  BoardDisplay(string fen);
  sf::Font font;
  sf::Text text;
  void add_text(std::string str, int xPos, int yPos, sf::Text t);
  int getPieceClicked(int mouseX, int mouseY);
  void highlightPossibleMoves(FixedStack<Engine::Move, 256> moves);
  void clearPossibleMoves();
  // array<sf::Sprite, 13> sprites; //vectors to store all sprites after they
  // are displayed
  void updateMove(Engine::Move move);

private:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  void parseFenString(string fenString); // parses fenString and fills the board
                                         // representation accordingly
  void generateSprite(string path, int xPos, int yPos);
  vector<unique_ptr<sf::Texture>>
      textures; // this stores the pointers to the textures
  array<array<Square, 8>, 8> boardOutWardRepresentation;
  void make_move(int move_from_r, int move_from_c, int move_to_r,
                 int move_to_c);
  void clear_square(int move_from_r, int move_from_c);
  int prevRowClicked = -1;
  int prevColClicked = -1;
  void handlePromotion(Piece piece, Color color, int row, int col);
};