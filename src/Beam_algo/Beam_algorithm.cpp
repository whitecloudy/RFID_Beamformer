#include "Beam_algo/Beam_algorithm.hpp"
#include <iostream>

#include "Beam_algo/Random_beamtrainer.hpp"
#include "Beam_algo/Directional_beamtrainer.hpp"
#include "Beam_algo/CA_with_directional_beamtrainer.hpp"

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
      case DIRECTIONAL_BEAM:
        class_ptr = new Directional_beamtrainer(ant_num);
        break;
      case CA_WITH_DIRECTIONAL:
        class_ptr = new CA_with_directional_beamtrainer(ant_num);
        break;
      case FIXED_BEAM:
        //TBD
      default:
        std::cerr << "Warning: No Algorithm selected" << std::endl;
        class_ptr = NULL;
        break;
    }

    return class_ptr;
  }


  algorithm parse_beam_algorithm(std::string input)
  {
    algorithm algo;
    if(!input.compare("random"))                    algo = RANDOM_BEAM;
    else if(!input.compare("directional"))          algo = DIRECTIONAL_BEAM;
    else if(!input.compare("fixed"))                algo = FIXED_BEAM;
    else if(!input.compare("ca_with_directional"))  algo = CA_WITH_DIRECTIONAL;
    else{
      std::cerr<<"Error : No such algorithm"<<std::endl;
      exit(1);
    }

    return algo;
  }
}


