#include <UnitTest++/UnitTest++.h>
#include <gaussian_distribution.hpp>
#include <exponential_distribution.hpp>

SUITE(TestUtilDistributions) {
  using namespace freedom;

  TEST(TestGauss){
   GaussianDistribution dist(0,1); 

   CHECK_CLOSE(dist(1),dist(-1),0.00001);
   CHECK_CLOSE(dist(2),dist(-2),0.00001);
   CHECK_CLOSE(dist(0.1),dist(-0.1),0.00001);
  }

  TEST(TestExponential){
   ExponentialDistribution dist(1); 
   CHECK_CLOSE(dist(0),1,1);
   CHECK_CLOSE(dist(5),0,0.01);
  }
}

