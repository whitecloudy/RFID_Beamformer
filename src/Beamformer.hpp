#ifndef __BEAMFORMER__
#define __BEAMFORMER__

#include "AD5371_ctrl/Controller_Global.h"
#include "AD5371_ctrl/Phase_Attenuator_controller.hpp"
#include "Beam_algo/Beamtrainer.hpp"
#include "Beam_algo/Random_beamtrainer.hpp"
#include "SIC_ctrl/SIC_controller.hpp"
#include "Reader_comm/IPC_controller.hpp"
#include "struct_global.hpp"
#include <iostream>
#include <string>
#include <cstdio>
#include <thread>
#include <fstream>




class Beamformer{
  private:
    Phase_Attenuator_controller * phase_ctrl;
    IPC_controller ipc;
    std::ofstream log;
    SIC_controller * sic_ctrl;
    Random_beamtrainer * BWtrainer;

    int ant_amount;
    int * ant_nums;
    int cur_weights[ANT_num] = {};
    

  private:
    int weights_addition(int * dest_weights, int * weights0, int * weights1);
    int weights_addition(int * dest_weights, int * weights);

    void weights_printing(int * weights);
    int vector2cur_weights(std::vector<int> weightVector);

    int weights_apply(int * weights);
    int weights_apply(void);

    int init_beamformer(void);
    int run_beamformer(void);
    int calculate_beamforming_weights(void);

    int SIC_port_measure(void);
    int SIC_port_measure_over(void);
    int SIC_handler(struct average_corr_data &);
    int Signal_handler(struct average_corr_data &);
    int dataLogging(struct average_corr_data &);

  public:
    Beamformer(Phase_Attenuator_controller * controller, int ant_amount, int * ant_num);
    ~Beamformer();
    int start_beamformer(void);

};

#endif

