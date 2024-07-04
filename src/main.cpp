#include "drivers/guiDriver.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "tests/perft.hpp"
#include <iostream>
#include "../utils.hpp"
#include <cassert>

using namespace std;

int main(int argc, char *argv[]) { // 2, 1, c -> completely manual, manual 1
                                   // player or computer vs computer
  std::string fen = "r3k3/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1r/PPPBBP1P/R3K2R w KQkq - 0 1";
  //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
  // r3k1r1/Rppp1ppp/1b3nbN/nPB5/B1P1P3/q4N2/Pp1P2PP/R2Q1RK1 w q - 0 1 //after h8g8
  if(*argv[1] == '2') {
    guiDriver gui(fen);
    gui.play();
  }
  else if(*argv[1] == '1') {
    //gui play against computer
  }
  else if(*argv[1] == 'p') {

    // Tests::testCases();
    //do perft testing
    Engine::Board perftBoard(fen);// initial start position
    int depth = *argv[2] - '0';
    int nodes = Tests::perft(perftBoard, depth);
    std::cout << YELLOW << nodes <<  RESET << std::endl;
    Tests::divide(depth, fen);
    // std::string str = utils::convertToChessNotation(19);
    // cout << str << endl;
  }
  else  {
    cout << "Mode not accepted" << endl;
  }
}