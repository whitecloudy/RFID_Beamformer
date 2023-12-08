#include "Beamformer.hpp"
#include <cstring>
#include <random>
#include <fstream>
#include <sys/time.h>
#include <cassert>


#define __COLLECT_DATA__
//#define __TIME_STAMP__

#define PREDEFINED_RN16_ (0x5555)
#define EXPECTED_TAG_NUM_ (1)

#define THRESHOLD_FOR_PERFECT_ (11500)

#define SIC_PORT_NUM_ ant_nums[ant_amount-1]

#define REPEAT_COUNT_ (1)


double normal_random(double mean, double std_dev){
  static std::random_device r;
  static std::default_random_engine generator(r());
  std::normal_distribution<double> distribution(mean, std_dev);

  return distribution(generator);
}


int Beamformer::stage_start(struct average_corr_data * data)
{
  char buffer[IO_BUF_SIZE] = {};

  if(ipc.data_recv(buffer) == -1){
    std::cerr <<"Breaker is activated"<<std::endl;
    return 1;
  }

  if(data != NULL)
    memcpy(data, buffer, sizeof(*data));

  return 0;
}


int Beamformer::stage_finish(void)
{
  phase_ctrl->data_apply();

  int send_bits = 0;
  if(needSIC)
    send_bits |= 0x01;

  if(status == BEAMFORMING)
    send_bits |= 0x02;

  if(ipc.send_ack(send_bits) == -1)
  {
    return 1;
  }
  return 0;
}



int Beamformer::run_beamformer(void){

  struct average_corr_data data;

  /******************** SIC measure stage *************/
  if(stage_start(NULL))
    return 0;

  SIC_port_measure();

  if(stage_finish())
    return 0;

  int sic_measure_count = 10;
  std::complex<float> avg_cw(0.0, 0.0);
  do
  {
    if(stage_start(&data))
      return 0;

    if(data.successFlag != _GATE_FAIL)
    {
      std::cout << std::complex<float>(data.cw_i, data.cw_q) << std::endl;

      avg_cw += std::complex<float>(data.cw_i, data.cw_q);

      sic_measure_count -= 1;

      if(sic_measure_count == 0)
      {
        avg_cw /= 10;

        std::cout << avg_cw << std::endl;
        sic_ctrl = std::unique_ptr<SIC_controller>(new SIC_controller(avg_cw));
        SIC_port_measure_over();
      }
    }

    //initial phase here
    if(stage_finish())
      return 0;
    std::cout << "Getting Ready" << std::endl;
  }while(data.successFlag == _GATE_FAIL || sic_measure_count > 0);
  /*****************************************************/

  std::cout << "READY"<<std::endl;
  //loop until it is over
  while(1){
    /******************* SIC stage *******************/
    do
    {
      std::cout << "SIC Phase" <<std::endl;
      if(stage_start(&data))
        return 0;

      //dataLogging(data);

      if(data.successFlag != _GATE_FAIL)
      {
        SIC_handler(data);    
      }
      else
      {
        std::cout << "SIC adjust"<<std::endl;
        SIC_adjustment();
      }

      //send ack so that Gen2 program can recognize that the beamforming has been done
      if(stage_finish())
        return 0;

    }while(data.successFlag == _GATE_FAIL);
    /*************************************************/



    /******************* Signal stage *****************/
    do
    {
      std::cout << "Signal Phase" <<std::endl;
      if(status == BEAMFORMING)
        std::cout << "BEAMFORMING" << std::endl;
      if(stage_start(&data))
        return 0;

      if(data.successFlag != _GATE_FAIL)
      {
        sic_adjust_trigger_count = SIC_ADJUST_THRESHOLD;
        Signal_handler(data);
      }
      else
      {
        sic_adjust_trigger_count--;
        if(sic_adjust_trigger_count == 0)
        {
          sic_adjust_trigger_count = SIC_ADJUST_THRESHOLD;
          if(status != BEAMFORMING)
            SIC_adjustment();
          else
            SIC_handler(data);  //What if we put SIC handling here?
        }
      }

      if(stage_finish())
        return 0;

    }while((data.successFlag == _GATE_FAIL)||!needSIC);
    /******************************************************/
  }//end of while(1)

  //print wait
  weights_printing(cur_weights);

  return 0;
}



Beamformer::Beamformer(std::vector<int> ant_nums_p, BEAM_ALGO::algorithm beam_algo, int sic_ant_num, std::vector<int> ant_array, int k) : logger(ant_nums_p)
{
  this->phase_ctrl = std::unique_ptr<Phase_Attenuator_controller>(new Phase_Attenuator_controller(0));
  this->ant_nums = ant_nums_p;
  this->ant_amount = ant_nums.size();
  this->training_round_max = 27 + k;
  this->BWtrainer = std::unique_ptr<Beamtrainer>(BEAM_ALGO::get_beam_class(ant_amount, training_round_max, beam_algo, ant_array));

  this->BWtrainer->printClassName();

  if(sic_ant_num != -1){
    sic_enabled = true;
    this->ant_nums.push_back(sic_ant_num); //TODO : SIC antenna shall be handled seperately
  }

  this->ant_amount++;
}



Beamformer::~Beamformer(){
}

int Beamformer::init_beamformer(void){
  std::vector<int> weightVector = BWtrainer->startTraining();
  vector2cur_weights(weightVector);
  for(int i = 0; i<ant_amount-1; i++){
    phase_ctrl->ant_on(ant_nums[i]);
  }

  status = TRAINING;
  status_count = training_round_max;

  std::cout<<"Init"<<std::endl;
  return weights_apply(cur_weights);
}


int Beamformer::weights_addition(int * dest_weights, int * weights0, int * weights1){
  for(int i = 0; i<ant_amount-1; i++){
    dest_weights[ant_nums[i]] = weights0[ant_nums[i]] + weights1[ant_nums[i]];
    while(dest_weights[ant_nums[i]] < 0)
      dest_weights[ant_nums[i]]+= 360;
    dest_weights[ant_nums[i]] %= 360;
  }

  return 0;
}


int Beamformer::weights_addition(int * dest_weights, int * weights){
  for(int i = 0; i<ant_amount-1; i++){
    dest_weights[ant_nums[i]] += weights[ant_nums[i]];
    while(dest_weights[ant_nums[i]] < 0)
      dest_weights[ant_nums[i]]+= 360;
    dest_weights[ant_nums[i]] %= 360;
  }
  return 0;
}


int Beamformer::weights_apply(int * weights){
  for(int i = 0; i<ant_amount-1; i++){
    phase_ctrl->phase_control(ant_nums[i], weights[ant_nums[i]]);
  }
  return 0;
}


int Beamformer::weights_apply(void){
  int * weights = cur_weights;
  for(int i = 0; i<ant_amount; i++){
    phase_ctrl->phase_control(ant_nums[i], weights[ant_nums[i]]);
  }
  return 0;
}


int Beamformer::vector2cur_weights(std::vector<int> weightVector){
  for(int i = 0; i<ant_amount-1;i++){
    cur_weights[ant_nums[i]] = weightVector[i];
  }
  curWeightVector = weightVector;
  return 0;
}



void Beamformer::weights_printing(int * weights){
  for(int i = 0; i<ant_amount; i++){
    std::cout<<"ANT num : "<<ant_nums[i]<<std::endl;
    std::cout<<"Phase : "<<weights[ant_nums[i]]<<std::endl<<std::endl;
  }
}


int Beamformer::start_beamformer(void){
  if(init_beamformer())
    std::cerr << "Beamformer init failed"<<std::endl;
  std::cout << "Beamformer init finished"<<std::endl;
  if(ipc.wait_sync())
    return -1;

  return run_beamformer();
}


int Beamformer::SIC_port_measure(void){
  //We must measure SIC port before we start.
  for(int i = 0; i<16; i++){
    phase_ctrl->ant_off(i);
  }
  cur_weights[SIC_PORT_NUM_] = 0;
  phase_ctrl->ant_on(SIC_PORT_NUM_);
  phase_ctrl->phase_control(SIC_PORT_NUM_, SIC_REF_POWER, 0);

  std::cout << "SIC Phase Set"<<std::endl;
  return 0;
}



int Beamformer::SIC_port_measure_over(void){
  //We must measure SIC port before we start.
  for(int i = 0; i<ant_amount-1; i++){
    phase_ctrl->ant_on(ant_nums[i]);
  }

  weights_apply(cur_weights);

  std::cout << "SIC over"<<std::endl;
  return 0;
}

int Beamformer::SIC_handler(const struct average_corr_data & data){
  sic_ctrl->setCurrentAmp(std::complex<float>(data.cw_i, data.cw_q));
  cur_weights[SIC_PORT_NUM_] = sic_ctrl->getPhase();   //get SIC phase
  phase_ctrl->phase_control(SIC_PORT_NUM_, sic_ctrl->getPower(), cur_weights[SIC_PORT_NUM_]); //change phase and power
  sic_ctrl->setPower(idx2dB(dB2idx(sic_ctrl->getPower())));
  sic_ctrl->setPhase(cur_weights[SIC_PORT_NUM_]);

  needSIC = false;
  return 0;
}

int Beamformer::SIC_adjustment(void)
{
  //needSIC = false;
  sic_ctrl->setPower(sic_ctrl->getPower() + 0.1);
  phase_ctrl->phase_control(SIC_PORT_NUM_, sic_ctrl->getPower(), sic_ctrl->getPhase());

  return 0;
}



int Beamformer::Signal_handler(const struct average_corr_data & data){
  uint16_t tag_id = 0;
  std::vector<int> weightVector;

  /*************************Add algorithm here***************************/
  if(perfect_flag)
    logger.dataLogging(cur_weights, data, sic_ctrl->getPower(), counter, beamforming_count, true, 99);
  else
    logger.dataLogging(cur_weights, data, sic_ctrl->getPower(), counter, beamforming_count, BWtrainer->isOptimalUsed(), BWtrainer->which_optimal());

  counter++;

  if(status == TRAINING)
  {
    if(data.successFlag == _SUCCESS)
    {
      if(data.data_flag == 0)
      {
        for(int i = 0; i<16; i++)
        {
          tag_id = tag_id << 1;
          tag_id += data.data_bits[i];
        }
      }else
      {
        std::cerr << "Why not receive RN16 at TRAINING section?"<<std::endl;
        assert(data.data_flag == 0);
      }
      printf("Got RN16 : %x\n",tag_id);
      printf("avg corr : %f\n",data.avg_corr);
      printf("avg iq : %f, %f\n",data.avg_i, data.avg_q);
      printf("avg amp : %f, %f\n\n",data.cw_i, data.cw_q);

      //if(tag_id == PREDEFINED_RN16_)
      if(true)
      {
        BWtrainer->getRespond(data);
        weight_amp_stack.push_back(phase_amp_dataset(curWeightVector, data));
      }
      else
      {
        BWtrainer->cannotGetRespond();
      }
    }else{
      printf("Couldn't get RN16\n\n");
      BWtrainer->cannotGetRespond();
    }
    needSIC = true;
  }

  if(status_count > 1)  //yet we still need to maintain current status
  {
    status_count -= 1;

    if(status == TRAINING)
    {
      weightVector = BWtrainer->getTrainingPhaseVector();
      needSIC = true;
    }else if(status == BEAMFORMING)
    {
      if(opt_repeat_counter < (REPEAT_COUNT_-1))
      {
        opt_repeat_counter += 1;
        needSIC = false;
      }else
      {      
        if(perfect_flag)
        {
          weightVector = perfectVector;
          needSIC = false;
          //needSIC = true;
        }else
        {
          weightVector = BWtrainer->getOptimalPhaseVector();
          needSIC = true;
        }
        opt_repeat_counter = 0;
      }
    }
  }else //switch status triggered
  {
    needSIC = true;
    if(status == BEAMFORMING)
    {
      beamforming_count += 1;
      status = TRAINING;
      status_count = training_round_max;
      BWtrainer->startTraining();
      sic_ctrl->setTargetPower(std::complex<float>(0.01, 0.0));

      weightVector = BWtrainer->getTrainingPhaseVector();
    }else if(status == TRAINING)
    { 
      if(data.round >= THRESHOLD_FOR_PERFECT_)
      {
        /***************** Perfect training phase start ******************/
        if(perfect_flag || weight_amp_stack.size() >= 50)
        {
          if(perfectVector.size() == 0)
          {
            std::vector<int> idx_cont(weight_amp_stack.size());

            for(int i = 0; i < idx_cont.size(); i++)
              idx_cont[i] = i;

            std::random_shuffle(idx_cont.begin(), idx_cont.end());

            CA_calculator ca_cal(ant_amount-1);

            for(int i = 0; i < weight_amp_stack.size(); i++)
            {
              ca_cal.setNewData(weight_amp_stack[idx_cont[i]].phase, weight_amp_stack[idx_cont[i]].amp);
            }
            perfectVector = ca_cal.processOptimalVector();
            perfect_flag = true;

          }
          status = BEAMFORMING;
          status_count = 0xFFFF;  //write max
          sic_ctrl->setTargetPower(std::complex<float>(0.01, 0.0));
          weightVector = perfectVector;
        }else
        {
          /***************** Not enough training data ******************/
          beamforming_count += 1;
          status = TRAINING;
          status_count = training_round_max;
          BWtrainer->startTraining();
          sic_ctrl->setTargetPower(std::complex<float>(0.01, 0.0));

          weightVector = BWtrainer->getTrainingPhaseVector();
        }
      }else
      {
        if(BWtrainer->isOptimalCalculated())
        {
          /***************** Start Optimal phase ******************/
          status = BEAMFORMING;
          status_count = BEAMFORMING_ROUND;
          sic_ctrl->setTargetPower(std::complex<float>(0.01, 0.0));
          weightVector = BWtrainer->getOptimalPhaseVector();
          opt_repeat_counter = 0;
        }else
        {
          /***************** Back to Training stage ******************/
          beamforming_count += 1;
          status = TRAINING;
          status_count = training_round_max;
          BWtrainer->startTraining();
          weightVector = BWtrainer->getTrainingPhaseVector();
        }
      }
    }
  }

  if(weightVector.size() != 0)
    vector2cur_weights(weightVector);

  if(weights_apply(cur_weights)){
    std::cerr<<"weight apply failed"<<std::endl;
    return 1;
  }

  if(needSIC)
  {
    sic_ctrl->setPower(-22);
    sic_ctrl->setPhase(SIC_REF_PHASE);
    phase_ctrl->phase_control(SIC_PORT_NUM_, -22, SIC_REF_PHASE);
  }
  /*****************************************************************/

  return 0;
}


Beamformer::phase_amp_dataset::phase_amp_dataset(std::vector<int> phase_, average_corr_data data_)
  : phase(phase_), amp(data_.avg_i, data_.avg_q)
{
}

Beamformer::phase_amp_dataset::phase_amp_dataset(std::vector<int> phase_, std::complex<float> amp_)
  : phase(phase_), amp(amp_)
{
}
