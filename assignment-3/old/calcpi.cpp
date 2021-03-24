// ======================================================================
// You must modify this file and then submit it for grading to D2L.
// ======================================================================
//
// count_pi() calculates the number of pixels that fall into a circle
// using the algorithm explained here:
//
// https://en.wikipedia.org/wiki/Approximations_of_%CF%80
//
// count_pixels() takes 2 paramters:
//  r         =  the radius of the circle
//  n_threads =  the number of threads you should create
//
// Currently the function ignores the n_threads parameter. Your job is to
// parallelize the function so that it uses n_threads threads to do
// the computation.

#include "calcpi.h"

uint64_t count_pixels(int r, int n_threads)
{
  double rsq = double(r) * r;
  uint64_t count = 0;
  for( double x = 1 ; x <= r ; x ++)
    for( double y = 0 ; y <= r ; y ++)
      if( x*x + y*y <= rsq) count ++;
  return count * 4 + 1;
}
