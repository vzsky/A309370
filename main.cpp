#include <omp.h>
#include <cassert>
#include "Elem.h"
#include "utils.h"
#include "treap.h"

auto score_to_order = [](auto score) {
  return [score](const auto& a, const auto& b) -> bool {
    auto sa = score(a), sb = score(b);
    if (sa != sb) return sa > sb;
    return a > b;
  };
};


template<int N>
bool check(const Elems<N>& cands) {
  for (size_t i = 0; i < cands.size(); i++) {
    for (size_t j = i+1; j < cands.size(); j++) {
      for (size_t k = 0; k < cands.size(); k++) {
        if (k == i || k == j) continue;
        const Elem<N>& c = cands[k];

        uint64_t result = 0;
        bool valid = true;
        for (int ind = 0; ind < N; ind++) {
          int bit = (cands[i][ind] + cands[j][ind] - cands[k][ind]);
          if (bit != 0 && bit != 1) valid=false;
          result |= bit << ind;
        }
        if (!valid) continue;
        if (cands.contains(Elem<N> {result})) return false;
      }
    }
  }
  return true;
}

// faster way of check(sidon_set + {next}) assuming check(sidon_set) is true
// and sidon_set is ordered increasingly (only weakly order of bit containment needed)
// and next is greater than all of sidon_set in that bit containment ordering
template<int N>
bool can_add (const Elems<N>& sidon_set, const Elem<N>& next) {
  if (sidon_set.size() < 3) return true;
  // check if a + b - c for all c<a<b is next or not

  for (int i = 0; i < sidon_set.size(); i++) {
    auto R1 = sidon_set[i].mUnd ^ next.mUnd;
    auto R2 = sidon_set[i].mUnd & next.mUnd;
    for (int j = i+1; j < sidon_set.size(); j++) {
      for (int k = j+1; k < sidon_set.size(); k++) {

        auto L1 = sidon_set[j].mUnd ^ sidon_set[k].mUnd;
        auto L2 = sidon_set[j].mUnd & sidon_set[k].mUnd;

        if (L1 == R1 && L2 == R2) return false;

      }
    }
  }
  return true;
}

template<int N, typename Heuristic, typename PruneFunc>
Elems<N> find_using_frontier(
  Heuristic h, 
  PruneFunc prune_alg, 
  bool show_smaller_output = false,
  bool show_progress = false
) {
  using Frontier = Treap <Elems<N>, Heuristic>;

  Frontier frontier (h);
  frontier.insert({}); 

  int smaller_output_cnt = 1;

  // looping from 0 up ensures bit containment ordering.
  // and ensure we get all the smaller N solutions
  for (uint64_t i = 0; i < (1<<N); i++) {
    if (show_progress) print_progress(i, 1<<N);
    if (show_smaller_output && i == (1 << smaller_output_cnt) - 1) {
      std::cout << "#============================" << std::endl;
      std::cout << "N = " << smaller_output_cnt << std::endl;
      auto result = frontier.max([](const auto &x){ return x.size(); }, Elems<N>());
      result.print(smaller_output_cnt);
      std::cout << std::flush;
      smaller_output_cnt++;
    }

    Elem<N> next {i};

    std::vector<Frontier> new_frontiers;
    #pragma omp parallel
    {
      Frontier local_front (h); // thread-local
      #pragma omp for nowait
      for (size_t idx = 0; idx < frontier.size(); idx++) {
        const Elems<N>& parent = frontier[idx];       // no copy
        if (can_add(parent, next)) {
          Elems<N> child = parent;
          child.push_back(next);
          child.mCreated = i;
          local_front.insert(std::move(child));
        }
      }
      // merge thread-local vectors into global
      #pragma omp critical
      new_frontiers.push_back(std::move(local_front));
    }

    for (auto& s : new_frontiers) frontier.merge(std::move(s));
    prune_alg(frontier);
  }
  if (show_progress) std::cout << "\rDone!            " << std::endl;
  return frontier.max([](const auto &x){ return x.size(); }, Elems<N>());
}

// ####################################
// # Main Search Driver
// ####################################

int main () {
  timeit([](){

    constexpr int N = 15;

    uint64_t clock = 0; 
  
    // prune is called at every tick
    const int SIZE_KEEP  = 10000;
    const int SIZE_LIMIT = 100000;
    auto prune = [ &clock, SIZE_KEEP, SIZE_LIMIT ] (auto& frontier) {

      // clock++;

      if (frontier.size () < SIZE_LIMIT) return;
      frontier.restrict(SIZE_KEEP);
    };

    // note: heuristic shouldn't change over time! Treap ordering needs to be the same!
    // that means we shouldn't be able to use mLastUsed or other fields we which to 
    // mutate in this calculation. this includes clock.
    // const int64_t SIZE_MULT = 200;
    // const int64_t DATE_CREATED_MULT = 1;
    auto heuristic = [] (const auto& v) -> int64_t {

      // return v.size() * SIZE_MULT + v.mCreated * DATE_CREATED_MULT;
      return v.size();
    };

    auto result = find_using_frontier<N>(
      score_to_order(heuristic),
      prune, 
      true, 
      false
    ); 

    assert(check(result));

  });
  return 0;
}

