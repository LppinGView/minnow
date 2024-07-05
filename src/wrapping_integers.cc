#include "wrapping_integers.hh"
#include <iostream>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  return Wrap32 { zero_point.operator+(static_cast<uint32_t> (n % (1LL << 32))) };
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  uint64_t cpmod = checkpoint / (1LL << 32);
  if (operator==(zero_point)){
    return ((checkpoint % (1LL << 32)) <= ((1LL << 32)/2))? (cpmod << 32) : (cpmod + 1) <<32;
  }
  auto net_value = raw_value_ < zero_point.raw_value_? ((1LL << 32) - zero_point.operator+(-raw_value_).raw_value_) : (operator+(-zero_point.raw_value_).raw_value_);
  auto estimate_value = (cpmod << 32) + net_value;
  auto near_value = estimate_value <= checkpoint? checkpoint - estimate_value : estimate_value - checkpoint;
  return near_value <= ((1LL << 32)/2)? estimate_value : (estimate_value <= checkpoint? (((cpmod + 1)<<32) + net_value) : ((cpmod>0?cpmod-1:cpmod) <<32) + net_value);
}