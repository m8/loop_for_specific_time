#pragma once

#include <sys/time.h>
#include <x86intrin.h>
#include <chrono>
#include <cstdarg>

#define PRETTY_PRINT

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

namespace timers
{
    static inline uint64_t get_us()
    {
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
    }

    static inline uint64_t rdtsc()
    {
        return __rdtsc();
    }

    static inline uint64_t rdtscp()
    {
        unsigned int k;
        return __rdtscp(&k);
    }
}

namespace log
{
    void print(const char* format, unsigned int count, ...)
    {
        va_list args;
        va_start(args, format);
        
        #ifdef PRETTY_PRINT
            vfprintf(stdout, format, args);
        #else
            std::string str = "";

            // REMARK: use for integers 
            for (int i = 0; i < count; i++)
            {
                int l = va_arg(args, int);
                str += std::to_string(l);
                str.push_back(',');
            }

            str.pop_back();
            printf("%s\n", str.c_str()); 
        #endif
        va_end(args);
    }
} // namespace log


static uint64_t estimate_gettime_latency()
{   
    uint64_t __attribute__((no_reorder)) start = 0;
    uint64_t __attribute__((no_reorder)) end = 0;

    start = timers::get_us();
    asm volatile ("nop"); // Assume 0
    end = timers::get_us();

    return (end - start);
}

static uint64_t estimate_rdtsc_latency()
{   
    uint64_t __attribute__((no_reorder)) start = 0;
    uint64_t __attribute__((no_reorder)) end = 0;

    start = timers::rdtsc();
    asm volatile ("nop");   // Assume 1
    end = timers::rdtsc();

    return (end - start - 1);
}

static uint64_t estimate_rdtscp_latency()
{   
    uint64_t __attribute__((no_reorder)) start = 0;
    uint64_t __attribute__((no_reorder)) end = 0;

    start = timers::rdtscp();
    asm volatile ("nop");   // Assume 1
    end = timers::rdtscp();

    return (end - start - 1);
}

static float estimate_clock_speed()
{
    // touch
    uint64_t i = 0;
    do
    {
        i += 1;
    } while (i < 100000);

    
    i = 0;
    // Get clock cycle
    uint64_t START_CC = timers::rdtscp();
    do
    {
        i += 1;
    } while (i < 100000);
    uint64_t END_CC = timers::rdtscp();


    i = 0;
    // Get time ellapsed
    auto START_TIME = std::chrono::high_resolution_clock::now();
    do
    {
        i += 1;
    } while (i < 100000);
    auto END_TIME = std::chrono::high_resolution_clock::now();


    long NSECOND = std::chrono::duration_cast<std::chrono::nanoseconds>(END_TIME-START_TIME).count();
    NSECOND = NSECOND - estimate_gettime_latency();

    long ELLAPSED_CC = END_CC - START_CC;
    ELLAPSED_CC = ELLAPSED_CC - estimate_rdtsc_latency();
    

    float estimated_speed = (float)ELLAPSED_CC / (float)NSECOND;
    return estimated_speed;
}
