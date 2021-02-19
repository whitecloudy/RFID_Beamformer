#ifndef __CALIBRATION__
#define __CALIBRATION__

#include <lib/AD5371_ctrl/Vout_controller.hpp>
#include "VNA_comm.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <complex>

class Calibration
{
  private:
    bool processed = false;
    Vout_controller V;
    int client_fd;
    VNA_communicator comm;
    int ant_num, resolution;

  private:
    std::unique_ptr<std::complex<double>[]> phase_measured_data;
    std::unique_ptr<std::complex<double>[]> attenuator_measured_data;



  private:
    std::complex<double> decodeData(uint8_t dataBuffer[]);
    std::complex<double> getMeasureData(void);

  public:
    Calibration(int ant_num, int resolution);
    void test();
    int process();
    int save_csv_file(std::string filename);

};

#endif
