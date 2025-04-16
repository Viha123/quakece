#pragma once
#include "../Headers/engine.hpp"
#include "move.hpp"
#include <algorithm>
#include <array>
#include <execution>
#include <memory>
#include <optional>
namespace Engine {
class HashEntry {
public:
  HashEntry(long zobrist, int depth, Flag flag, int eval, int ancient,
            Move move)
      : zobrist(zobrist), depth(depth), eval(eval), ancient(ancient),
        flag(flag), move(move) {};
  long zobrist;
  int depth;
  int eval;
  int ancient;
  Flag flag;
  Move move;
};

class TranspositionTable {
public:
  TranspositionTable() = default;

  std::optional<HashEntry> get(long key); // return entry at
  int lookupEvaluation(long key, int depth, int alpha, int beta);
  void store(long key, int depth, Flag flag, int eval, int ancient, Move move);
  // void set_entry(long key, int depth, )
  void clear(); // clear table between games
  static constexpr int tt_size = 1024;
  std::array<std::unique_ptr<HashEntry>, tt_size>
      tt; // will reserve space for it
  int lookupfailed = -1;
};
} // namespace Engine
