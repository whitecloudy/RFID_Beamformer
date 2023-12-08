#ifndef __BEAMFORMER__
#define __BEAMFORMER__

#include "lib/AD5371_ctrl/Controller_Global.h"
#include "lib/AD5371_ctrl/Phase_Attenuator_controller.hpp"
#include "Beam_algo/Beam_algorithm.hpp"
#include "Beam_algo/Beamtrainer.hpp"
//#include "Beam_algo/Random_beamtrainer.hpp"
#include "lib/SIC/SIC_controller.hpp"
#include "lib/Reader_comm/IPC_controller.hpp"
#include "global/struct_global.hpp"
#include "Beam_algo/common/CA_calculator.hpp"
#include "Logger.hpp"
#include <iostream>
#include <string>
#include <cstdio>
#include <thread>
#include <memory>

#define BEAMFORMING_ROUND (85)
#define SIC_ADJUST_THRESHOLD (3)

class Beamformer{
  enum BeamformerStatus
  {
    TRAINING,
    BEAMFORMING
  } status = TRAINING;

  struct phase_amp_dataset
  {
    std::vector<int> phase;
    std::complex<float> amp;

    public:
      phase_amp_dataset(std::vector<int> phase_, average_corr_data data_);
      phase_amp_dataset(std::vector<int> phase_, std::complex<float> amp_);
  };

  int status_count;
  int beamforming_count = 0;
  int needSIC = true;
  int sic_adjust_trigger_count = 5;

  int training_round_max;

  private:
    std::unique_ptr<Phase_Attenuator_controller> phase_ctrl;
    std::unique_ptr<SIC_controller> sic_ctrl;
    std::unique_ptr<Beamtrainer> BWtrainer;

    Logger logger;
    
    IPC_controller ipc;
    bool sic_enabled = false;
    int ant_amount;   //TODO : this is actually not nessesary.... need to be depricated
    std::vector<int> ant_nums;
    int cur_weights[ANT_num] = {};  //TODO : change this more neet way

    int counter = 0;
    int opt_repeat_counter = 0;

    bool perfect_flag = false;

    std::vector<int> perfectVector;
    std::vector<int> curWeightVector;
    //std::vector<std::vector<int>> weight_stack;
    //std::vector<average_corr_data> data_stack;

    std::vector<struct phase_amp_dataset> weight_amp_stack;
    

  private:
    int weights_addition(int * dest_weights, int * weights0, int * weights1);
    int weights_addition(int * dest_weights, int * weights);

    void weights_printing(int * weights);
    int vector2cur_weights(std::vector<int> weightVector);
    int weights_apply(int * weights);
    int weights_apply(void);

    int stage_start(struct average_corr_data * data);
    int stage_finish(void);

    int init_beamformer(void);
    int run_beamformer(void);
    int calculate_beamforming_weights(void);

    int SIC_port_measure(void);
    int SIC_port_measure_over(void);
    int SIC_handler(const struct average_corr_data &);
    int SIC_adjustment(void);
    int Signal_handler(const struct average_corr_data &);

  public:
    Beamformer(std::vector<int> ant_nums, BEAM_ALGO::algorithm beam_algo, int sic_ant_num, std::vector<int> ant_array, int k);
    ~Beamformer();
    int start_beamformer(void);

};

#endif

