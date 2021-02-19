#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "Calibrator/Calibration.hpp"

int main(int argc, char ** argv)
{
  int ant_num;
  int resolution;
  std::string filename;

  //get Antenna number from parameters
  if(argc >= 4)
  {
    ant_num = atoi(argv[1]);
    resolution = atoi(argv[2]);
    filename = argv[3];
  }else
  {
    //default
    ant_num = 0;
  }

  Calibration cal(ant_num, resolution);
  std::cout << "Ready!"<<std::endl;

  cal.process();
  cal.save_csv_file(filename);
  //cal.test();

  return 0;
}
