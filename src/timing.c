#include "timing.h"

void InitTimer(Timer *t)
{
    LARGE_INTEGER freq = {0};
    QueryPerformanceFrequency(&freq);

    t->perf_freq = (double)freq.QuadPart;
}

void StartTimer(Timer *t)
{
    LARGE_INTEGER start_time = {0};
    QueryPerformanceCounter(&start_time);

    t->start_time = (double)start_time.QuadPart;
}

void EndTimer(Timer *t)
{
    LARGE_INTEGER end_time = {0};
    QueryPerformanceCounter(&end_time);

    t->end_time = (double)end_time.QuadPart;

    t->elapsed_time = t->end_time - t->start_time;
    t->elapsed_time /= t->perf_freq;
}






