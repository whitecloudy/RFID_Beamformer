#ifndef __BEAM_ALGORITHM__
#define __BEAM_ALGORITHM__

#include "Beamtrainer.hpp"
#include <string>



namespace BEAM_ALGO
{
  enum algorithm
  {
    NO_ALGO_SELECTED,
    FIXED_BEAM,
    DIRECTIONAL_BEAM,
    CA_WITH_DIRECTIONAL,
    CA_WITH_RANDOM,
    CA_KALMANED_WITH_RANDOM,
    RANDOM_BEAM,
    AGENT
  };

  Beamtrainer * get_beam_class(int ant_num, algorithm algo);

  algorithm parse_beam_algorithm(std::string input);

}


#endif
