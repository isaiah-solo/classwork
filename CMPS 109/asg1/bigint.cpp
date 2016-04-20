// $Id: bigint.cpp,v 1.61 2014-06-26 17:06:06-07 - - $

#include <cstdlib>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"

bigint::bigint (long that): big_value (that) {
   if (that < 0)
      negative = true;
   else
      negative = false;
}

bigint::bigint (const string& that) {
   auto itor = that.cbegin();
   negative = false;
   if (itor != that.cend() and *itor == '_') {
      negative = true;
      ++itor;
   }
   int newval = 0;
   while (itor != that.end()) newval = newval * 10 + *itor++ - '0';
   big_value = newval;
}

bigvalue_t do_bigadd (const bigvalue_t& left, const bigvalue_t& right)
{
   int current = 0;
   bigvalue_t new_big_value = {}, left_ = left, right_ = right;
   if (left_.size() > right_.size())
      right_.resize(left_.size(), '0');
   else
      left_.resize(right.size(), '0');
   new_big_value.resize(left.size() + 1, '0');
   for (int i = 0; i < left_.size(); i++)
   {
      current += (left_.at(i) - '0') + (right_.at(i) - '0');
      if (current > 9)
      {
         current -= 10;
         new_big_value.at(i) = current + '0';
         current = 1;
      }
      else
      {
         new_big_value.at(i) = current + '0';
         current = 0;
      }
   }
   while (new_big_value.back() == '0')
      new_big_value.resize(new_big_value.size() - 1);
   return new_big_value;
}

bigvalue_t do_bigsub (const bigvalue_t& left, const bigvalue_t& right)
{
   int current = 0;
   bigvalue_t new_big_value = {}, left_ = left, right_ = right;
   if (left_.size() > right_.size())
      right_.resize(left_.size(), '0');
   else
      left_.resize(right.size(), '0');
   new_big_value.resize(left.size(), '0');
   for (int i = 0; i < left_.size(); i++)
   {
      if (left_.at(i) + current < right_.at(i))
      {
         current += (left_.at(i) - '0' + 10) - (right_.at(i) - '0');
         new_big_value.at(i) = current + '0';
         current = -1;
      }
      else
      {
         current += (left_.at(i) - '0') + (right_.at(i) - '0');
         new_big_value.at(i) = current + '0';
         current = 0;
      }
   }
   while (new_big_value.back() == '0')
      new_big_value.resize(new_big_value.size() - 1);
   return new_big_value;
}

bigvalue_t do_bigmul (const bigvalue_t& left, const bigvalue_t& right)
{
   bigvalue_t new_big_value = {}, left_ = left, right_ = right;
   new_big_value.resize(left.size() + right.size(), '0');
   for (int i = 0; i < left_.size(); i++)
   {
      long c = 0;
      for (int j = 0; j < right_.size(); j++)
      {
         long d = (new_big_value.at(i + j) - '0') + ((left_.at(i) - '0') * (right_.at(i) - '0')) + c;
         new_big_value.at(i + j) = d % 10;
         c = d / 10;
      }
      new_big_value.at(i + right.size()) = c;
   }
   return new_big_value;
}

bool do_bigless (const bigvalue_t& left, const bigvalue_t& right)
{
   if (left.size() < right.size())
      return true;
   else if (left.size() > right.size())
      return false;
   else
   {
      for (int i = 0; i < left.size(); i++)
      {
         if (left.at(i) < right.at(i))
            return true;
         else if (left.at(i) > right.at(i))
            return false;
      }
   }
   return false;
}


bigint operator+ (const bigint& left, const bigint& right) {
   bigint result = left;
   if (left.negative == right.negative)
      result.big_value = do_bigadd(left.big_value, right.big_value);
   else
   {
      if (do_bigless(left.big_value, right.big_value))
      {
         result.big_value = do_bigsub(right.big_value, left.big_value);
         result.negative = right.negative;
      }
      else
         result.big_value = do_bigsub(left.big_value, right.big_value);
   }
   return result;
}

bigint operator- (const bigint& left, const bigint& right) {
   bigint result = left;
   if (left.negative == right.negative)
   {
      if (do_bigless(left.big_value, right.big_value))
      {
         result.big_value = do_bigsub(right.big_value, left.big_value);
         result.negative = right.negative;
      }
      else
         result.big_value = do_bigsub(left.big_value, right.big_value);
   }
   else
      result.big_value = do_bigadd(left.big_value, right.big_value);
   return result;
}

bigint operator+ (const bigint& right) {
   return right;
}

bigint operator- (const bigint& right) {
   bigint right_ = right;
   right_.negative = not right_.negative;
   return right_;
}

long bigint::to_long() const {
   if (*this <= bigint (numeric_limits<long>::min())
    or *this > bigint (numeric_limits<long>::max()))
               throw range_error ("bigint__to_long: out of range");
   return long_value;
}

bool abs_less (const long& left, const long& right) {
   return left < right;
}

//
// Multiplication algorithm.
//
bigint operator* (const bigint& left, const bigint& right) {
   bigint result = left;
   if (left.negative == right.negative)
      result.negative = false;
   else
      result.negative = true;
   result.big_value = do_bigmul(left.big_value, right.big_value);
   return result;
}

//
// Division algorithm.
//

void multiply_by_2 (bigint::unumber& unumber_value) {
   unumber_value *= 2;
}

void divide_by_2 (bigint::unumber& unumber_value) {
   unumber_value /= 2;
}


bigint::quot_rem divide (const bigint& left, const bigint& right) {
   if (right == 0) throw domain_error ("divide by 0");
   using unumber = unsigned long;
   static unumber zero = 0;
   if (right == 0) throw domain_error ("bigint::divide");
   unumber divisor = right.long_value;
   unumber quotient = 0;
   unumber remainder = left.long_value;
   unumber power_of_2 = 1;
   while (abs_less (divisor, remainder)) {
      multiply_by_2 (divisor);
      multiply_by_2 (power_of_2);
   }
   while (abs_less (zero, power_of_2)) {
      if (not abs_less (remainder, divisor)) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divide_by_2 (divisor);
      divide_by_2 (power_of_2);
   }
   return {quotient, remainder};
}

bigint operator/ (const bigint& left, const bigint& right) {
   return divide (left, right).first;
}

bigint operator% (const bigint& left, const bigint& right) {
   return divide (left, right).second;
}

bool operator== (const bigint& left, const bigint& right) {
   return left.negative == right.negative and left.big_value.size() == right.big_value.size()
}

bool operator< (const bigint& left, const bigint& right) {
   if ((left.negative and not right.negative) or (left.size() < right.size())
      return true;
   else if (left.big_value.size() == right.big_value.size())
   {
      for (int i = 0; i < left.big_value.size(); i++)
      {
         if (left.big_value.at(i) < right.big_value.at(i))
            return true;
         else if (left.big_value.at(i) > right.big_value.at(i))
            return false;
      }
   }
   return false;
}

ostream& operator<< (ostream& out, const bigint& that) {
   for (int i = 0; i < that.big_value.size(); i++)
      out << that.big_value.at(i);
   out << endl;
   return out;
}


bigint pow (const bigint& base, const bigint& exponent) {
   DEBUGF ('^', "base = " << base << ", exponent = " << exponent);
   if (base == 0) return 0;
   bigint base_copy = base;
   long expt = exponent.to_long();
   bigint result = 1;
   if (expt < 0) {
      base_copy = 1 / base_copy;
      expt = - expt;
   }
   while (expt > 0) {
      if (expt & 1) { //odd
         result = result * base_copy;
         --expt;
      }else { //even
         base_copy = base_copy * base_copy;
         expt /= 2;
      }
   }
   DEBUGF ('^', "result = " << result);
   return result;
}
