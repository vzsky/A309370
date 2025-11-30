#include "../Integer.h"
#include <gtest/gtest.h>

TEST(IntegerTest, ConstructFromInt)
{
  Integer a(0);
  EXPECT_EQ(a, Integer("0"));

  Integer b(12345);
  EXPECT_EQ(b, Integer("12345"));

  Integer c(-987);
  EXPECT_EQ(c, Integer("-987"));
}

TEST(IntegerTest, ConstructFromString)
{
  Integer a("0");
  EXPECT_EQ(a, Integer(0));

  Integer b("00123");
  EXPECT_EQ(b, Integer(123));

  Integer c("-00456");
  EXPECT_EQ(c, Integer(-456));

  Integer d("+789");
  EXPECT_EQ(d, Integer(789));
}

TEST(IntegerTest, Comparison)
{
  Integer a("123");
  Integer b(123);
  Integer c("456");
  Integer d("-123");

  EXPECT_TRUE(a == b);
  EXPECT_TRUE(a < c);
  EXPECT_TRUE(c > a);
  EXPECT_TRUE(d < a);
  EXPECT_TRUE(d < c);
}

TEST(IntegerTest, StreamOutput)
{
  Integer a("00123");
  Integer b("-0456");

  std::stringstream ss;
  ss << a << " " << b;
  EXPECT_EQ(ss.str(), "123 -456");
}

TEST(IntegerArithmeticTest, AdditionAssignment)
{
  Integer a("123");

  a += Integer(456);
  EXPECT_EQ(a, 579);

  Integer b("-100");
  b += 50;
  EXPECT_EQ(b, Integer(-50));

  Integer c("0");
  c += Integer(0);
  EXPECT_EQ(c, Integer(0));
}

TEST(IntegerArithmeticTest, SubtractionAssignment)
{
  Integer a("123");

  a -= Integer(456);
  EXPECT_EQ(a, -333);

  Integer b("-100");
  b -= 50;
  EXPECT_EQ(b, Integer(-150));

  Integer c("0");
  c -= Integer(0);
  EXPECT_EQ(c, Integer(0));
}

TEST(IntegerArithmeticTest, MultiplicationAssignment)
{
  Integer a("123");

  a *= Integer(456);
  EXPECT_EQ(a, 56088);

  Integer b("-100");
  b *= -50;
  EXPECT_EQ(b, 5000);

  Integer c("-1001");
  c *= 1234;
  EXPECT_EQ(c, -1235234);

  Integer d("5");
  d *= Integer(0);
  EXPECT_EQ(d, 0);

  Integer e("1000000000");
  e *= e;
  e += 1;
  e *= 123456;
  EXPECT_EQ(e, std::string("123456000000000000123456"));
}
