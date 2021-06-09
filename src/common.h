#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <assert.h>

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef signed char s8;
typedef signed short int s16;
typedef signed int s32;
typedef signed long long int s64;

typedef float f32;
typedef double f64;

int StringLen(char *s);
bool StringCmp(char *s1, char *s2);
char *StringConnect(char *s1, char *s2);

#endif //COMMON_H
