#pragma once

#include <iostream>
#include <string>
#include <vector>

class Integer
{
public:
  Integer();
  Integer(int);
  Integer(const std::string&);

  Integer abs() const;
  Integer operator-() const;
  const Integer& operator+=(const Integer&);
  const Integer& operator-=(const Integer&);
  const Integer& operator*=(const Integer&);

  friend std::ostream& operator<<(std::ostream&, const Integer&);

  std::strong_ordering operator<=>(const Integer& other) const;
  bool operator==(const Integer& other) const
  {
    return (*this <=> other) == std::strong_ordering::equal;
  }

private:
  void remove_leading_zeros();
  void abs_add(const Integer&);
  void abs_sub(const Integer&);

public:
  bool mIsNegative{false};
  std::vector<char> mDigits; // least significant bit first
};
