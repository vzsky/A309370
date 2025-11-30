#include <chrono>
#include <iostream>

inline void print_progress(int i, int total)
{
  if (i % (total / 100 + 1) == 0 || i == total - 1)
  {
    int percent = static_cast<int>(100.0 * i / total);
    std::cout << "\rProgress: " << percent << "% " << std::flush;
  }
}

inline auto size_comparer = [](auto const& a, auto const& b) { return a.size() < b.size(); };

template <typename Func> void timeit(Func f)
{
  using namespace std::chrono;
  auto start = high_resolution_clock::now();

  f();

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start).count();
  std::cout << "Time elapsed: " << duration / 1000 << "." << duration % 1000 << "s" << std::endl;
}
