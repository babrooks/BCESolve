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

// Heade file for Duopoly Game
// AH 7-17-2019

#ifndef DUO_HPP
#define DUO_HPP

#include "bceabstractgame.hpp"
#include "bce.hpp"

class Duo : public BCEAbstractGame
{
private:
  int numprices;
  double entryCost;
  double reservePrice;
  double highbid;
  double lowbid;
  double alpha;
  double minAngleIncrement;

public:
  BCEDistrArray distribution;

  Duo(int np, int nt, double _alpha,
	     double _highbid,double _reservePrice,double _entryCost,double _mai)
    : BCEAbstractGame(3,np,2,3), alpha(_alpha), numprices(nt),
      entryCost(_entryCost), highbid(_highbid),
      lowbid(0.0), reservePrice(_reservePrice), minAngleIncrement(_mai)
  {
    distribution.push_back(new uniform(),1.0);
  }

  int getNumValues()const {return numprices;}


  double prior (int state, const vector<int> &types) const
  {
      if (types[0] == 1 && types[1] == 0 && state == 0)
      {
          return 1.0 / 3.0;
      }
      else if (types[0] == 0 && types[1] == 1 && state == 1)
      {
          return 1.0 / 3.0;
      }
      else if (types[0] == 1 && types[1] == 1 && state == 2)
      {
          return 1.0 / 3.0;
      }
      else
      {
         return 0.0;
      }

  }

  double objective(int state, const vector<int> &actions, int objectiveIndex) const
  {
      int player = objectiveIndex;
      if (objectiveIndex < 2)
      {
          double myprice = actions[player]/(numActions[player]-1.0);
          if (state == 2)
          {
              if (actions[player]>actions[1-player])
              {
                  return 0.0;
              }
              if (actions[player]<actions[1-player])
              {
                 return (1-myprice)*myprice;
              }
              if (actions[player]==actions[1-player])
              {
                  return (1-myprice)*myprice*0.5;
              }
          }
          else if (player == state)
          {
            return (1-myprice)*myprice;
          }
          else
          {
              return 0.0;
          }
      }

      else if (objectiveIndex==2)
      {
          if (state == 2)
          {
              int w = (actions[0]<actions[1]? 0: 1);
              return (pow((1-actions[w]/(numActions[w]-1.0)), 2.0) / 2.0);
          }
          else
          {
              return (pow((1-actions[state]/(numActions[state]-1.0)), 2.0) / 2.0);
          }
      }

      return 0.0;
  } // objective

  bool dominated(int a, int t, int player) const
  {
    if (t == 0 && a > 0)
    {
        return true;
    }
    return false;
  }
};

#endif
