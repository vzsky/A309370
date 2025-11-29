#include "Integer.h"
#include <algorithm>
#include <cctype>
#include <compare>

/**************************************
 * CONSTRUCTIONS
 *************************************/

Integer::Integer()
: mIsNegative(false), mDigits{0}
{}

Integer::Integer(int x) {
  mIsNegative = x < 0;
  if (mIsNegative) x = -x;

  if (x == 0) {
    mDigits = {0};
    return;
  } 
  while (x > 0) {
    mDigits.push_back(char(x % 10));
    x /= 10;
  }
}

Integer::Integer(const std::string& s) {
  mDigits.clear();
  mIsNegative = s[0] == '-';

  size_t i = mIsNegative ? 1 : 0;
  for (; i < s.size(); i++) {
    if (std::isdigit(s[i])) mDigits.push_back(char(s[i] - '0'));
  }

  if (mDigits.empty()) mDigits = {0};
  std::reverse(mDigits.begin(), mDigits.end());
  remove_leading_zeros();
}

/**************************************
 * OPERATIONS
 *************************************/

void Integer::remove_leading_zeros() {
  while (mDigits.size() > 1 && mDigits.back() == 0)
    mDigits.pop_back();

  if (mDigits.size() == 1 && mDigits[0] == 0)
    mIsNegative = false;
}

void Integer::abs_add(const Integer& src) {
  int carry = 0;
  size_t n = std::max(mDigits.size(), src.mDigits.size());

  mDigits.resize(n, 0);

  for (size_t i = 0; i < n; ++i) {
    int d = mDigits[i] + (i < src.mDigits.size() ? src.mDigits[i] : 0) + carry;
    mDigits[i] = d % 10;
    carry = d / 10;
  }
  if (carry)
    mDigits.push_back(carry);
}

void Integer::abs_sub(const Integer& src) {
  int borrow = 0;
  for (size_t i = 0; i < mDigits.size(); ++i) {
    int d = mDigits[i] - borrow - (i < src.mDigits.size() ? src.mDigits[i] : 0);
    if (d < 0) {
      d += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }
    mDigits[i] = d;
  }
  remove_leading_zeros();
}

std::ostream& operator << (std::ostream& os, const Integer& x) {
  if (x.mDigits.empty()) return os << 0;
  if (x.mIsNegative) os << '-';

  for (auto it = x.mDigits.rbegin(); it != x.mDigits.rend(); ++it)
    os << char('0' + *it);

  return os;
}


std::strong_ordering Integer::operator<=>(const Integer& other) const {
  // Compare signs first
  if (mIsNegative != other.mIsNegative)
    return mIsNegative <=> other.mIsNegative;

  if (mDigits.size() != other.mDigits.size()) {
    if (mIsNegative)
      return other.mDigits.size() <=> mDigits.size();
    else
      return mDigits.size() <=> other.mDigits.size();
  }

  // Same size: compare digits from most significant to least
  for (size_t i = mDigits.size(); i > 0; --i) {
    if (mDigits[i-1] != other.mDigits[i-1]) {
      if (mIsNegative > 0)
        return other.mDigits[i-1] <=> mDigits[i-1];
      else
        return mDigits[i-1] <=> other.mDigits[i-1];
    }
  }

  return std::strong_ordering::equal;
}

