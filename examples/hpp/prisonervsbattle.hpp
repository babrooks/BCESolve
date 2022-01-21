// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2022 Benjamin A. Brooks
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

// Default game for bceviewer
// BAB 6-23-2016
//! @example

#ifndef PRISONERVSBATTLE_HPP
#define PRISONERVSBATTLE_HPP

#include "bce.hpp"
#include "bceabstractgame.hpp"

class PrisonerVsBattle : public BCEAbstractGame
{
public:
  
  PrisonerVsBattle()
    : BCEAbstractGame(2,2,1,2)
  {}

  double prior (int state, const vector<int> &types) const
  { return 0.5; }

  double objective(int state, const vector<int> &actions, 
		   int obj) const
  {
    if (obj < 0 || obj > 1)
      return 0;
    
    if (state == 0) // Prisoner's dilemma
      {
	if (actions[0] != actions[1]) // (C,D) or (D,C)
	  {
	    if (actions[obj]==1) // player obj played D
	      return 3;
	    return -1; // player obj played C
	  }
	if (actions[0]) // (D,D)
	  return 1;
	return 2; // (C,C)
      }
    else // Battle of the sexes
      {
	if (actions[0] != actions[1]) // Coordinated
	  {
	    if (actions[obj]==1) // Coordinated on player obj's preferred
	      return 3;
	    return 1; // Coordinated on player obj's least preferred
	  }
	return 0; // Miscoordinated
      }
  } // objective
};

#endif
