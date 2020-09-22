#ifndef __BEAM_ALGORITHM__
#define __BEAM_ALGORITHM__

#include "Beam_algo/Beamtrainer.hpp"



namespace BEAM_ALGO
{
  enum algorithm
  {
    NO_ALGO_SELECTED,
    FIXED_BEAM,
    DIRECTIONAL_BEAM,
    CA_WITH_DIRECTIONAL,
    RANDOM_BEAM
  };

  Beamtrainer * get_beam_class(int ant_num, algorithm algo);

  algorithm parse_beam_algorithm(std::string input);
}


#endif
