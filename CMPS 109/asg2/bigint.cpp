// $Id: bigint.cpp,v 1.61 2014-06-26 17:06:06-07 - - $
// Isaiah Solomon
// icsolomo
// 1344680
// Wesley Mackey
// CMPS 109
// 2/2/2015


#include <cstdlib>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"

bigint::bigint (long that): big_value (that)
{
   if (that < 0)
      negative = true;
   else
      negative = false;
}

bigint::bigint (const string& that)
{
   negative = false;
   for (auto itor = that.crbegin(); itor != that.crend(); ++itor)
   {
      if (*itor == '_')
         negative = true;
      else
         big_value.push_back(*itor);
   }
   while (big_value.back() == '0')
      big_value.pop_back();
}

bigint::bigvalue_t bigint::do_bigadd (const bigvalue_t& left, const bigvalue_t& right)
{
   int carry = 0;
   bigvalue_t result {};
   auto itor_left = left.begin();
   auto itor_right = right.begin();
   while (itor_left != left.end() or itor_right != right.end())
   {
      if (itor_left != left.end())
         carry += static_cast<int>(*itor_left) - 48;
      if (itor_right != right.end())
         carry += static_cast<int>(*itor_right) - 48;
      if (carry > 9)
      {
         carry -= 10;
         result.push_back(static_cast<digit_t>(carry + 48));
         carry = 1;
      }
      else
      {
         result.push_back(static_cast<digit_t>(carry + 48));
         carry = 0;
      }
      if (itor_left != left.end())
         ++itor_left;
      if (itor_right != right.end())
         ++itor_right;
   }
   if (carry == 1)
      result.push_back('1');
   while (result.back() == '0')
      result.pop_back();
   return result;
}

bigint::bigvalue_t bigint::do_bigsub (const bigvalue_t& left, const bigvalue_t& right)
{
   int carry = 0;
   bigvalue_t result {};
   auto itor_left = left.begin();
   auto itor_right = right.begin();
   while (itor_left != left.end() or itor_right != right.end())
   {
      if (itor_left != left.end())
         carry += static_cast<int>(*itor_left) - 48;
      if (itor_right != right.end())
         carry -= static_cast<int>(*itor_right) - 48;
      if (carry < 0)
      {
         carry += 10;
         result.push_back(static_cast<digit_t>(carry + 48));
         carry = -1;
      }
      else
      {
         result.push_back(static_cast<digit_t>(carry + 48));
         carry = 0;
      }
      if (itor_left != left.end())
         ++itor_left;
      if (itor_right != right.end())
         ++itor_right;
   }
   while (result.back() == '0')
      result.pop_back();
   return result;
}

bigint::bigvalue_t bigint::do_bigmul (const bigvalue_t& left, const bigvalue_t& right)
{
   bigvalue_t result {}, temp {}, zero {};
   if (left == zero or right == zero)
      return zero;
   int carry = 0, place = 0;
   for (auto itor_left = left.cbegin(); itor_left < left.cend(); ++itor_left, ++place)
   {
      temp = zero;
      carry = 0;
      for (int i = 0; i < place; ++i)
         temp.push_back('0');
      for (auto itor_right = right.cbegin(); itor_right < right.cend(); ++itor_right)
      {
         carry = (static_cast<int>(*itor_left) - 48) * (static_cast<int>(*itor_right) - 48) + carry;
         temp.push_back(static_cast<digit_t>((carry % 10) + 48));
         carry /= 10;
      }
      if (carry > 0)
         temp.push_back(static_cast<digit_t>((carry % 10) + 48));
      result = do_bigadd(result, temp);
   }
   while (result.back() == '0')
      result.pop_back();
   return result;
}

bool bigint::do_bigless (const bigvalue_t& left, const bigvalue_t& right)
{
   if (left.size() < right.size())
      return true;
   else if (left.size() > right.size())
      return false;
   else
   {
      auto itor_left = left.crbegin();
      auto itor_right = right.crbegin();
      while (itor_left != left.crend() and itor_right != right.crend())
      {
         if (*itor_left < *itor_right)
            return true;
         else if (*itor_left > *itor_right)
            return false;
         ++itor_left;
         ++itor_right;
      }
   }
   return false;
}


bigint operator+ (const bigint& left, const bigint& right) {
   bigint result {};
   if (left.negative == right.negative)
   {
      result.big_value = result.do_bigadd(left.big_value, right.big_value);
      result.negative = left.negative;
   }
   else
   {
      if (result.do_bigless(left.big_value, right.big_value))
      {
         result.big_value = result.do_bigsub(right.big_value, left.big_value);
         result.negative = not left.negative;
      }
      else
      {
         result.big_value = result.do_bigsub(left.big_value, right.big_value);
         result.negative = left.negative;
      }
   }
   if (result.big_value.empty())
      result.negative = false;
   return result;
}

bigint operator- (const bigint& left, const bigint& right) {
   bigint result {};
   if (left.negative == right.negative)
   {
      if (result.do_bigless(left.big_value, right.big_value))
      {
         result.big_value = result.do_bigsub(right.big_value, left.big_value);
         result.negative = not left.negative;
      }
      else
      {
         result.big_value = result.do_bigsub(left.big_value, right.big_value);
         result.negative = left.negative;
      }
   }
   else
   {
      result.big_value = result.do_bigadd(left.big_value, right.big_value);
      result.negative = left.negative;
   }
   if (result.big_value.empty())
      result.negative = false;
   return result;
}

bigint operator+ (const bigint& right) {
   bigint right_ = right;
   right_.negative = false;
   return right_;
}

bigint operator- (const bigint& right) {
   bigint right_ = right;
   right_.negative = true;
   return right_;
}

long bigint::to_long() const {
   bigint long_ceiling {"2147483647"};
   if (*this > long_ceiling)
      throw range_error ("bigint__to_long: out of range");
   long long_value = 0, place = 1;
   for (auto itor = big_value.begin(); itor != big_value.end(); ++itor)
   {
      long_value += (*itor - '0') * place;
      place *= 10;
   }
   return long_value;
}

bool abs_less (const vector<unsigned char>& left, const vector<unsigned char>& right)
{
   if (left.size() < right.size())
      return true;
   else if (left.size() > right.size())
      return false;
   else
   {
      auto itor_left = left.crbegin();
      auto itor_right = right.crbegin();
      while (itor_left != left.crend() and itor_right != right.crend())
      {
         if (*itor_left < *itor_right)
            return true;
         else if (*itor_left > *itor_right)
            return false;
         ++itor_left;
         ++itor_right;
      }
   }
   return false;
}

//
// Multiplication algorithm.
//

bigint operator* (const bigint& left, const bigint& right)
{
   bigint result {};
   result.negative = false;
   if (left.negative != right.negative)
      result.negative = true;
   result.big_value = result.do_bigmul(left.big_value, right.big_value);
   if (result.big_value.empty())
      result.negative = false;
   return result;
}

//
// Division algorithm.
//

void multiply_by_2 (bigint::bigvalue_t& value)
{
   bigint temp {};
   value = temp.do_bigadd(value, value);
}

void divide_by_2 (bigint::bigvalue_t& value)
{
   bigint::bigvalue_t value_ {};
   int remainder = 0;
   for (auto itor = value.rbegin(); itor != value.rend(); ++itor)
   {
      remainder += static_cast<int>(*itor) - 48;
      if (remainder < 2)
         remainder *= 10;
      else
      {
         value_.push_back(static_cast<unsigned char>((remainder / 2) + 48));
         remainder = (remainder % 2) * 10;
      }
   }
   value.resize(value_.size(), '0');
   int i = 0;
   for (auto itor = value_.rbegin(); itor != value_.rend(); ++itor)
   {
      value.at(i) = *itor;
      ++i;
   }
}


bigint::quot_rem divide (const bigint::bigvalue_t& left, const bigint::bigvalue_t& right)
{
   bigint temp {};
   bigint::bigvalue_t zero {};
   if (right == zero)
   {
      cout << "ydc: divide by zero" << endl;
      return {zero, zero};
   }
   bigint::bigvalue_t divisor {right};
   bigint::bigvalue_t remainder {left};
   bigint::bigvalue_t quotient {'0'};
   bigint::bigvalue_t power_of_2 {'1'};
   while (abs_less (divisor, remainder))
   {
      multiply_by_2 (divisor);
      multiply_by_2 (power_of_2);
   }
   while (abs_less (zero, power_of_2))
   {
      if (not abs_less (remainder, divisor))
      {
         remainder = temp.do_bigsub(remainder, divisor);
         quotient = temp.do_bigadd(quotient, power_of_2);
      }
      divide_by_2 (divisor);
      divide_by_2 (power_of_2);
   }
   return {quotient, remainder};
}

bigint operator/ (const bigint& left, const bigint& right)
{
   bigint result {};
   if (left.negative == right.negative)
      result.negative = false;
   else
      result.negative = true;
   result.big_value = divide(left.big_value, right.big_value).first;
   if (result.big_value.empty())
      result.negative = false;
   return result;
}

bigint operator% (const bigint& left, const bigint& right)
{
   bigint result {};
   result.negative = false;
   result.big_value = divide(left.big_value, right.big_value).second;
   return result;
}

bool operator== (const bigint& left, const bigint& right)
{
   return left.negative == right.negative and left.big_value.size() == right.big_value.size();
}

bool operator< (const bigint& left, const bigint& right)
{
   if ((left.negative and not right.negative) or (left.big_value.size() < right.big_value.size()))
      return true;
   else if (left.big_value.size() > right.big_value.size())
      return false;
   else if (left.big_value.size() == right.big_value.size())
   {
      auto itor_left = left.big_value.crbegin();
      auto itor_right = right.big_value.crbegin();
      while (itor_left != left.big_value.crend() or itor_right != right.big_value.crend())
      {
         if (*itor_left < *itor_right)
            return true;
         else if (*itor_left > *itor_right)
            return false;
         ++itor_left;
         ++itor_right;
      }
   }
   return false;
}

ostream& operator<< (ostream& out, const bigint& that)
{
   if (that.big_value.empty() and that.negative == false)
      out << '0';
   else if (that.negative == true)
      out << "-";
   for (auto itor = that.big_value.crbegin(); itor != that.big_value.crend(); itor++)
      out << *itor;
   return out;
}


bigint pow (const bigint& base, const bigint& exponent)
{
   bigint base_copy = base, zero {};
   if (exponent < zero)
      return zero;
   long expt = exponent.to_long();
   bigint result {"1"};
   while (expt > 0)
   {
      if (expt & 1) //odd
      {
         result = result * base_copy;
         --expt;
      }
      else //even
      {
         base_copy = base_copy * base_copy;
         expt /= 2;
      }
   }
   return result;
}
