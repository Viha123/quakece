// things i want here
// square num to rank file
// rank file to square num
#include "Headers/gui.hpp"
namespace utils {
  int getNumFromRF(int rank, int file) {
    return rank * 8 + file;
  }
  int getRank(int num) {
    return num/8;
  }
  int getFile(int num) {
    return num % 8;
  }
  int getYPos(int row) {
    return row * BOX;
  }
  int getXPos(int col) {
    return col * BOX;
  }
}