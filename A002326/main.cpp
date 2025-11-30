#include <utils/Integer.h>
#include <utils/MoreMath.h>
#include <utils/PrimeFactorizer.h>
#include <utils/Utils.h>

template <int N> class A002326
{
private:
  // assume total = (p1^a1 ... pn^an)
  // returns minimum k such that n = (p1^a1 ... p^k ... pn^an) still make 2^n =
  // 1 p^k should divides total and 2^total = 1
  int min_p_needed(int total, int p, int k, int mod)
  {
    int l = 0, r = k;
    while (l < r)
    {
      int mid = (l + r) / 2;
      int now = total / pow(p, k - mid);
      if (pow(2, now, mod) == 1)
        r = mid;
      else
        l = mid + 1;
    }
    return l;
  }

  int order_of_2_mod_p(int prime)
  {
    int n = prime;
    int phi_n = prime - 1;
    auto primes = factorizer.factors_freq(phi_n);
    int ans = 1;
    for (auto& [p, k] : primes)
    {
      int x = min_p_needed(phi_n, p, k, n);
      ans *= pow(p, x);
    }
    return ans;
  }

  int order_of_2_mod_prime_power(int p, int power)
  {
    if (power == 1)
      return order_of_2_mod_p(p);
    int cur = order_of_2_mod_p(p);
    int mod = pow(p, power);
    int l = 0;
    int r = power - 1;
    while (l < r)
    {
      int mid = (l + r) / 2;
      if (pow(2, cur * pow(p, mid), mod) == 1)
        r = mid;
      else
        l = mid + 1;
    }
    return cur * pow(p, l);
  }

public:
  int get_answer(int n)
  {
    if (n == 1)
      return 1;
    n -= 1;
    auto factors = factorizer.factors_freq(2 * n + 1);
    int ans = 1;
    for (auto& [p, k] : factors)
    {
      auto res = order_of_2_mod_prime_power(p, k);
      ans = lcm(ans, res);
    }
    return ans;
  }

  std::vector<int> get_answers_until(int until)
  {
    std::vector<int> all_answers(until);
    all_answers[1] = 1;
    for (int ind = 2; ind < until; ind++)
    {
      int n = 2 * ind - 1;
      int ans = 1;
      auto factors = factorizer.factors_freq(n);
      if (factors.size() == 1)
      {
        auto p = factors.begin()->first;
        auto k = factors.begin()->second;
        if (k == 1)
          ans = order_of_2_mod_p(p);
        else
        {
          int prev_ind = pow(p, k - 1) / 2 - 1;
          ans = all_answers[prev_ind];
          if (pow(2, all_answers[prev_ind], n) != 1)
            ans *= p;
        }
      }
      else
      {
        for (auto& [p, k] : factors)
        {
          ans = lcm(ans, all_answers[pow(p, k) / 2 - 1]);
        }
      }
      all_answers[ind - 2] = ans;
    }
    return all_answers;
  }

private:
  PrimeFactorizer<2 * N + 1> factorizer;
};

int main()
{

  timeit(
      []()
      {
        constexpr int N = 1e5;
        A002326<N> a;
        for (int i = 1; i < N; i++)
          a.get_answer(i);
      });

  timeit(
      []()
      {
        constexpr int N = 1e5;
        A002326<N> a;
        auto res = a.get_answers_until(N);
      });
}
