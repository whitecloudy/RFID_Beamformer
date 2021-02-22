#include "Calibration.hpp"
#include <cmath>
#include <unistd.h>
#include <fstream>

enum VOUTNUM
{
  ANT1_phase = 23,
  ANT2_phase = 22,
  ANT3_phase = 21,
  ANT4_phase = 20,
  ANT5_phase = 19,
  ANT6_phase = 18,
  ANT7_phase = 17,
  ANT8_phase = 16,
  ANT9_phase = 7,
  ANT10_phase = 6,
  ANT11_phase = 5,
  ANT12_phase = 4,
  ANT13_phase = 3,
  ANT14_phase = 2,
  ANT15_phase = 1,
  ANT16_phase = 0,
  ANT1_attenuator = 31,
  ANT2_attenuator = 30,
  ANT3_attenuator = 29,
  ANT4_attenuator = 28,
  ANT5_attenuator = 27,
  ANT6_attenuator = 26,
  ANT7_attenuator = 25,
  ANT8_attenuator = 24,
  ANT9_attenuator = 39,
  ANT10_attenuator = 38,
  ANT11_attenuator = 37,
  ANT12_attenuator = 36,
  ANT13_attenuator = 35,
  ANT14_attenuator = 34,
  ANT15_attenuator = 33,
  ANT16_attenuator = 32
};


#define PI (3.1415926535897)

#define Deg2Rad(_num) (float)(_num * (PI / 180))

const char PHASE[] = {ANT1_phase, ANT2_phase, ANT3_phase, ANT4_phase, ANT5_phase, ANT6_phase, ANT7_phase, ANT8_phase, ANT9_phase, ANT10_phase, ANT11_phase, ANT12_phase, ANT13_phase, ANT14_phase, ANT15_phase, ANT16_phase};
const char ATTENUATOR[] = {ANT1_attenuator, ANT2_attenuator, ANT3_attenuator, ANT4_attenuator, ANT5_attenuator, ANT6_attenuator, ANT7_attenuator, ANT8_attenuator, ANT9_attenuator, ANT10_attenuator, ANT11_attenuator, ANT12_attenuator, ANT13_attenuator, ANT14_attenuator, ANT15_attenuator, ANT16_attenuator};


Calibration::Calibration(int ant_num, int resolution)
  : phase_measured_data(new std::complex<double>[resolution]),
    attenuator_measured_data(new std::complex<double>[resolution])
{
  this->ant_num = ant_num;
  this->resolution = resolution;

  client_fd = comm.wait_connection();
}

std::complex<double> Calibration::getMeasureData(void)
{
  uint8_t data = 1;
  comm.sendData(client_fd,&data, sizeof(uint8_t));

  uint8_t dataBuffer[32];
  int recv_len = comm.readData(client_fd, dataBuffer, 32);

  if(recv_len == 24)
  {
    return decodeData(dataBuffer);
  }else
  {
    std::cerr << "Measure Data Error"<<std::endl;
    exit(1);
  }
}

std::complex<double> Calibration::decodeData(uint8_t dataBuffer[])
{
  int64_t real_significand = 0;
  int32_t real_exponent = 0;
  int64_t imag_significand = 0;
  int32_t imag_exponent = 0;

  memcpy(&real_significand, dataBuffer, 8);
  memcpy(&real_exponent, dataBuffer + 8, 4);
  memcpy(&imag_significand, dataBuffer + 12, 8);
  memcpy(&imag_exponent, dataBuffer + 20, 4);

  double real = real_significand / 1.0e15 * exp2(real_exponent);
  double imag = imag_significand / 1.0e15 * exp2(imag_exponent);

  std::complex<double> recv_data(real, imag);

  return recv_data;
}


void Calibration::test(void)
{
  uint8_t dataBuffer[32];
  uint8_t data = 1;
  comm.sendData(client_fd, &data, sizeof(uint8_t));
  int recv_len = comm.readData(client_fd, dataBuffer, 32);

  std::cout <<"We received : " << decodeData(dataBuffer)<<std::endl;

}


int Calibration::process(void)
{
  processed = true;
  int attenuator_num = ATTENUATOR[ant_num];
  int phase_num = PHASE[ant_num];

  std::cout << "antenna port "<<ant_num << " Calibration start"<< std::endl;

  //set default attenuator voltage
  V.voltage_modify(attenuator_num, 10.0);
  V.voltage_modify(phase_num, 0.0);
  V.data_apply();
  usleep(1e5);
  std::complex<double> reference_data = getMeasureData();

  for(int step = 0; step < resolution; step++)
  {
    float voltage = 10.0/resolution * step;
    V.voltage_modify(phase_num, voltage);
    V.data_apply(); 
    usleep(1e5);
    std::complex<double> measured_data = getMeasureData();
    phase_measured_data[step] = measured_data;
    std::cout <<voltage << " : "<<measured_data<<std::endl;
  }

  V.voltage_modify(attenuator_num, 10.0);
  V.voltage_modify(phase_num, 0.0);
  V.data_apply();
  usleep(1e5);
  reference_data = getMeasureData();

  for(int step = 0; step < resolution; step++)
  {
    float voltage = 10.0/resolution * step;
    V.voltage_modify(attenuator_num, voltage);
    V.data_apply(); 
    usleep(1e5);
    std::complex<double> measured_data = getMeasureData();
    attenuator_measured_data[step] = measured_data;
    std::cout <<voltage << " : "<<measured_data<<std::endl;
  }

  return 0;
}

int Calibration::save_csv_file(std::string filename)
{
  if(processed)
  {
    std::ofstream csvFile(filename);

    csvFile << "Phase(V), Attenuator(V), real, imag" << std::endl;

    for(int i = 0; i < resolution; i++)
    {
      double volt = 10.0/resolution * i;
      csvFile << volt << ", 10, " <<phase_measured_data[i].real() << ", "<<phase_measured_data[i].imag() << std::endl;

    }

    for(int i = 0; i < resolution; i++)
    {
      double volt = 10.0/resolution * i;
      csvFile << "0.0, "<< volt << ", " <<attenuator_measured_data[i].real() << ", "<<attenuator_measured_data[i].imag() << std::endl;
    }

    csvFile.close();
  }
  else
  {
    std::cerr << "save csv before process" <<std::endl;
    exit(1);
  }
}
