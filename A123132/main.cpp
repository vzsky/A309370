#include <bits/stdc++.h>
#include <utils/Integer.h>
#include <utils/PrimeFactorizer.h>
#include <utils/utils.h>
using namespace std;

template<int N>
class A123132 {
public:
  Integer describe_prime_factors(int x) {
    std::map freq = factorizer.factors_freq(x);
    std::string out;
    for (auto &[p, cnt] : freq) {
      out += std::to_string(cnt);
      out += std::to_string(p);
    }
    return out;
  }

  static int digit_of(int x) {
    if (x == 0) return 1;
    return static_cast<int>(std::log10(std::abs(x))) + 1;
  }

  const PrimeFactorizer<N> & getFactorizer () const {
    return factorizer;
  }

private:
  PrimeFactorizer<N> factorizer;
};

int main() {
  timeit([](){
    
    constexpr int N = 1e7;
    A123132<N> a;
    auto factorizer = a.getFactorizer();

    int cnt = 0;
    for (int n = 2; n <= N; ++n) {
      if (n % 2 == 0) continue; // focusing on ==, it's impossible
      if (n % 5 == 0) continue; // focusing on ==, it's impossible
      if (factorizer.factors_freq(n).size() == 1) continue; // same

      if (n >= a.describe_prime_factors(n)) {
        cnt++;
        cout << n << " -> ";

        for (auto& [p, c] : factorizer.factors_freq(n)) {
          cout << p << ':' << c << ' ';
        }
        cout << endl;

      }
    }
    cout << "total: " << cnt << endl;

  });

}
