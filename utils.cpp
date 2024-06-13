// things i want here
// square num to rank file
// rank file to square num
#include "utils.hpp"
#include "Headers/gui.hpp"
#include <iostream>
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

  void printVectorGivenIndex(int index) {
  // std::cout << index << std::endl;
    std::cout << getRank(index) << " " << getFile(index) << std::endl;
  } 
  int getNumFromStr(std::string str) {
    int col = str[0] - 'a';
    int row = 8 - (str[1] - '0');
    // std::cout << "HERE" << std::endl;
    return getNumFromRF(row, col);
  }

}