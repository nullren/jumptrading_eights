#include "Data.h"
#include <stdio.h>
#include <cmath>

float compute(const Data& d);

float bad_power(const float);
float ok_power(const float);
float (*power)(const float) = ok_power;

float compute_hmean(const float *);

void print_tuple(const float *);

float bad_sum(const float *, const int, const int);
float kahan_sum(const float *, const int, const int);
float (*sum)(const float *, const int, const int) = kahan_sum;

float compute(const Data& d)
{
  int N = d.size();

  float avg = 0;

  //printf("Mean @@ {HarmonicMean[Function[x,x^(8 + 1/8)]/@#]&/@{\n");
  //printf("}}\n");

  for(int i = 0; i < N; i++){

    const float *v = d.value(i);

    float m = compute_hmean(v);
    float a = (i*avg + m)/(i+1);
    avg = a;

    //printf("%f:%f: ", a, m);
    //print_tuple(v);
  }

  return avg;
}

float kahan_sum(const float *x, const int i, const int n){
  float s = x[i];
  float c = 0;
  for(int j = i+1; j < n; j++){
    float y = x[j] - c;
    float t = s + y;
    c = (t - s) - y;
    s = t;
  }
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