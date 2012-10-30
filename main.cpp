#include "Data.h"
#include "compute.h"
#include <iostream>

#define NUM_DATA 8e6

int main()
{
  Data data(NUM_DATA);
  float result = compute(data);
  std::cout << "result = " << result << std::endl;
  return 0;
}

