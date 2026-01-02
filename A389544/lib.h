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
  std::vector<Int> quickProduct = {2};
  Prime<N> primeFactorizer{};

  A389544()
  {
    for (int i = 2; i <= N; i++)
      integerMap[i] = primeFactorizer.vector_factors_freq(i);
  }

  void add_to_seqeunce(int v)
  {
    sequence.push_back(v);
    quickProduct.push_back(quickProduct.back() * integerMap[v]);
  }

  Int product_of_range(int l, int r) const
  {
    assert(r >= l);
    if (l == 0)
      return quickProduct[r];
    return quickProduct[r] / quickProduct[l - 1];
  }

  bool has_duplicate_product(const Int& targetProduct) const
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

  size_t first_divisible_from(size_t l, const Int& target) const
  {
    // first r such that target divides prod(l..r)
    size_t mnr = l, mxr = sequence.size() - 1;
    while (mnr < mxr)
    {
      size_t mid = (mnr + mxr) >> 1;
      Int p = product_of_range(l, mid);
      if (p.is_divisible_by(target))
        mxr = mid;
      else
        mnr = mid + 1;
    }
    return mnr;
  }

  size_t last_divisible_until(size_t r, const Int& target) const
  {
    // last l such that target divides prod(l..r)
    size_t mnl = 0, mxl = r;
    while (mnl < mxl)
    {
      size_t mid = (mnl + mxl + 1) >> 1;
      Int p = product_of_range(mid, r);
      if (p.is_divisible_by(target))
        mnl = mid;
      else
        mxl = mid - 1;
    }
    return mnl;
  }

  bool has_duplicate_product_bsearch(const Int& targetProduct) const
  {
    size_t l = 0, r = 0;
    while (r < sequence.size() - 1)
    {
      r = first_divisible_from(l, targetProduct);
      if (product_of_range(l, r) == targetProduct)
        return true;
      l = last_divisible_until(r, targetProduct);
      if (product_of_range(l, r) == targetProduct)
        return true;
      l++;
    }
    return false;
  }

  std::vector<int> get_sequence_until_N()
  {
    utils::log("started");

    for (uint64_t n = 3; n <= N; n++)
    { // adding n

      if (n % 500 == 0)
      {
        utils::log(n);
      }

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
