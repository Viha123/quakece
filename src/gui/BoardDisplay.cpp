// draws 64 alternating squares and puts the pieces on it.
// by default it will use the starting fen string
// otherwise it will use the provided fen string.

#include "BoardDisplay.hpp"
#include "../../Headers/gui.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <string>
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
  array<string, 8> boards{};
  while (token.length() != 0) {
    r += 1;
    if (r > 8) {
      break;
    }
    int end = token.find("/");
    string trow = token.substr(0, end);
    // cout << trow << endl;
    token = token.substr(end + 1);
    // cout << r << endl;
    int col = -1;
    for (char c : trow) {
      col += 1;
      switch (c) {
      case 'B': {
        Square whiteBishop{0, wbishop};
        boardOutWardRepresentation[r - 1][col] = whiteBishop;
        break;
      }
      case 'b': {
        Square blackBishop{0, bbishop};
        boardOutWardRepresentation[r - 1][col] = blackBishop;
        break;
      }
      case 'P': {
        Square whitePawn{0, wpawn};
        boardOutWardRepresentation[r - 1][col] = whitePawn;
        break;
      }
      case 'p': {
        Square blackPawn{0, bpawn};
        boardOutWardRepresentation[r - 1][col] = blackPawn;
        break;
      }
      case 'N': {
        Square whiteKnight{0, wknight};
        boardOutWardRepresentation[r - 1][col] = whiteKnight;
        break;
      }
      case 'n': {
        Square blackKnight{0, bknight};
        boardOutWardRepresentation[r - 1][col] = blackKnight;
        break;
      }
      case 'R': {
        Square whiteRook{0, wrook};
        boardOutWardRepresentation[r - 1][col] = whiteRook;
        break;
      }
      case 'r': {
        Square blackRook{0, brook};
        boardOutWardRepresentation[r - 1][col] = blackRook;
        break;
      }
      case 'Q': {
        Square whiteQueen{0, wqueen};
        boardOutWardRepresentation[r - 1][col] = whiteQueen;
        break;
      }
      case 'q': {
        Square blackQueen{0, bqueen};
        boardOutWardRepresentation[r - 1][col] = blackQueen;
        break;
      }
      case 'K': {
        Square whiteKing{0, wking};
        boardOutWardRepresentation[r - 1][col] = whiteKing;
        break;
      }
      case 'k': {
        Square blackKing{0, bking};
        boardOutWardRepresentation[r - 1][col] = blackKing;
        break;
      }
      default: { // is an integer and need to skip squares
        int intv = c - 'a';
        col -= 1;
        col += intv % 8;
        Square empty{0, nopiece};
        boardOutWardRepresentation[r - 1][col] = empty;
        break;
      }
      }
    }
  }
}
void BoardDisplay::draw(sf::RenderTarget &target,
                        sf::RenderStates states) const {
  sf::Color black(0xB7C0D8ff);
  sf::Color white(0xE8EDF9ff);

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      int xPos = BOX * file;
      int yPos = BOX * rank;
      // cout << "xpos : " << xPos << endl;
      // cout << "ypos : " << yPos << endl;
      sf::RectangleShape rect(sf::Vector2f(BOX, BOX));
      rect.setPosition(xPos, yPos);
      if ((rank + file) % 2 == 0) {
        // square should be white
        rect.setFillColor(white);
      } else {
        rect.setFillColor(black);
      }
      target.draw(rect, states);

      if (boardOutWardRepresentation[rank][file].piece == bbishop) {
        drawSprite(target, "assets/bbishop.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == wbishop) {
        drawSprite(target, "assets/wbishop.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == bpawn) {
        drawSprite(target, "assets/bpawn.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == wpawn) {
        drawSprite(target, "assets/wpawn.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == bknight) {
        drawSprite(target, "assets/bknight.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == wknight) {
        drawSprite(target, "assets/wknight.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == brook) {
        drawSprite(target, "assets/brook.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == wrook) {
        drawSprite(target, "assets/wrook.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == bqueen) {
        drawSprite(target, "assets/bqueen.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == wqueen) {
        drawSprite(target, "assets/wqueen.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == bking) {
        drawSprite(target, "assets/bking.png", xPos, yPos);
      }
      if (boardOutWardRepresentation[rank][file].piece == wking) {
        drawSprite(target, "assets/wking.png", xPos, yPos);
      }
    }
  }
}
void BoardDisplay::drawSprite(sf::RenderTarget &target, string path, int xPos, int yPos) const {
  sf::Texture texture;
  if (!texture.loadFromFile(path)) {
    // Error...
  }
  sf::Sprite sprite;
  sprite.setTexture(texture);
  sprite.setPosition(xPos, yPos);
  target.draw(sprite);
}
