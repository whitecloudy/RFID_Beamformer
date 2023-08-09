#ifndef __LOGGER__
#define __LOGGER__

#include <iostream>
#include <fstream>
#include <vector>
#include "global/struct_global.hpp"

class Logger{
    int ant_amount = 0;
    std::vector<int> ant_nums;
    std::ofstream log, optimal_log;

    public:
        Logger(std::vector<int> ant_nums);
        ~Logger();
        int dataLogging(int * cur_weights, const struct average_corr_data &, double sic_power, int counter, int beamforming_count, bool optimal=false, const int which_op = 0);
};

#endif