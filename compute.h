#include "Data.h"
#include <stdio.h>
#include <cmath>

float compute(const Data& d);

float ok_power(const float);
float (*power)(const float) = ok_power;

float compute_hmean(const float *);

void kahan_action(const float, float *, float *);
float kahan_sum(const float *, const int, const int);
float (*sum)(const float *, const int, const int) = kahan_sum;

float compute(const Data& d)
{
  int N = d.size();

  float avg = 0;
  float c = 0;

  for(int i = 0; i < N; i++)
  {
    const float *v = d.value(i);
    float m = compute_hmean(v);

    kahan_action(m, &avg, &c);
  }

  return avg/N;
}

void kahan_action(const float x, float *s, float *c)
{
  float y = x - *c;
  float t = *s + y;

  *c = (t - *s) - y;
  *s = t;
}

float kahan_sum(const float *x, const int i, const int n)
{
  float s = x[i];
  float c = 0;

  for(int j = i+1; j < n; j++)
    kahan_action(x[j], &s, &c);

  return s;
}

float ok_power(const float d)
{
  float f = sqrt(sqrt(sqrt(d)));
  float g = pow(pow(pow(d,2),2),2);
  return 1/(f*g);
}

float compute_hmean(const float *x)
{
  float y[8];

  // raise power and invert
  for(int i=0; i<8; i++)
    y[i] = power(x[i]);

  float s = sum(y, 0, 8);
  float r = 8/s;

  if( std::isnan(r) )
    r = 0;

  return r;
}
