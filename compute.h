#include "Data.h"
#include <stdio.h>
#include <cmath>
#include <smmintrin.h>

float compute(const Data& d);

float ok_power(const float);
float (*power)(const float) = ok_power;

float compute_hmean(const float *);
float sse_compute_hmean(const float *);
float (*hmean)(const float *) = sse_compute_hmean;

void kahan_action(const float, float *, float *);
float kahan_sum(const float *, const int, const int);
float (*sum)(const float *, const int, const int) = kahan_sum;

__m128 sse_sqr(const __m128);
__m128 sse_power(const __m128);
__m128 v1 = _mm_set_ps(1.0f, 1.0f, 1.0f, 1.0f);

float compute(const Data& d)
{
  int N = d.size();

  float avg = 0;
  float c = 0;

  for(int i = 0; i < N; ++i)
  {
    const float *v = d.value(i);
    float m = hmean(v);

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

  for(int j = i+1; j < n; ++j)
    kahan_action(x[j], &s, &c);

  return s;
}

float ok_power(const float d)
{
  float f = sqrtf(sqrtf(sqrtf(d)));
  float g = pow(pow(pow(d,2),2),2);
  return 1/(f*g);
}

float compute_hmean(const float *x)
{
  float y[8];

  // raise power and invert
  for(int i=0; i<8; ++i)
    y[i] = power(x[i]);

  float s = sum(y, 0, 8);
  float r = 8/s;

  if( std::isnan(r) )
    r = 0;

  return r;
}

__m128 sse_sqr(const __m128 d)
{
  return _mm_mul_ps(d,d);
}

__m128 sse_power(const __m128 d)
{
  __m128 f = _mm_rsqrt_ps(_mm_rsqrt_ps(_mm_rsqrt_ps(d)));
  __m128 g = sse_sqr(sse_sqr(sse_sqr(d)));
  return _mm_div_ps(f,g);
}

float sse_compute_hmean(const float *x)
{
  float y[8];

  __m128 *sx = (__m128*)x;
  __m128 *sy = (__m128*)y;

  for(int i=0; i<2; ++i)
    sy[i] = sse_power(sx[i]);

  sy[0] = _mm_add_ps(sy[0], sy[1]);
  sy[0] = _mm_dp_ps(sy[0], v1, 241);

  return 8/y[0];
}
