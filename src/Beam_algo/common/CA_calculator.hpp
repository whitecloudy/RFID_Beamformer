#ifndef _CA_CALCULATOR_
#define _CA_CALCULATOR_

#include <iostream>
#include <armadillo>
#include "Beam_util.hpp"

class CA_calculator{
  private:
    int ant_num;

    arma::Mat<std::complex<double>> trainingWeightMatrix;
    arma::Col<std::complex<double>> avgCorrColumn;

  public:
    CA_calculator(int ant_num);

    int setNewTrainingVector(std::vector<int> trainingVector);
    int setNewCorrData(std::complex<double> corrData);

    int resetTrainingVector(std::vector<int> trainingVector);
    int resetCorrData(std::complex<double> corrData);

    bool is_processable(void);
    bool clear(void);
    std::vector<int> processOptimalVector(void);
};

#endif
