#include "drivers/guiDriver.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) { // 2, 1, c -> completely manual, manual 1
                                   // player or computer vs computer
  if(*argv[1] == '2') {
    guiDriver gui;
    gui.play();
  }
  else if(*argv[1] == '1') {
    //gui play against computer
  }
  else if(*argv[1] == 'p') {
    //do perft testing
    

  }
  else  {
    cout << "Mode not accepted" << endl;
  }
}