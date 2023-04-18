#include "CA_calculator.hpp"
#include <cassert>

#define GAIN (1)


CA_calculator::CA_calculator(int ant_num, int ML_Ratio) : ant_num(ant_num)
{
  clear();
}

int CA_calculator::setNewData(std::vector<int> trainingVector, std::complex<double> corrData)
{
  int W_num = setNewTrainingVector(trainingVector);
  int a_num = setNewCorrData(corrData);

  if(W_num != a_num)
  {
    std::cerr << "W matrix and a matrix is not match!" << std::endl;
    assert(W_num == a_num);
  }

  return W_num;
}

int CA_calculator::setNewTrainingVector(std::vector<int> trainingVector)
{
  std::cout << "setNew Training : "<< W_Mat.n_rows << ", " << avgCorrColumn.n_rows<<std::endl;
  
  arma::Row<std::complex<double>> beamWeight(ant_num);
  
  for(int i = 0; i<ant_num; i++)
  {
    beamWeight(i) = beam_util::phase2NormalComplex(trainingVector[i]);
  }

  W_Mat.insert_rows(W_Mat.n_rows, beamWeight);

  return W_Mat.n_rows;
}


int CA_calculator::setNewCorrData(std::complex<double> corrData)
{
  std::cout << "setNew corr data : "<< W_Mat.n_rows << ", " << avgCorrColumn.n_rows<<std::endl;
  arma::Row<std::complex<double>> data(1);
  data(0) = corrData;

  avgCorrColumn.insert_rows(avgCorrColumn.n_rows, data);

  return avgCorrColumn.n_rows;
}


int CA_calculator::resetTrainingVector(std::vector<int> trainingVector)
{
  std::cout << "reset Training : "<< W_Mat.n_rows << ", " << avgCorrColumn.n_rows<<std::endl;

  if(W_Mat.n_rows <= 0)
  {
    std::cerr << "Tried to reset empty Training Vector"<< std::endl;
    exit(1);
  }

  arma::Row<std::complex<double>> beamWeight(ant_num);

  for(int i = 0; i<ant_num; i++)
  {
    beamWeight(i) = beam_util::phase2NormalComplex(trainingVector[i]);
  }

  for(int i = 0; i<W_Mat.n_rows ; i++)
  {
    if(arma::approx_equal(W_Mat.row(i), beamWeight, "reldiff",0.01))
    {
      W_Mat.shed_row(i);
      avgCorrColumn.shed_row(i);
    }
  }

  W_Mat.shed_row(W_Mat.n_rows - 1);
  W_Mat.insert_rows(W_Mat.n_rows, beamWeight);

  return W_Mat.n_rows;
}


int CA_calculator::resetCorrData(std::complex<double> corrData)
{
  std::cout << "reset corr : "<< W_Mat.n_rows << ", " << avgCorrColumn.n_rows<<std::endl;

  arma::Row<std::complex<double>> data(1);
  data(0) = corrData;

  if(avgCorrColumn.n_rows <= 0)
  {
    std::cerr << "Tried to reset empty column"<< std::endl;
    exit(1);
  }else
  {
    avgCorrColumn.shed_row(avgCorrColumn.n_rows - 1);
    avgCorrColumn.insert_rows(avgCorrColumn.n_rows, data);
  }

  return avgCorrColumn.n_rows;
}

std::vector<int> CA_calculator::processOptimalVector(void)
{
  std::vector<int> phaseVector(ant_num);

  //Check current state is processable
  if(is_processable())
  {
    if(First)
    {
      channelMatrix = getInvMat(W_Mat) * avgCorrColumn;
      First = false;
    }else{
      arma::Col<std::complex<double>> curChaMat = getInvMat(W_Mat) * avgCorrColumn;
      channelMatrix = (1 - GAIN)*channelMatrix + GAIN * curChaMat;
    }


    for (int i = 0; i < ant_num; i++){
      phaseVector[i] = beam_util::complex2Phase(std::conj(channelMatrix(i)));
    }

  }else
  {
    std::cerr << "Tried to calculate channel with not enough matrix"<< std::endl;
    exit(1);
  }

  return phaseVector;
}

bool CA_calculator::is_processable(void)
{
  return ((W_Mat.n_rows == avgCorrColumn.n_rows) && (arma::rank(W_Mat) >= ant_num)); 
}


bool CA_calculator::clear(void)
{
  W_Mat.reset();
  avgCorrColumn.reset();

  return false;
}

arma::cx_mat CA_calculator::getInvMat(arma::cx_mat Mat)
{
  return ((Mat.t() * Mat).i() * Mat.t());
}
