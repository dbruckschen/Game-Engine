#ifndef TIME_H
#define TIME_H

#include <windows.h>
#include "common.h"

struct Timer {
    double perf_freq;
    double start_time;
    double end_time;
    double elapsed_time;
};

__declspec(dllexport) void InitTimer(struct Timer *t);
__declspec(dllexport) void StartTimer(struct Timer *t);
__declspec(dllexport) void EndTimer(struct Timer *t);

#endif
