#pragma once
#include <X11/Xmd.h>
enum Piece : BYTE { p, n, b, r, q, k, e };

enum Color : BYTE { white, black, none };
enum Directions {
  northwest,
  northeast,
  southwest,
  southeast,
  north,
  west,
  east,
  south,
};
constexpr char pieceReps[2][7] = {{'P', 'N', 'B', 'R', 'Q', 'K', '.'},
                                  {'p', 'n', 'b', 'r', 'q', 'k', '.'}};
constexpr bool isSlide[6] = {false, false, true, true, true, true};

constexpr int directionOffsets[6][8] = {
    // updated to mailbox method
    {0, 0, 0, 0, 0, 0, 0, 0}, // pawn
    {-21, -19, -12, -8, 8, 12, 19,
     21}, // knight (n) cannot access Directions for this
    {-11, -9, 9, 11, 0, 0, 0,
     0}, // bishop (it cannot move north south east west)
    {0, 0, 0, 0, -10, -1, 1,
     10}, // rook (went off script a bit, want to use directions as colIndex)
    {
        -11,
        -9,
        9,
        11,
        -10,
        -1,
        1,
        10,
    },                               // queen
    {-11, -10, -9, -1, 1, 9, 10, 11} // king same sa queen
};

constexpr int mailbox[120] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

                              -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

                              -1, 0,  1,  2,  3,  4,  5,  6,  7,  -1,

                              -1, 8,  9,  10, 11, 12, 13, 14, 15, -1,

                              -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,

                              -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,

                              -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,

                              -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,

                              -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,

                              -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,

                              -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

                              -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
constexpr int mailbox64[64] =
    { // translates our 8 by 8 array indeces to mailbox 120 indeces
        21, 22, 23, 24, 25, 26, 27, 28,

        31, 32, 33, 34, 35, 36, 37, 38,

        41, 42, 43, 44, 45, 46, 47, 48,

        51, 52, 53, 54, 55, 56, 57, 58,

        61, 62, 63, 64, 65, 66, 67, 68,

        71, 72, 73, 74, 75, 76, 77, 78,

        81, 82, 83, 84, 85, 86, 87, 88,

        91, 92, 93, 94, 95, 96, 97, 98};

constexpr int evaluationWeightsMaterial[6] = {10, 30, 30, 50, 90, 1000}; 