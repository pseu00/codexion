#include "struct.h"
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h> 

long long get_time(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void ft_usleep(long long time_in_ms)
{
    long long start;

    start = get_time();
    while ((get_time() - start) < time_in_ms)
    {
        ft_usleep(100);
    }
}