#include <chrono>
#include <iostream>

namespace utils
{

inline void print_progress(long long i, long long total)
{
  if (i % (total / 100 + 1) == 0 || i == total - 1)
  {
    int percent = static_cast<int>(100.0 * i / total);
    std::cout << "\rProgress: " << percent << "% " << std::flush;
  }
}

void log(auto x)
{
  static size_t startTime = 0;
  using namespace std::chrono;
  auto now = high_resolution_clock::now();
  size_t ms = duration_cast<milliseconds>(now.time_since_epoch()).count();
  if (startTime == 0)
    startTime = ms;

  std::cout << ms - startTime << ": " << x << std::endl;
  startTime = ms;
}

} // namespace utils

inline auto size_comparer = [](auto const& a, auto const& b) { return a.size() < b.size(); };

template <typename Func> auto timeit(Func f)
{
  using namespace std::chrono;
  auto start = high_resolution_clock::now();

  f();

  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start).count();
  std::cout << "Time elapsed: " << duration / 1000 << "." << duration % 1000 << "s" << std::endl;
}
