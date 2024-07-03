#include "drivers/guiDriver.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "tests/perft.hpp"
#include <iostream>
#include <cassert>

using namespace std;

int main(int argc, char *argv[]) { // 2, 1, c -> completely manual, manual 1
                                   // player or computer vs computer
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  if(*argv[1] == '2') {
    guiDriver gui;
    gui.play();
  }
  else if(*argv[1] == '1') {
    //gui play against computer
  }
  else if(*argv[1] == 'p') {
    //do perft testing
    // Engine::Board perftBoard(fen);// initial start position
    // int nodes = Tests::perft(perftBoard, 3);
    // std::cout << YELLOW << nodes <<  RESET << std::endl;
    Tests::testCases();
  }
  else  {
    cout << "Mode not accepted" << endl;
  }
}