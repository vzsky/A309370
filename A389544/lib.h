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
  static_assert(CacheLim < std::numeric_limits<uint32_t>::max());

private:
  virtual void _skip(uint64_t n) {}
  virtual void _not_skip(uint64_t n) {}

  void skip(uint64_t n) { _skip(n); }

  void not_skip(uint64_t n)
  {
    _not_skip(n);
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

public:
  A389544()
  {
    integerMap.resize(N);
    for (size_t i = 2; i <= N; i++) integerMap[i] = primeFactorizer.vector_factors_freq(i);
    consecCache = {};
    sequence.resize(N);
    sequence.push_back(2);
  }

  bool has_duplicate_product_cache(const uint64_t& targetProduct) const
  {
    // assert(targetProduct < CacheLim)
    return consecCache.contains(targetProduct);
  }

  inline bool optimization_1(const Int& targetProduct) const
  {
    DenseBigInt target = targetProduct.to_bigint<DenseBigInt>();
    DenseBigInt candidate;
    const auto& primes  = primeFactorizer.all_primes();
    const auto& factors = targetProduct.factors();

    for (auto it = factors.rbegin(); it != factors.rend(); ++it)
    {
      const auto& [p, m] = *it;
      if (p == 2 || m <= 1) continue;
      if (m >= p) continue;

      // if multplicity of p is m, we at least need p, .., m * p
      auto endIt   = std::upper_bound(sequence.begin(), sequence.end(), m * p);
      auto startIt = std::lower_bound(sequence.begin(), sequence.end(), p);
      candidate    = 1;
      for (auto it = startIt; it != endIt; ++it)
      {
        candidate *= *it;
        if (candidate > target)
        {
          stats.opt1++;
          return true;
        }
      }
    }
    return false;
  }

  inline bool optimization_2(const Int& targetProduct) const
  {
    DenseBigInt target      = targetProduct.to_bigint<DenseBigInt>();
    const auto& factors     = targetProduct.factors();
    const auto largestPrime = factors.back().first;
    const size_t largestPrimeIndex =
        std::upper_bound(sequence.begin(), sequence.end(), largestPrime) - sequence.begin();

    if (factors.size() == 1) return false;

    size_t endForward  = largestPrime;
    size_t endBackward = largestPrime;
    for (int k = 2; k <= factors.size(); k++)
    {
      const auto nextPrime = factors[factors.size() - k].first;
      endForward           = std::max(endForward, largestPrime + (nextPrime - (largestPrime % nextPrime)));
      endBackward          = std::min(endBackward, largestPrime - (largestPrime % nextPrime));
    }

    DenseBigInt candidate;
    bool failed_forward = false, failed_backward = false;
    candidate = 1;
    for (int i = largestPrimeIndex; i < sequence.size(); i++)
    {
      candidate *= sequence[i];
      if (candidate > target)
      {
        failed_forward = true;
        break;
      }
      if (sequence[i] == endForward) break;
    }

    candidate = 1;
    for (int i = largestPrimeIndex; i >= 0; i--)
    {
      candidate *= sequence[i];
      if (candidate > target)
      {
        failed_backward = true;
        break;
      }
      if (sequence[i] == endBackward) break;
    }

    if (failed_forward && failed_backward)
    {
      stats.opt2++;
      return true;
    }
    return false;
  }

  inline bool optimization_3(const Int& targetProduct, const uint64_t multipliedTerms) const
  {
    // if there is a consecutive product = acc, it must has > (what constructed acc) terms;
    // (what constructed acc ~ log_n(acc))
    // since all numbers are less than n
    uint64_t minimumTerms = multipliedTerms + 1;
    DenseBigInt target    = targetProduct.to_bigint<DenseBigInt>();
    DenseBigInt candidate;
    const auto& primes  = primeFactorizer.all_primes();
    const auto& factors = targetProduct.factors();

    const auto largestPrime = factors.back().first;
    const size_t largestPrimeIndex =
        std::upper_bound(sequence.begin(), sequence.end(), largestPrime) - sequence.begin();

    if (largestPrimeIndex < minimumTerms) return false;

    candidate = 1;
    for (size_t i = largestPrimeIndex - minimumTerms + 1; i <= largestPrimeIndex; ++i)
      candidate *= sequence[i];

    if (target < candidate)
    {
      stats.opt3++;
      return true;
    }
    return false;
  }

  bool duplicate_product_impossible(const Int& targetProduct, const uint64_t multipliedTerms) const
  {
    if (optimization_1(targetProduct)) return true;
    if (optimization_2(targetProduct)) return true;
    if (optimization_3(targetProduct, multipliedTerms)) return true;
    return false;
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

  void try_add_number(uint64_t n)
  {
    std::vector<Int> productsToCheck = {};

    if (primeFactorizer.is_prime(n)) return not_skip(n);

    stats.cached++;
    if (has_duplicate_product_cache(n)) [[unlikely]]
      return skip(n);

    productsToCheck.clear();
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
        stats.cached++;
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

      const auto multipliedTerms = sequence.size() - trail + 1;
      // parallelizing this seems to make it worse
      if (duplicate_product_impossible(acc, multipliedTerms)) continue;
      productsToCheck.push_back(acc);
    }

    if (found) return skip(n);

    stats.loop += productsToCheck.size();
    if (utils::par_all_of(begin(productsToCheck), end(productsToCheck),
                          [&](const Int& target) { return !has_duplicate_product_loop(target); }))
      return not_skip(n);

    return skip(n);
  }

  const std::vector<uint64_t>& get_sequence_until_N()
  {
    uint64_t start = sequence.back() + 1;
    for (uint64_t n = start; n <= N; n++)
    {
      if (n % 100000 == 0)
      {
        std::cout << "==== Progress: " << n << " / " << N << std::endl;
        stats.print();
      }
      try_add_number(n);
    }
    return sequence;
  }

  int is_interesting(uint64_t skippedElement) const
  {
    return !has_duplicate_product_loop(integerMap[skippedElement]);
  }

public:
  std::vector<Int> integerMap              = std::vector<Int>(N + 1);
  std::unordered_set<uint64_t> consecCache = std::unordered_set<uint64_t>();
  std::vector<uint64_t> sequence;
  Prime<N> primeFactorizer{};

  mutable struct Stats
  {
    uint64_t loop   = 0;
    uint64_t cached = 0;
    uint64_t opt1   = 0;
    uint64_t opt2   = 0;
    uint64_t opt3   = 0;

    void print() const
    {
      std::cout << "Cached " << cached << std::endl;
      std::cout << "Loop " << loop << std::endl;
      std::cout << "Opt1 " << opt1 << std::endl;
      std::cout << "Opt2 " << opt2 << std::endl;
      std::cout << "Opt3 " << opt3 << std::endl;
    }
  } stats;
};
