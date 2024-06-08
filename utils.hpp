// things i want here
// square num to rank file
// rank file to square num
#pragma once
#include <string>
namespace utils {
  int getNumFromRF(int rank, int file);
  int getRank(int num);
  int getFile(int num);
  int getYPos(int row);
  int getXPos(int col);
  void printVectorGivenIndex(int num);
  int getNumFromStr(std::string str);
}