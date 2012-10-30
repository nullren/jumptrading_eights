#ifndef __JUMP_CHALLENGE_DATA_H__
#define __JUMP_CHALLENGE_DATA_H__

#include <cstdlib>

////////////////////////////////////////////////////////////
// Creates a data set n data points, where each data point
// contains 8 floats ranging from 0 to 1.
////////////////////////////////////////////////////////////
#define DATA_DIM 8

class Data
{
public:
  Data(int n);
  ~Data();

  // Returns number of data points in data set.
  int size() const;

  // Returns data point at index. Data point
  // is float array of length 8.
  const float* value(int index) const;

private:
  const int n_;
  float* const values_;
};


////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////
static float random(float min, float max)
{
  int r = rand();
  r = (r == 0 ? 1 : r);
  return ((float(r) / float(RAND_MAX)) * (max - min)) + min;
}

inline Data::Data(int n)
  : n_(n), values_(new float[n*DATA_DIM])
{
  for (int i=0; i < n_; ++i) {
    for (int j=0; j < DATA_DIM; ++j)
      values_[i*DATA_DIM+j] = random(0, 1);
  }
}

inline Data::~Data()
{
  delete[] values_;
}

inline int Data::size() const
{
  return n_;
}

inline const float* Data::value(int index) const
{
  return values_ + (index * DATA_DIM);
}

#endif

