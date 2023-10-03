#ifndef _MATH_H_
#define _MATH_H_

#include <stdint.h>

uint32_t abs(int32_t n);
double pow(double n, int32_t e);
double sqrt(double a);
uint32_t log10int(uint32_t n);
uint32_t rand(uint32_t max);
void srand(uint32_t seed);
void int_to_string(uint32_t n, char* destination);
float sin(float rads);
float cos(float rads);

extern const float pi;

#endif