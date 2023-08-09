#include "Logger.hpp"
#include <cassert>

Logger::Logger(std::vector<int> ant_nums)
{
  this->ant_amount = ant_nums.size();
  this->ant_nums = ant_nums;

  this->log.open("log.csv");
  this->optimal_log.open("log_optimal.csv");

  for(int i = 0; i<ant_amount; i++){
    this->log<<"phase "<<ant_nums[i]<<", ";
    this->optimal_log<<"phase "<<ant_nums[i]<<", ";
  }

  log<<"SIC phase, SIC power, avg corr,corr i, corr q, cw i, cw q,std i,std q ,RN16, round"<<std::endl;
  optimal_log<<"SIC phase, SIC power, avg corr,corr i, corr q, cw i, cw q,std i,std q ,RN16, round"<<std::endl;
}

Logger::~Logger()
{
    this->log.close();
    this->optimal_log.close();
}

int Logger::dataLogging(int * cur_weights, const struct average_corr_data & data, double sic_power, int counter, int beamforming_count, bool optimal, const int which_op)
{
  uint32_t tag_id = 0;

  if(data.successFlag == _SUCCESS){
    if(data.data_flag == 0) //RN16
    {
      for(int i = 0; i<16; i++)
      {
        tag_id = tag_id << 1;
        tag_id += data.data_bits[i];
      }
    }else if(data.data_flag == 1) //EPC 128bit
    {
      for(int i = 0; i<32; i++)
      {
        tag_id = tag_id << 1;
        tag_id += data.data_bits[i];
      }
    }
    else
    {
      std::cerr << "Wrong data flag"<<std::endl;
      std::cerr << (unsigned int)data.data_flag << std::endl;
      assert(data.data_flag == 0);
    }

    if(optimal)
    {
      for(int i = 0; i<ant_amount;i++){
        optimal_log<<cur_weights[ant_nums[i]]<<", ";
      }
      optimal_log<<sic_power<< ", "<<data.avg_corr<<", "<<data.avg_i<<", "<<data.avg_q<<", "<<data.cw_i<<", "<<data.cw_q<<", "<<data.stddev_i<<", "<<data.stddev_q<<", "<<tag_id<<", "<<data.round<< ", "<<which_op<<", " <<counter << ", " <<beamforming_count<<std::endl;
    }else
    {
      for(int i = 0; i<ant_amount;i++){
        log<<cur_weights[ant_nums[i]]<<", ";
      }

      log<<sic_power<< ", "<<data.avg_corr<<", "<<data.avg_i<<", "<<data.avg_q<<", "<<data.cw_i<<", "<<data.cw_q<<", "<<data.stddev_i<<", "<<data.stddev_q<<", "<<tag_id<<", "<<data.round<<", "<<counter<< ", " << beamforming_count<<std::endl;
    }
  }else if(data.successFlag == _PREAMBLE_FAIL){
    if(optimal)
    {
      for(int i = 0; i<ant_amount;i++){
        optimal_log<<cur_weights[ant_nums[i]]<<", ";
      }
      optimal_log<<sic_power<< ", "<<0.0<<", "<<0.0<<", "<<0.0<<","<<data.cw_i<<", "<<data.cw_q<<", "<<data.stddev_i<<", "<<data.stddev_q<<", "<<"-"<<","<<data.round<<", "<<which_op<<", "<<counter<< ", " << beamforming_count<<std::endl;
    }else
    {
      for(int i = 0; i<ant_amount;i++){
        log<<cur_weights[ant_nums[i]]<<", ";
      }
      log<<sic_power<< ", "<<0.0<<", "<<0.0<<", "<<0.0<<","<<data.cw_i<<", "<<data.cw_q<<", "<<data.stddev_i<<", "<<data.stddev_q<<", "<<"-"<<","<<data.round<<", "<<counter<< ", " << beamforming_count<<std::endl;
    }
  }else if(data.successFlag == _GATE_FAIL){
    if(optimal)
    {
      for(int i = 0; i<ant_amount;i++){
        optimal_log<<cur_weights[ant_nums[i]]<<", ";
      }
      optimal_log<<sic_power<< ", "<<0.0<<", "<<0.0<<", "<<0.0<<","<<data.cw_i<<", "<<data.cw_q<<", "<<data.stddev_i<<", "<<data.stddev_q<<", "<<"GATE Failed"<<","<<data.round<<", "<<counter<<std::endl;
    }else
    {
      for(int i = 0; i<ant_amount;i++){
        log<<cur_weights[ant_nums[i]]<<", ";
      }
      log<<sic_power<< ", "<<0.0<<", "<<0.0<<", "<<0.0<<","<<data.cw_i<<", "<<data.cw_q<<", "<<data.stddev_i<<", "<<data.stddev_q<<", "<<"GATE Failed"<<","<<data.round<<std::endl;
    }

  }

  return 0;
}
