#pragma once
#include "../../Headers/engine.hpp"
#include "../../engine/Board.hpp"
#include "../../engine/move.hpp"
#include "../gui/BoardDisplay.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <vector>
class guiDriver {
  public:
  guiDriver();
  // void handleRightClickUnmake();
  void updateWindow(sf::RenderWindow &window, BoardDisplay &guiBoard);
  void initialize_char_to_piece();
  void handle_gui_promotion(Piece &promoted_piece);
  void play(); // handle the main state machine of a 2 player gui driver
  void initialize_variables(); // sort of like a constructor but this is not a
                               // class yet
  std::unordered_map<char, Piece> char_to_piece;
  bool options = false;
  int piece_from = -1;
  int piece_to = -1;
  sf::RenderWindow window;
  BoardDisplay guiBoard;
  Engine::Board board;
  vector<Engine::Move> moves;
  string fen;
};
