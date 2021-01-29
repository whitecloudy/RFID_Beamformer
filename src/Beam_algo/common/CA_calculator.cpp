#include "CA_calculator.hpp"


CA_calculator::CA_calculator(int ant_num) : ant_num(ant_num)
{
  clear();
}

int CA_calculator::setNewTrainingVector(std::vector<int> trainingVector)
{
  arma::Row<std::complex<double>> beamWeight(ant_num);

  for(int i = 0; i<ant_num; i++)
  {
    beamWeight(i) = beam_util::phase2NormalComplex(trainingVector[i]);
  }
  
  if(trainingWeightMatrix.n_rows == ant_num)
  {
    trainingWeightMatrix.shed_row(0);
    trainingWeightMatrix.insert_rows(trainingWeightMatrix.n_rows, beamWeight);
  }else if(trainingWeightMatrix.n_rows < ant_num)
  {
    trainingWeightMatrix.insert_rows(trainingWeightMatrix.n_rows, beamWeight);
  }else
  {
    std::cerr << "Training Weight Matrix Error"<<std::endl;
    exit(1);
  }

  return trainingWeightMatrix.n_rows;
}


int CA_calculator::setNewCorrData(std::complex<double> corrData)
{
  std::cout << "setNew corr data"<<std::endl;
  arma::Row<std::complex<double>> data(1);
  data(0) = corrData;

  if(avgCorrColumn.n_rows == ant_num)
  {
    avgCorrColumn.shed_row(0);
    avgCorrColumn.insert_rows(avgCorrColumn.n_rows, data);
  }else if(avgCorrColumn.n_rows < ant_num)
  {
    avgCorrColumn.insert_rows(avgCorrColumn.n_rows, data);
  }else
  {
    std::cerr << "Average Correaltion Weight Matrix Error"<<std::endl;
    exit(1);
  }

  return avgCorrColumn.n_rows;
}


int CA_calculator::resetTrainingVector(std::vector<int> trainingVector)
{
  arma::Row<std::complex<double>> beamWeight(ant_num);

  for(int i = 0; i<ant_num; i++)
  {
    beamWeight(i) = beam_util::phase2NormalComplex(trainingVector[i]);
  }

  if(trainingWeightMatrix.n_rows <= 0)
  {
    std::cerr << "Tried to reset empty Training Vector"<< std::endl;
    exit(1);
  }else
  {
    trainingWeightMatrix.shed_row(trainingWeightMatrix.n_rows - 1);
    trainingWeightMatrix.insert_rows(trainingWeightMatrix.n_rows, beamWeight);
  }

  return trainingWeightMatrix.n_rows;
}


int CA_calculator::resetCorrData(std::complex<double> corrData)
{
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
    arma::Col<std::complex<double>> channelMatrix = arma::inv(trainingWeightMatrix) * avgCorrColumn;

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
  return (trainingWeightMatrix.is_square() && (trainingWeightMatrix.n_rows == avgCorrColumn.n_rows));
}


bool CA_calculator::clear(void)
{
  trainingWeightMatrix.reset();
  avgCorrColumn.reset();

  return false;
}


