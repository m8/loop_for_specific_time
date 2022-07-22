#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <unistd.h>

#include "helpers.hpp"

#define NDEBUG

#define ERROR_MARGIN 1/100
#define SAMPLE_SIZE 100

std::vector<uint64_t> 
RUN_FOR_US = {1,10,100,1000};

void test_1(uint64_t _us_to_run)
{
    uint64_t _ns_to_run = _us_to_run * 1000;
    uint64_t num_epoch = (_ns_to_run / 5000);
    uint64_t final_epoch = _ns_to_run % 5000;
    
    uint64_t i = 0, j = 0;

    uint64_t START_TIME = timers::rdtsc();
    do
    {
        i += 1;        

        if (unlikely(i == 5000))
        {
            j += 1;
            i = 0;
            asm volatile("nop");
        }
    } while (!(num_epoch == j && final_epoch == i));
    uint64_t END_TIME = timers::rdtsc();

    log::print("[Test %d]: Performance (ito cc): %d\n", 3,
                1, _us_to_run, END_TIME - START_TIME - estimate_rdtsc_latency());
}

void test_2(uint64_t _us_to_run)
{
    uint64_t _ns_to_run = _us_to_run * 1000;
    uint64_t i = 0;

    uint64_t START_TIME = timers::rdtsc();
    do
    {
        i += 1;        

        if (unlikely((i % 5000) == 0))
        {
            asm volatile("nop");
        }
    } while (i < _ns_to_run);
    uint64_t END_TIME = timers::rdtsc();

    log::print("[Test %d - %d]: Performance (ito cc): %d\n", 3,
                2, _us_to_run, END_TIME - START_TIME - estimate_rdtsc_latency());
}

int main()
{
    for (auto ms : RUN_FOR_US)
    {
        for (size_t i = 0; i < SAMPLE_SIZE; i++)
        {
            test_1(ms);
            test_2(ms);
        }
    }
}