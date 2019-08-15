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

#ifndef BCESOLUTION_HPP
#define BCESOLUTION_HPP

#include "bcecommon.hpp"
#include "bcegame.hpp"
#include "bceexception.hpp"
#include "bcequilibrium.hpp"
#include "bcecounter.hpp"

#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>

//! Class for storing data produced by BCESolver
/*! This class stores Bayes correlated equilibria as output by the
  BCESolver class. It also contains statistical routines for analyzing
  the list of equilibria.

  The class contains data structures for storing BCE in the
  BCESolution::equilibria member. It also stores various information about
  the BCEGame object for which the equilibrium was calculated, in the
  members BCESolution::objectives, BCESolution::prior, and
  BCESolution::dominated. These arrays correspond to the methods of the
  same name in the BCEGame class.

  BCESolution also contains methods for calculating statistics of a BCE. In
  particular, BCESolution::getConditionalMarginal is a general purpose tool
  for calculating marginal conditional distributions of the BCE,
  BCESolution::getExpectedObjectives can be used to calculate the expected
  objectives under the BCE, and BCESolution::getDeviationObjectives can be
  used to calculate counterfactual payoffs if players had used actions
  different from their recommendations.

  \ingroup src
*/
class BCESolution
{
private:
  //! The game that the equilibria correspond to
  BCEGame game;
  //! List of new equilibria that have not yet been consolidated.
  list<BCEquilibrium> newEquilibria;
  //! Consolidated vector of equilibria
  vector<BCEquilibrium> equilibria;
  //! Index of the current equilibrium in BCESolution::equilibria
  int currentEquilibrium;
  //! Matrix of weights on mapBoundary objectives.
  /*! The first row, i.e. mapBoundaryWeights[0] contains weights
    for the main objective, and the second row contains weights for
    the secondary objective. The number of columns is the number of objectives.
   */
  vector<vector<double> > mapBoundaryWeights;
  //! Vector of weights on the main objective.
  vector<double> mainObjectiveWeights;
  //! True if boundary was mapped in the solver.
  bool boundaryMapped;

public:
  //! Serialization routine
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & newEquilibria;
    ar & equilibria;
    ar & game;
    ar & currentEquilibrium;
    ar & mapBoundaryWeights;
    ar & boundaryMapped;
    ar & mainObjectiveWeights;
  }

  //! Default constructor
  BCESolution ():currentEquilibrium(0),
		 boundaryMapped(false),
		 mapBoundaryWeights(2,vector<double>(2,0)),
		 mainObjectiveWeights(2,0)
  {}

  //! Constructor
  /*! Initializes a new BCESolution object for a game with the given
      numbers of players, states, actions, types, objectives. */
  BCESolution (const BCEGame & game);

  //! Destructor
  ~BCESolution () {}

  /* Managing the equilibria. */
  //! Appends distr to the end of the list newEquilibria
  void addEquilibrium(const map<int,double> & distr, map<int,double> & multipliers);
  //void addEquilibrium(const map<int,double> & distr);
  //! Clears all equilibria
  void clearEquilibria();
  //! Sets boundaryMapped status.
  void setBoundaryMapped(const bool isMapped)
  { boundaryMapped = isMapped; }
  //! Sets mapBoundaryWeights.
  void setMapBoundaryWeights(const vector<vector<double> >& weights)
  { mapBoundaryWeights = weights; }
  //! Sets main objective weights
  void setMainObjectiveWeights(const vector<double>& weights) {
    mainObjectiveWeights = weights;
  }
  //! Consolidates equilibria
  /*! Appends the list of newEquilibria to the vector
      equilibria. Clears newEquilibria.*/
  void consolidateEquilibria();
  //! Returns the game
  const BCEGame & getGame() const { return game; }


  //! Returns the equilibrium at equilibriumIndex
  const BCEquilibrium & getEquilibrium(int equilibriumIndex) const;
  //! Set current equilibrium
  /*! Sets the current equilibrium to the equilibrium with index
    i. That vector must be an element of the vector equilibria. */
  void setCurrentEquilibrium(int equilibriumIndex);

  //! Assignment operator
  BCESolution& operator=(const BCESolution &rhs);

  /* Statistical routines */
  //! Gets the expected objectives under the current BCE
  /*!  This method will calculate the expected objectives for the
       current equilibrium, as specified by BCESolution::currentIndex. */
  void getExpectedObjectives(vector<double> &objectiveValues) const;
  //! Gets the expected objectives under all BCE
  /*!  This method will calculate the expected objectives for each of
    the equilibrium distributions in "equilibria". Optionally can be
    called with a particular given distribution, which is useful,
    e.g., for calculating objectives with a conditional
    distribution. */
  void getExpectedObjectives(vector< vector<double> > &objectiveValues) const;
  //! Calculates expected objectives for the given distribution.
  void getExpectedObjectives(vector<double> &objectiveValues,
			     const map<int,double> &distribution) const;

  //! Calculates deviation objectives
  /*! Calculates the expected payoff resulting from each action for
    the given player, type, and recommended action. Returns the
    probability of the given action. */
  double getDeviationObjectives(int player, int action, int type,
				vector< vector<double> > & objectiveValues) const;

  //! Calculates constraint multipliers
  /*! Calculates the multiplier associated with each action for
    the given player, type, and recommended action. */
  double getConstraintMultipliers(int player, int action, int type,
  					   vector< vector<double> > &values) const;

  //! Computes the marginal conditional distribution of a BCE
  /*! A general method for calculating conditional marginal
    distributions of the current BCE. The state must be equal to an
    element of stateConditions, action[i] must be an element of
    actionConditions[i], etc. If the given condition vector is empty,
    no restriction is imposed. The distribution is projected onto
    variables with a logical true in Marginal. Returns the probability
    of the event we are conditioning on. See BCECounter for a
    description of the conditioning and marginal arguments. */
  double getConditionalMarginal(const vector<int> &stateConditions,
			      const vector< vector<int> > &actionConditions,
			      const vector< vector<int> > &typeConditions,
			      bool stateMarginal,
			      const vector<bool> &actionMarginal,
			      const vector<bool> &typeMarginal,
			      vector<double> &distribution) const;

  //! Returns the mapBoundary weights
  const vector<vector<double> >& getMapBoundaryWeights() const {
    return mapBoundaryWeights;
  }

  //! Returns the main objective weights
  const vector<double> getMainObjectiveWeights() const {
    return mainObjectiveWeights;
  }

  const bool getIsBoundaryMapped() const {
    return boundaryMapped;
  }

  //! Serialize a BCESolution object using Boost.
  static void save(const BCESolution & data, const char* filename)
  {
    ofstream ofs(filename);

    if (ofs.good())
      {
	boost::archive::text_oarchive oa(ofs);
	oa << data;
	ofs.close();
      }
    else
      throw(BCEException(BCEException::FailedOpen));

  }

  //! Deserialize a BCESolution object using Boost.
  static void load(BCESolution & data, const char* filename)
  {
    ifstream ifs(filename);

    if (ifs.good())
      {
	boost::archive::text_iarchive ia(ifs);
	ia >> data;
	ifs.close();
      }
    else
      throw(BCEException(BCEException::FailedOpen));

  }

  friend class boost::serialization::access;
}; // BCESolution

#endif
