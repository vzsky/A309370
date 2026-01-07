#include "lib.h"

int main()
{
  utils::timeit(
      []
      {
        A389544<500000, 2'000'000'000> e;
        const std::vector<uint64_t>& result = e.get_sequence_until_N();
        size_t index                        = 0;
        for (auto x : result)
        {
          std::cout << ++index << ' ' << x << std::endl;
        }
        std::cout << "endSize: " << e.consecCache.size() << std::endl;
      });
}
