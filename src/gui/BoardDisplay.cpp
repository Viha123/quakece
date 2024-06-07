// draws 64 alternating squares and puts the pieces on it.
// by default it will use the starting fen string
// otherwise it will use the provided fen string.

#include "BoardDisplay.hpp"
#include "../../Headers/gui.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include "../../utils.hpp"
using namespace std;
BoardDisplay::BoardDisplay() {
  // nothing
  string defaultFendString =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  parseFenString(defaultFendString);
}
BoardDisplay::BoardDisplay(string fen) {
  // nothing
  parseFenString(fen);
}
void BoardDisplay::parseFenString(string fenString) {
  string token = fenString.substr(0, fenString.find(" ")); // part 1 of string
  int r = 0;
  while (token.length() != 0) {
    r += 1;
    if (r > 8) {
      break;
    }
    int end = token.find("/");
    string trow = token.substr(0, end);
    token = token.substr(end + 1);
    // cout << r << endl;
    int col = 0;
    for (char c : trow) {
      int yPos = utils::getYPos(r-1);
      int xPos = utils::getXPos(c);
      boardOutWardRepresentation[r - 1][col].num = utils::getNumFromRF(r-1, col);
      switch (c) {
      case 'B': {

        boardOutWardRepresentation[r - 1][col].piece = wbishop;
        generateSprite("assets/wbishop.png", r - 1, col, wbishop);

        break;
      }
      case 'b': {
        boardOutWardRepresentation[r - 1][col].piece = bbishop;
        generateSprite("assets/bbishop.png", r - 1, col, bbishop);
        break;
      }
      case 'P': {
        boardOutWardRepresentation[r - 1][col].piece = wpawn;
        generateSprite("assets/wpawn.png", r - 1, col, wpawn);

        break;
      }
      case 'p': {
        boardOutWardRepresentation[r - 1][col].piece = bpawn;
        generateSprite("assets/bpawn.png", r - 1, col, bpawn);
        break;
      }
      case 'N': {
        boardOutWardRepresentation[r - 1][col].piece = wknight;
        generateSprite("assets/wknight.png", r - 1, col, wknight);
        break;
      }
      case 'n': {
        boardOutWardRepresentation[r - 1][col].piece = bknight;
        generateSprite("assets/bknight.png", r - 1, col, bknight);

        break;
      }
      case 'R': {
        boardOutWardRepresentation[r - 1][col].piece = wrook;
        generateSprite("assets/wrook.png", r - 1, col, wrook);
        break;
      }
      case 'r': {
        boardOutWardRepresentation[r - 1][col].piece = brook;
        generateSprite("assets/brook.png", r - 1, col, brook);
        break;
      }
      case 'Q': {
        boardOutWardRepresentation[r - 1][col].piece = wqueen;
        generateSprite("assets/wqueen.png", r - 1, col, wqueen);

        break;
      }
      case 'q': {
        boardOutWardRepresentation[r - 1][col].piece = bqueen;
        generateSprite("assets/bqueen.png", r - 1, col, bqueen);

        break;
      }
      case 'K': {
        boardOutWardRepresentation[r - 1][col].piece = wking;
        generateSprite("assets/wking.png", r - 1, col, wking);

        break;
      }
      case 'k': {
        boardOutWardRepresentation[r - 1][col].piece = bking;
        generateSprite("assets/bking.png", r - 1, col, bking);

        break;
      }
      default: { // is an integer and need to skip squares
        int intv = c - '0';
        for (int tc = 0; tc < intv; tc += 1) {
          boardOutWardRepresentation[r - 1][col + tc].num =
              (r - 1) * 8 + (col + tc);
          boardOutWardRepresentation[r - 1][col + tc].piece = nopiece;
        }
        col += intv - 1;
        break;
      }
      }
      col += 1;
    }
  }
}
void BoardDisplay::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {
  sf::Color black(0xB7C0D8ff);
  sf::Color white(0xE8EDF9ff);
  sf::Color clicked(0xFAEBD7dd);
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      int xPos = utils::getXPos(file);
      int yPos = utils::getYPos(rank);
      // cout << "xpos : " << xPos << endl;
      // cout << "ypos : " << yPos << endl;
      // cout << rank << " " << file << " " <<
      // boardOutWardRepresentation[rank][file].piece << endl;
      sf::RectangleShape rect(sf::Vector2f(BOX, BOX));
      rect.setPosition(xPos, yPos);
      if ((rank + file) % 2 == 0) {
        // square should be white
        rect.setFillColor(white);
      } else {
        rect.setFillColor(black);
      }
      if (boardOutWardRepresentation[rank][file].clicked == true) {
        rect.setFillColor(clicked);
      }

      target.draw(rect, states);
      if (boardOutWardRepresentation[rank][file].sprite != nullptr) {
        target.draw(*boardOutWardRepresentation[rank][file].sprite, states);
      }
    }
  }
}

int BoardDisplay::getPieceClicked(int mouseX, int mouseY) {
  int row = mouseY / 100;
  int col = mouseX / 100;
  // cout << row << " " << col << endl;
  cout << "prev clicked " << prevRowClicked << prevColClicked << endl;

  boardOutWardRepresentation[row][col].clicked = true;
  if (prevColClicked == col and prevRowClicked == row) {
    bool toggle = !boardOutWardRepresentation[prevRowClicked][prevColClicked].clicked;
    cout << boardOutWardRepresentation[prevRowClicked][prevColClicked].clicked << endl;
    boardOutWardRepresentation[prevRowClicked][prevColClicked].clicked = toggle;
  }
  else if (prevColClicked != -1) {
    boardOutWardRepresentation[prevRowClicked][prevColClicked].clicked = false;
  }
  
  prevRowClicked = row;
  prevColClicked = col;
  // cout << "curr clicked " << row << col << endl;
  cout << boardOutWardRepresentation[row][col].num << endl;
  return boardOutWardRepresentation[row][col].num; // place holder
}
//@TODO: Rewrite this method to simply create the sprite then return a unique
// pointer to the sprite. In thsi function we can also append the pointer
// reference to vector/array make this not const let the displayboard method
// call target.draw(dereference pointer to sprite)
void BoardDisplay::generateSprite(string path, int row, int col,
                                  piece_values value) {
  auto texture = make_unique<sf::Texture>();
  if (!texture->loadFromFile(path)) {
    // Error...
  }
  auto sprite = make_unique<sf::Sprite>();
  sprite->setTexture(*texture);
  sprite->setPosition(utils::getXPos(col), utils::getYPos(row));
  // cout << sprite->getGlobalBounds().width << endl;
  textures.push_back(std::move(texture));
  boardOutWardRepresentation[row][col].sprite = std::move(sprite);
}
