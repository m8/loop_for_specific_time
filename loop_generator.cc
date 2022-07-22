#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <unistd.h>

#include "helpers.hpp"

#define NDEBUG

#define ERROR_MARGIN 1/100
#define SAMPLE_SIZE 2

std::vector<uint64_t> 
RUN_FOR_US = {1,5,10,50,100,250,500};


uint64_t generate_loop(uint64_t _us_to_run, uint64_t schedule_delta, float magic)
{
    uint64_t yield_cost_cc = 30;

    uint64_t _ns_to_run = (_us_to_run * 1000) / estimate_clock_speed() / magic;
    uint64_t num_epoch = (_ns_to_run - (_ns_to_run / schedule_delta)*yield_cost_cc) / schedule_delta;
    uint64_t final_epoch = (_ns_to_run - (_ns_to_run / schedule_delta)*yield_cost_cc) % schedule_delta;
    std::vector<uint64_t> yield_at = {};

    uint64_t i = 0, j = 0;

    uint64_t START_TIME = timers::get_us();
    do
    {
        i += 1;        

        if (unlikely(i == schedule_delta))
        {
            yield_at.push_back(timers::get_us());
            j += 1;
            i = 0;
        }
    } while (!(num_epoch == j && final_epoch == i));
    uint64_t END_TIME = timers::get_us();

    uint64_t RES = END_TIME - START_TIME - estimate_gettime_latency();

    std::printf("Yield deltas: ");
    for (size_t i = 0; i < yield_at.size(); i++)
    {
        if(i == 0)
            std::printf("%d,", yield_at[0] - START_TIME - estimate_gettime_latency());
        else
            std::printf("%d,", yield_at[i] - yield_at[i-1] - estimate_gettime_latency());
    }
    std::printf("\n");
    log::print("Expected %d Res: %d \n", 2, _us_to_run, RES);
}

int main()
{
    for (auto ms : RUN_FOR_US)
    {
        for (size_t i = 0; i < SAMPLE_SIZE; i++)
        {
            generate_loop(ms, 5000 / estimate_clock_speed(), 1.4);
        }
    }
}