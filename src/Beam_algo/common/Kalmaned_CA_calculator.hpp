#ifndef _KALMANED_CA_CALCULATOR_
#define _KALMANED_CA_CALCULATOR_

#include <iostream>
#include <armadillo>
#include "Beam_util.hpp"
#include <lib/kalman_filter/Kalman_filter.hpp>

class Kalmaned_CA_calculator{
  private:
    int ant_num;
    Kalman_filter * filter = NULL;

    arma::Mat<std::complex<double>> trainingWeightMatrix;
    arma::Col<std::complex<double>> avgCorrColumn;

  public:
    Kalmaned_CA_calculator(int ant_num);

    int setNewTrainingVector(std::vector<int> trainingVector);
    int setNewCorrData(std::complex<double> corrData);

    int resetTrainingVector(std::vector<int> trainingVector);
    int resetCorrData(std::complex<double> corrData);

    bool is_processable(void);
    bool clear(void);
    std::vector<int> processOptimalVector(void);
};

#endif
