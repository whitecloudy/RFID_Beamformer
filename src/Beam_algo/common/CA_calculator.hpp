#ifndef _CA_CALCULATOR_
#define _CA_CALCULATOR_

#include <iostream>
#include <armadillo>
#include "Beam_util.hpp"


class CA_calculator{
  private:
    int ant_num;

    bool First = true;

    arma::Mat<std::complex<double>> W_Mat;
    arma::Col<std::complex<double>> avgCorrColumn;
    arma::Col<std::complex<double>> channelMatrix;

  private:
    arma::cx_mat getInvMat(arma::cx_mat Mat);

  public:
    CA_calculator(int ant_num, int ML_Ratio = 1);

    int setNewData(std::vector<int> trainingVector, std::complex<double> corrData);

    int setNewTrainingVector(std::vector<int> trainingVector);
    int setNewCorrData(std::complex<double> corrData);

    int resetTrainingVector(std::vector<int> trainingVector);
    int resetCorrData(std::complex<double> corrData);

    bool is_processable(void);
    bool clear(void);
    std::vector<int> processOptimalVector(void);
};

#endif
