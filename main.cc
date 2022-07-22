#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <unistd.h>

#include "helpers.hpp"

#define NDEBUG

#define ERROR_MARGIN 1/100
#define SAMPLE_SIZE 10

std::vector<uint64_t> 
RUN_FOR_US = {5,10,100};

long RDTSCP_LATENCTY = 0;

void test_1(uint64_t _us_to_run)
{
    uint64_t _ns_to_run = (_us_to_run * 1000);
    uint64_t num_epoch = (_ns_to_run / 5000);
    uint64_t final_epoch = _ns_to_run % 5000;
    uint64_t i = 0, j = 0;


    uint64_t START_TIME = timers::get_us();
    do
    {
        i += 1;        

        if (unlikely(i == 5000))
        {
            j += 1;
            i = 0;
        }
    } while (!(num_epoch == j && final_epoch == i));
    uint64_t END_TIME = timers::get_us();


    log::print("[Test 1]: Expected: %d , got: %d\n", 2, _us_to_run, END_TIME - START_TIME - estimate_gettime_latency());
}

void test_2(uint64_t _us_to_run)
{
    uint64_t _ns_to_run = _us_to_run * 1000;
    uint64_t i = 0;


    uint64_t START_TIME = timers::get_us();
    do
    {
        i += 1;        

        if (unlikely((i % 5000) == 0))
        {
            asm volatile("nop");
        }
    } while (i < _ns_to_run);
    uint64_t END_TIME = timers::get_us();


    log::print("[Test 2]: Expected: %d , got: %d\n", 2, _us_to_run, END_TIME - START_TIME - estimate_gettime_latency());
}

void test_3(uint64_t _us_to_run)
{
    float estimated_clock_speed = estimate_clock_speed();

    uint64_t _ns_to_run = (_us_to_run * 1000) / estimated_clock_speed / 1.3;
    uint64_t i = 0;


    uint64_t START_TIME = timers::get_us();
    do
    {
        i += 1; 

        if (unlikely((i % 5000) == 0))
        {
            asm volatile("nop");
        }       
    } while (i < _ns_to_run);
    uint64_t END_TIME = timers::get_us();

    log::print("[Test 3]: Expected: %d , got: %d\n", 2, _us_to_run, END_TIME - START_TIME - estimate_gettime_latency());
}

float calc_function_cc()
{
    uint64_t counter = 0;
    int t = 0;

    uint64_t _s = timers::rdtscp();
    while (t < 1000000)
    {
        t += 1; 
        if (unlikely((t % 5000) == 0))
        {
            asm volatile("nop");
        }
    }
    uint64_t _t = timers::rdtscp();
    
    counter += (_t - _s - RDTSCP_LATENCTY);
    return ((float)counter / 1000000);
}

void test_4(uint64_t _us_to_run)
{
    uint64_t i = 0;
    uint64_t CLOCK_TIMESTAMP = 0;

    float estimated_clock_speed = estimate_clock_speed();
    float clock_cycle_per_iter = calc_function_cc();
    float nanosec_per_iter = (float)clock_cycle_per_iter / (float)estimated_clock_speed;
    uint64_t _stop_at = 5000 / (float)nanosec_per_iter;


    log::print("Estimated clock speed: %f , number of cc: %f, ns per iter: %f, will stop at: %d\n", 4,
                        estimated_clock_speed, clock_cycle_per_iter, nanosec_per_iter, _stop_at);

    uint64_t _ns_to_run = (_us_to_run * 1000) / nanosec_per_iter / 1.5;
    printf("will run for %d\n", _ns_to_run);

    uint64_t START_TIME = timers::get_us();
    do
    {
        i += 1; 

        if (unlikely((i % (_stop_at)) == 0))
        {
            // CLOCK_TIMESTAMP = timers::get_us();
            asm volatile("nop");
        }       
    } while (i < _ns_to_run);
    uint64_t END_TIME = timers::get_us();

    log::print("[Test 4]: Expected: %d , got: %d\n", 2, _us_to_run, END_TIME - START_TIME - estimate_gettime_latency());
    log::print("[Test 4] Yield fired at: %d\n", 1, CLOCK_TIMESTAMP - START_TIME - estimate_gettime_latency());
}



void init_setup()
{
    for (size_t i = 0; i < 1000; i++)
    {
        RDTSCP_LATENCTY += estimate_rdtscp_latency();
    }
    
    RDTSCP_LATENCTY = (float)RDTSCP_LATENCTY / 1000;
}


int main()
{
    init_setup();

    for (auto ms : RUN_FOR_US)
    {
        for (size_t i = 0; i < SAMPLE_SIZE; i++)
        {
            test_1(ms);
            test_2(ms);
            test_3(ms);
            test_4(ms);
        }
    }
}