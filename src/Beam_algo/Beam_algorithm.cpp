#include "Beam_algo/Beam_algorithm.hpp"

namespace BEAM_ALGO
{

  Beamtrainer * get_beam_class(int ant_num, algorithm algo)
  {
    Beamtrainer * class_ptr;

    switch(algo)
    {
      case RANDOM_BEAM:
        class_ptr = new Random_beamtrainer(ant_num);
        break;
      case FIXED_BEAM:
        //TBD
        break;
      default:
        //TBD
        break;
    }

    return class_ptr;
  }


  algorithm parse_beam_algorithm(std::string input)
  {
    algorithm algo;
    if(input.compare("random"))
      algo = RANDOM_BEAM;
    else if(input.compare("fixed"))
      algo = FIXED_BEAM;
    else{
      std::cerr<<"Error : No such algorithm"<<std::endl;
      exit(1);
    }
  }
}


