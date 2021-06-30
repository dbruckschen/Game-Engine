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

} Timer;

__declspec(dllexport) void InitTimer(Timer *t);
__declspec(dllexport) void StartTimer(Timer *t);
__declspec(dllexport) void EndTimer(Timer *t);

#endif
