#include <cmath>
#include <gtest/gtest.h>
#include <stdexcept>
#include <utils/LogInt.h>

class LogIntAccessor : public LogInt
{
public:
  using LogInt::LogInt;
  LogIntAccessor(const LogInt& other) : LogInt(other) {}
  double getLower() const { return static_cast<double>(mLower); }
  double getUpper() const { return static_cast<double>(mUpper); }
};

inline void expect_logint_exact(const LogIntAccessor& li, uint64_t exp)
{
  EXPECT_EQ(li.getLower(), exp);
  EXPECT_EQ(li.getUpper(), exp);
}

inline void expect_logint_approx(const LogIntAccessor& li, uint64_t n)
{
  double val = std::log2(static_cast<double>(n));
  EXPECT_LE(li.getLower(), val);
  EXPECT_GE(li.getUpper(), val);
}

inline void expect_logint_compat(const LogIntAccessor& li, const LogIntAccessor& target)
{
  EXPECT_LE(li.getLower(), target.getLower());
  EXPECT_GE(li.getUpper(), target.getUpper());
}

TEST(LogIntTest, Construction)
{
  uint64_t tests[] = {1, 2, 24, 30, 192, 1024, 1025, 1500, 2500, 83392, 1ll << 60};
  for (auto& t : tests)
  {
    expect_logint_approx(LogInt{t}, t);
    expect_logint_approx(LogInt{PrimeInt{t}}, t);
  }

  expect_logint_exact(LogInt{1}, 0);
  expect_logint_exact(LogInt{2}, 1);
  expect_logint_exact(LogInt{1024}, 10);
  expect_logint_exact(LogInt{1ll << 62}, 62);

  for (int exp = 500; exp <= 1000; exp++) expect_logint_exact(PrimeInt({{2, exp}}), exp);

  ASSERT_THROW((void)LogInt(0), std::invalid_argument);
}

TEST(LogIntTest, MultiplicationPowersOf2)
{
  uint64_t powers[] = {1, 2, 4, 8, 16, 32, 1024, 4096, 65536};
  for (uint64_t i = 0; i <= 15; i++)
  {
    for (uint64_t j = 0; j <= 15; j++)
    {
      LogInt prod1 = LogInt(1 << i) * LogInt(1 << j);
      LogInt prod2 = LogInt(1 << (i + j));
      expect_logint_exact(prod1, i + j);
      expect_logint_exact(prod2, i + j);
    }
  }
}

TEST(LogIntTest, MultiplicationAllIntegersCompatible)
{
  uint64_t test_values[] = {3, 7, 8, 15, 16, 1024, 1025, 1500, 23453, 65535};

  for (uint64_t a : test_values)
  {
    for (uint64_t b : test_values)
    {
      LogInt prod = LogInt(a) * LogInt(b);

      expect_logint_approx(prod, a * b);
      expect_logint_compat(prod, LogInt{a * b});
    }
  }
}
