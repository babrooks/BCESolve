#ifndef BCEDATA_HPP
#define BCEDATA_HPP

#include "bcecommon.hpp"
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
  BCEData::equilibria member. It also stores various information about
  the BCEGame object for which the equilibrium was calculated, in the
  members BCEData::objectives, BCEData::prior, and
  BCEData::dominated. These arrays correspond to the methods of the
  same name in the BCEGame class.

  BCEData also contains methods for calculating statistics of a BCE. In
  particular, BCEData::getConditionalMarginal is a general purpose tool
  for calculating marginal conditional distributions of the BCE,
  BCEData::getExpectedObjectives can be used to calculate the expected
  objectives under the BCE, and BCEData::getDeviationObjectives can be
  used to calculate counterfactual payoffs if players had used actions
  different from their recommendations.

  \ingroup src
*/
class BCEData
{
public:
  //! The number of players, always 2.
  int numPlayers;
  //! Number of states.
  int numStates;
  //! Number of types for each player.
  vector<int> numTypes;
  //! Number of actions for each player.
  vector<int> numActions;
  //! Number of objective functions
  int numObjectives;

  //! Indicates if model is private values.
  /*! If true, the model is "private values", in the sense that the
      state has a product structure, and each player's payoff only
      depends on their coordinate. */
  bool isPrivateValues;
  //! Number of values for each player.
  /*! If the model is private values, this vector indicates how many
      private values each player has. Should be that the product of
      the numbers of private values is equal to the total numer of
      states. */
  vector<int> numValues;

  //! Optional names for each objective. 
  /*! Not currently used? */
  vector<string> objectiveNames;

  //! Total number of action profiles
  int numActions_total;
  //! Total number of type profiles
  int numTypes_total;
  //! Total number of actions and types per player.
  /*! Does this assume model is symmetric? */
  int numActionsTypesPerPlayer_total;
  //! Total number of action and type profiles
  int numActionsTypes_total;

  //! List of new equilibria that have not yet been consolidated.
  list<BCEquilibrium> newEquilibria;
  //! Consolidated vector of equilibria
  vector<BCEquilibrium> equilibria;

  //! Index of the current equilibrium in BCEData::equilibria
  int currentEquilibrium;

  //! Objectives
  /*! objectives[objectiveCounter][stateActionIndex] gives the
      objective evaluated at the action profile and state indicated by
      the index. See BCECounter and BCEGame::objective. */
  vector< vector<double> > objectives; 
  //! Prior distribution of state and types.
  /*! prior[stateTypeIndex] gives the prior probability of the state
      and type profile indicated by the index. See BCECounter and
      BCEGame::prior. */
  vector<double> prior;
  //! Indicates if a given action/type profile is dominated.
  /*! Should there not be separate dominated lists for each player? */
  vector<bool> dominated;

  //! Converts states/types/actions to a linear index
  /*! Converts multivariate indices over states, types, and actions,
      to a linear index. The index will only aggregate up those
      variables for which the corresponding marginal is true. Compare
      to BCECounter. */
  int stateTypesActionsToMarginalIndex(int state, 
  				       const vector<int> &types, 
  				       const vector<int> &actions,
  				       bool stateMarginal,
  				       const vector<bool> &actionMarginal,
  				       const vector<bool> &typeMarginal) const;
  //! Counts the total number of action/type profiles
  void countActionsTypes();

  //! Serialization routine
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & numPlayers;
    ar & numStates;
    ar & numTypes;
    ar & numActions;
    ar & numObjectives;

    ar & isPrivateValues;
    ar & numValues;

    ar & objectiveNames;

    ar & numActions_total;
    ar & numTypes_total;
    ar & numActionsTypesPerPlayer_total;
    ar & numActionsTypes_total;

    ar & newEquilibria;
    ar & equilibria;

    ar & currentEquilibrium;

    // Stores objective information. Syntax is
    // objectives[objectiveCounter][actionCounter].
    ar & objectives; 
    ar & prior;
    ar & dominated;
  }

public:
  //! Default constructor
  BCEData ():currentEquilibrium(0), isPrivateValues(false) {}

  //! Constructor
  /*! Initializes a new BCEData object for a game with the given
      numbers of players, states, actions, types, objectives. */
  BCEData (int numPlayersArg,
	   int numStatesArg, 
	   const vector<int> & numActionsArg, 
	   const vector<int> & numTypesArg,
	   int numObjectivesArg);

  //! Destructor
  ~BCEData () {}

  /*! We do not do this in the constructor, since BCESolver has a
    BCEData object that gets initialized in the constructor, and we
    cannot simply pass the virtual methods for objectives, prior, and
    dominated to the BCEData constructor. */
  void setObjectivesPriorDominated(const vector< vector<double> > & objectivesArg,
				   const vector<double> & priorArg,
				   const vector<bool> & dominatedArg);

  //! Set the names of the objectives
  void setObjectiveNames(const vector<string> & _names);

  /* Managing the equilibria. */
  //! Appends distr to the end of the list newEquilibria
  void addEquilibrium(const map<int,double> & distr);
  //! Clears all equilibria
  void clearEquilibria();
  //! Consolidates equilibria
  /*! Appends the list of newEquilibria to the vector
      equilibria. Clears newEquilibria.*/
  void consolidateEquilibria();
  //! Returns the equilibrium at equilibriumIndex
  const BCEquilibrium & getEquilibrium(int equilibriumIndex) const;
  //! Set current equilibrium
  /*! Sets the current equilibrium to the equilibrium with index
    i. That vector must be an element of the vector equilibria. */
  void setCurrentEquilibrium(int equilibriumIndex);

  //! Set the number of private values
  void setNumValues(const vector<int> &_numValues);
  //! Assignment operator
  BCEData& operator=(const BCEData &rhs);

  /* Statistical routines */
  //! Gets the expected objectives under the current BCE
  /*!  This method will calculate the expected objectives for the
       current equilibrium, as specified by BCEData::currentIndex. */  
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
    the given player, type, and recommended action. */
  double getDeviationObjectives(int player, int action, int type,
				vector< vector<double> > & objectiveValues) const;
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

  //! Serialize a BCEData object using Boost.
  static void save(const BCEData & data, const char* filename)
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

  //! Deserialize a BCEData object using Boost.
  static void load(BCEData & data, const char* filename)
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
}; // BCEData

#endif
