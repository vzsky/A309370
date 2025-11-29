#include <cassert>
#include <map>
#include <vector>

template <int N> class PrimeFactorizer {
public:
  PrimeFactorizer() : mLowestPrime(N) { sieve(); }

private:
  void sieve();

public:
  std::vector<int> factors(int n) const;
  std::map<int, int> factors_freq(int n) const;

private:
  std::vector<int> mLowestPrime{N};
};

template <int N> void PrimeFactorizer<N>::sieve() {
  for (int i = 0; i <= N; ++i)
    mLowestPrime[i] = i;
  for (int i = 2; i * i <= N; ++i) {
    if (mLowestPrime[i] == i) {
      for (int j = i * i; j <= N; j += i) {
        if (mLowestPrime[j] == j)
          mLowestPrime[j] = i;
      }
    }
  }
}

template <int N> std::vector<int> PrimeFactorizer<N>::factors(int n) const {
  assert(2 <= n && n <= N);
  std::vector<int> f;
  while (n > 1) {
    f.push_back(mLowestPrime[n]);
    n /= mLowestPrime[n];
  }
  return f;
}

template <int N>
std::map<int, int> PrimeFactorizer<N>::factors_freq(int n) const {
  std::map<int, int> freq;
  for (int p : factors(n))
    freq[p]++;
  return freq;
}
