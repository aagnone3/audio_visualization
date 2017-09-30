#include "shared.hpp"

int mod(int dividend, int divisor)
{
  if (divisor == 0) {
    throw std::domain_error("Attempted modulo when bufferSizeSamples is zero!");
  }

  int r = dividend%divisor;
  if (r<0) {
    r += divisor;
  }
  return r;
}
