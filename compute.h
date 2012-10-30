#include "Data.h"
#include <stdio.h>
#include <cmath>

//#define MATHEMATICA_TEST
#define MATHEMATICA_TEST_LIMIT 10000

float compute(const Data& d);

float bad_power(const float);
float ok_power(const float);
float (*power)(const float) = ok_power;

float compute_hmean(const float *);

void print_tuple(const float *);

float bad_sum(const float *, const int, const int);
void kahan_action(const float, float *, float *);
float kahan_sum(const float *, const int, const int);
float (*sum)(const float *, const int, const int) = kahan_sum;

float compute(const Data& d)
{
#ifdef MATHEMATICA_TEST
  int N = MATHEMATICA_TEST_LIMIT;
#else
  int N = d.size();
#endif

  float avg = 0;
  float c = 0;

#ifdef MATHEMATICA_TEST
  printf("Mean @@ {HarmonicMean[Function[x,x^(8 + 1/8)]/@#]&/@{\n");
#endif

  for(int i = 0; i < N; i++){

    const float *v = d.value(i);
    float m = compute_hmean(v);

    kahan_action(m, &avg, &c);

#ifdef MATHEMATICA_TEST
    print_tuple(v);
#endif
  }

#ifdef MATHEMATICA_TEST
  printf("}}\n");
#endif

  return avg/N;
}

void kahan_action(const float x, float *s, float *c){
  float y = x - *c;
  float t = *s + y;

  *c = (t - *s) - y;
  *s = t;
}

float kahan_sum(const float *x, const int i, const int n){
  float s = x[i];
  float c = 0;

  for(int j = i+1; j < n; j++)
    kahan_action(x[j], &s, &c);

  return s;
}

float bad_sum(const float *x, const int i, const int n){
  float s = x[i];
  for(int j = i+1; j < n; j++)
    s = s + x[j];
  return s;
}

float bad_power(const float d){
  return pow(d, -(8 + (float)(1/8)));
}

float ok_power(const float d){
  float f = sqrt(sqrt(sqrt(d)));
  float g = pow(pow(pow(d,2),2),2);
  return 1/(f*g);
}

float compute_hmean(const float *x){
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

void print_tuple(const float *x){
  printf("{%f", x[0]);
  for(int i=1; i<8; i++)
    printf(",%f", x[i]);
  printf("},\n");
}
