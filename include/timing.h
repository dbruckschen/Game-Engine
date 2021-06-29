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

void InitTiming(Timing *t);
void StartTiming(Timing *t);
void EndTiming(Timing *t);
void ElapsedTiming(Timing *t);


#endif
