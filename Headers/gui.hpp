#pragma once

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200 
#define BOX 800 / 8
#include <unordered_map> 
#include <string>
using namespace std;
enum piece_values {
  wpawn = 1,
  bpawn = 2,
  wbishop = 3,
  bbishop = 4,
  wknight = 5,
  bknight = 6,
  wrook = 7,
  brook = 8,
  wqueen = 9,
  bqueen = 10,
  wking = 11,
  bking = 12, 
  nopiece = 0
};


