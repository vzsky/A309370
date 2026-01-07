#include "BigInt.h"
#include <cstdint>
#include <unordered_set>
#include <utils/ModInt.h>
#include <utils/Prime.h>
#include <utils/PrimeInt.h>
#include <utils/Utils.h>
#include <vector>

template <uint32_t N, uint32_t CacheLim> struct A389544
{
  using Int = PrimeInt;
  static_assert(CacheLim > N);

private:
  virtual void _skip(uint64_t n) {}
  virtual void _not_skip(uint64_t n) {}

public:
  A389544()
  {
    for (size_t i = 2; i <= N; i++) integerMap[i] = primeFactorizer.vector_factors_freq(i);
    consecCache = {};
    sequence    = {2};
  }

  bool has_duplicate_product_cache(const uint64_t& targetProduct) const
  {
    return consecCache.contains(targetProduct);
  }

  bool duplicate_product_impossible(const Int& targetProduct, const uint64_t minimumTerms) const
  {
    const uint64_t largestPrime = targetProduct.factors().back().first;
    auto it                     = std::upper_bound(sequence.begin(), sequence.end(), largestPrime);

    if ((size_t)(it - sequence.begin()) < minimumTerms) return false;

    DenseBigInt lowerBound = 1;
    size_t startIndex      = it - sequence.begin() - minimumTerms;
    for (size_t i = startIndex; i < (size_t)(it - sequence.begin()); ++i) lowerBound *= sequence[i];

    return targetProduct.to_bigint<DenseBigInt>() < lowerBound;
  }

  bool has_duplicate_product_loop(const Int& targetProduct) const
  {
    Int candidate = 1;
    size_t l      = 0;

    for (size_t r = 0; r < sequence.size(); r++)
    {
      candidate *= integerMap[sequence[r]];

      while (l < r && !targetProduct.is_divisible_by(candidate))
      {
        candidate /= integerMap[sequence[l]];
        l++;
      }

      if (candidate == targetProduct) return true;
    }
    return false;
  }

  std::vector<uint64_t> get_sequence_until_N()
  {
    uint64_t start = sequence.back() + 1;
    for (uint64_t n = start; n <= N; n++)
    { // adding n

      if (n % 5000 == 0) std::cout << "==== Progress: " << n << " / " << N << std::endl;

      if (has_duplicate_product_cache(n)) [[unlikely]]
      {
        _skip(n);
        continue;
      }

      std::vector<Int> productsToCheck = {};

      bool found         = false;
      Int acc            = integerMap[n];
      uint64_t acc_small = n;
      for (size_t trail = sequence.size(); trail-- > 0;)
      {
        if (primeFactorizer.is_prime(sequence[trail])) break;
        acc *= integerMap[sequence[trail]];
        acc_small *= sequence[trail];
        if (acc_small != 0 && acc_small < CacheLim)
        {
          if (has_duplicate_product_cache(acc_small)) [[unlikely]]
          {
            found = true;
            break;
          }
          else
            continue;
        }
        else
          acc_small = 0;

        // if there is a consecutive product = acc, it must has > (what constructed acc) terms;
        // (what constructed acc ~ log_n(acc))
        // since all numbers are less than n
        auto minimumTerms = sequence.size() - trail + 2;
        if (duplicate_product_impossible(acc, minimumTerms)) continue;

        productsToCheck.push_back(acc);
      }

      if (found)
      {
        _skip(n);
        continue;
      }

      // TODO: how many products are we checking on avg. Is par helping ?
      if (utils::par_all_of(begin(productsToCheck), end(productsToCheck),
                            [&](const Int& target) { return !has_duplicate_product_loop(target); }))
      {
        uint64_t cand = n;
        for (size_t trail = sequence.size(); trail-- > 0;)
        {
          cand *= sequence[trail];
          if (cand >= CacheLim) break;
          consecCache.insert(cand);
        }
        _not_skip(n);
        sequence.push_back(n);
      }
      else
        _skip(n);
    }
    return sequence;
  }

  int is_interesting(uint64_t skippedElement) const
  {
    return !has_duplicate_product_loop(integerMap[skippedElement]);
  }

  void load_sequence(std::vector<uint64_t> skipped)
  {
    sequence.clear();
    size_t j = 0;
    for (uint64_t i = 1; i <= skipped.back(); i++)
    {
      if (i == skipped[j])
      {
        j++;
        continue;
      }
      sequence.push_back(i);
    }

    for (int i = 0; i <= sequence.size(); i++)
    {
      uint64_t n = sequence[i];
      int j      = i - 1;
      while (j >= 0 && n < CacheLim)
      {
        n *= sequence[j--];
        consecCache.insert(n);
      }
    }

    std::cout << "Loading sequence done" << std::endl;
  }

  // TODO: refactor this to not take skipped and check consecCache instead
  void print_interesting(std::vector<uint64_t> skipped)
  {
    size_t j = 0;
    for (uint64_t i = 1; i <= N; i++)
    {
      if (i == skipped[j])
      {
        if (is_interesting(skipped[j])) std::cout << "interesting skip " << skipped[j] << std::endl;
        j++;
        continue;
      }
      sequence.push_back(i);
    }
  }

public:
  std::vector<Int> integerMap              = std::vector<Int>(N + 1);
  std::unordered_set<uint64_t> consecCache = std::unordered_set<uint64_t>();
  std::vector<uint64_t> sequence;
  Prime<N> primeFactorizer{};
};
