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

// Header file for Partnership Dissolution
// AH 7-17-2019

#ifndef ASYMETRICDOUBLEAUCTION_HPP
#define ASYMETRICDOUBLEAUCTION_HPP

#include "bceabstractgame.hpp"
#include "bce.hpp"

class AsymetricDoubleAuction : public BCEAbstractGame
{
private:
  int numValues;
  double entryCost;
  double reservePrice;
  double highbid;
  double lowbid;
  double alpha;
  double share;

public:
  BCEDistrArray distribution;

  AsymetricDoubleAuction() {distribution.push_back(new vToTheAlpha(1.0),1.0);}

  AsymetricDoubleAuction(int na, int nv, double _alpha,
	     double _highbid,double _reservePrice,double _entryCost, double share)
    : BCEAbstractGame(nv*nv,na,1,3), alpha(_alpha), numValues(nv),
      entryCost(_entryCost), highbid(_highbid),
      lowbid(0.0), reservePrice(_reservePrice), share(share)
  {
    distribution.push_back(new vToTheAlpha(1.0),1.0);
    numTypes = vector<int>(2,nv);
    setHasProductStructureData(true);
    setNumPrivateStates(numTypes);
    //setNumPrivateStates(vector<int>(2,nv));
  }

  int getNumValues(){return numValues;} const

  int stateToPrivateValues(int state, vector<int> &values) const
  {
    values.resize(2);

    values[0]=state%numValues;
    state-=values[0];
    values[1]=state/numValues;
  }

  double prior (int state, const vector<int> &types) const
  {
    vector<int> values(2,0);
    stateToPrivateValues(state,values);

    if (values[0]==types[0] && values[1]==types[1])
    {
        vector<int> values;
        stateToPrivateValues(state,values);
        double v0 = static_cast<double>(values[0]+1)/numValues;
        double v1 = static_cast<double>(values[1]+1)/numValues;
        double incr = 1.0/numValues;
        return distribution.PDF(v0,v1,incr);
    }
    else
    {
        return 0.0;
    }
  }

  double objective(int state, const vector<int> &actions, int objectiveIndex) const
  {
    // Convert the state into a pair of valuations
      vector<int> values(2,0);
      stateToPrivateValues(state,values);

      if (objectiveIndex < 2)
      {
          int player = objectiveIndex;
          double mybid = (highbid)*actions[player]/(numActions[player]-1.0);
          double otherbid = (highbid)*actions[1-player]/(numActions[1-player]-1.0);
          double val = (1.0*values[player]/(numValues-1.0));
          double myshare = (objectiveIndex==0? share: (1.0-share));

          if (actions[player]>actions[1-player])
          {
              return val - (1-myshare)*((alpha)*mybid + (1-alpha)*otherbid);
          }
          else if (actions[player]<actions[1-player])
          {
              return (myshare)*((alpha)*otherbid + (1-alpha)*mybid);
          }
          else if (actions[player]==actions[1-player])
          {
              return 0.5*(val - (1-2*myshare)*((alpha)*mybid + (1-alpha)*otherbid));
          }
      }

      else if (objectiveIndex==2)
      {
          if (actions[0]==actions[1])
          {
              return ((1.0*values[0]/(numValues-1.0)) + (1.0*values[1]/(numValues-1.0))) / 2.0;
          }
          else
          {
              int w = (actions[0]>actions[1]? 0: 1);
              return (1.0*values[w]/(numValues-1.0));
          }
      }

      return 0.0;
  } // objective

  // bool dominated(int a, int t, int player) const
  // {
  //   return false;
  // }
};

#endif
