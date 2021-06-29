#include "timing.h"

void InitTiming(Timing *t)
{
    LARGE_INTEGER freq = {0};
    QueryPerformanceFrequency(&freq);

    t->perf_freq = freq.QuadPart;
}

void StartTiming(Timing *t)
{
    LARGE_INTEGER start_time = {0};
    QueryPerformanceCounter(&start_time);

    t->start_time = start_time.QuadPart;
}

void EndTiming(Timing *t)
{
    LARGE_INTEGER end_time = {0};
    QueryPerformanceCounter(&end_time);

    t->end_time = end_time.QuadPart;

    t->elapsed_time = t->end_time - t->start_time;

}

void ElapsedTiming(Timing *t)
{

}






