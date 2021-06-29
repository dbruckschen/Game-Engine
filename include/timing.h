#ifndef TIME_H
#define TIME_H

#include <windows.h>
#include "common.h"

typedef struct
{
    double perf_freq;
    double start_time;
    double end_time;
    double elapsed_time;

} Timing;

__declspec(dllexport) void InitTiming(Timing *t);
__declspec(dllexport) void StartTiming(Timing *t);
__declspec(dllexport) void EndTiming(Timing *t);

#endif
