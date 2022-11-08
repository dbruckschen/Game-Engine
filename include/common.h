#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

__declspec(dllexport) long long StringLen(char *s);
__declspec(dllexport) bool StringCmp(char *s1, char *s2);

// the last character will be the null terminator 
__declspec(dllexport) void StringCpy(char *dst, char *src, size_t dst_len);
__declspec(dllexport) bool CharBelongsToText(char c);
__declspec(dllexport) bool CharBelongsToAlphabet(char c); 
__declspec(dllexport) int CharToInt(char *s);
__declspec(dllexport) void IntToString(int i, char *buff, int buff_len);

#endif //COMMON_H
