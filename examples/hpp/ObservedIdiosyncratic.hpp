// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
//
// BCESolve free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// BCESolve is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
// Benjamin A. Brooks
// ben@benjaminbrooks.net
// Chicago, IL

// Header file for Observed Idiosyncratic
// AH 7-17-2019

#ifndef OI_HPP
#define OI_HPP

#include "fpagame.hpp"

class OI : public FPAGame
{
private:

public:

  OI() {};

  OI(int na, int nv, double _entryCost, double _reservePrice,
	   double _highbid, bool exAnteFee)
    : FPAGame(na,nv,_entryCost,_reservePrice,_highbid, exAnteFee)
  {
    numTypes = vector<int>(2,2*nv-1);
    //numTypes = vector<int>(2,nv);
    setHasProductStructureData(true);
    setNumPrivateStates(numTypes);
  };

  double prior (int state, const vector<int> &types) const
  {
    // Use a uniform prior
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    if (types[0]==(values[0] - values[1] + numValues - 1 ) && types[1]==(values[1] - values[0] + numValues - 1))
      {
  	return FPAGame::prior(state,types);
      }
    else
      return 0.0;
  }

  bool dominated(int a, int t, int player) const
  {
    //return (lowbid + (highbid-lowbid)*(a-1.0)/(numActions[player]-2.0)
	  //  > (1.0*t)/(numValues-1.0));


     return (false);
  }

};

#endif
