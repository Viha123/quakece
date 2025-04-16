#include "TranspositionTable.hpp"

namespace Engine {
std::optional<HashEntry> TranspositionTable::get(long key) {
  auto index = key % tt_size;
  if (tt[index] && tt[index].get()->zobrist == key) { // check if the key is not a collision
    return *tt[index];
  }
  return std::nullopt; // Return the HashEntry
}

void TranspositionTable::store(long key, int depth, Flag flag, int eval, int ancient, Move move) {
  // store new if new entry has greater depth than old entry
  HashEntry new_entry(key, depth, flag, eval, ancient, move);
  std::optional<HashEntry> current_entry = get(key);
  // only store if there is nothing in that spot or if the new_entries depth is
  // greater
  if (!current_entry.has_value() || current_entry->depth <= new_entry.depth) {
    tt[key % tt_size] = std::make_unique<HashEntry>(new_entry);
  }
}
int TranspositionTable::lookupEvaluation(long key, int depth, int alpha, int beta) {
  auto entry = get(key);
  if (entry) {
    std::cout << "found something" << std::endl;
    if (entry->depth >= depth) { // we are able to use this
      int score = entry->eval;
      if (entry->flag == Flag::exact) {
        return score;
      } else if (entry->flag == Flag::alpha && score <= alpha) {
        return score;
      } else if (entry->flag == Flag::beta && score >= beta) {
        return score;
      }
    }
  }
  return -1;
}
void TranspositionTable::clear() {
  std::for_each(std::execution::par, tt.begin(), tt.end(),
                [](std::unique_ptr<HashEntry> &entry) { entry.reset(); });
}
}; // namespace Engine
