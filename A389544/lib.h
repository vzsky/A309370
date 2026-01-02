#include <utils/ModInt.h>
#include <utils/Prime.h>
#include <utils/PrimeInt.h>
#include <utils/Utils.h>
#include <vector>

template <int N> struct A389544
{

  using Int = PrimeInt;

  std::vector<Int> integerMap = std::vector<Int>(N + 1);
  std::vector<int> sequence = {2};
  Prime<N> primeFactorizer{};

  A389544()
  {
    for (int i = 2; i <= N; i++)
      integerMap[i] = primeFactorizer.vector_factors_freq(i);
  }

  void add_to_seqeunce(int v)
  {
    sequence.push_back(v);
  }

  bool has_duplicate_product(const Int& targetProduct)
  {
    Int candidate = 1;
    size_t l = 0;

    for (size_t r = 0; r < sequence.size(); r++)
    {
      candidate *= integerMap[sequence[r]];

      while (l < r && !targetProduct.is_divisible_by(candidate))
      {
        candidate /= integerMap[sequence[l]];
        l++;
      }

      if (candidate == targetProduct)
        return true;
    }
    return false;
  }

  std::vector<int> get_sequence_until_N()
  {
    for (uint64_t n = 3; n <= N; n++)
    { // adding n
      bool shouldSkip = false;
      Int trailingProduct = integerMap[n];

      if (has_duplicate_product(trailingProduct))
        shouldSkip = true;

      for (size_t trail = sequence.size(); !shouldSkip && trail-- > 0;)
      {
        if (primeFactorizer.is_prime(sequence[trail]))
          break; // we can just add this number!

        trailingProduct *= integerMap[sequence[trail]];

        if (has_duplicate_product(trailingProduct))
          shouldSkip = true;
      }

      if (!shouldSkip)
        add_to_seqeunce(n);
    }

    return sequence;
  }
};
