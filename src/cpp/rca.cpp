#include "rca.hpp"
#include "multiindex.hpp"

RCABase::RCABase ():
  env(),
  model(env),
  cplex(env),
  cplexObjective(env),
  variables(env),
  constraints(env),
  numPlayers(2),
  numActions(2,0),
  numStates(0),
  numTypes(0),
  numMechanisms(2),
  numICConstraints(numMechanisms,vector<int>(numPlayers,0)),
  objectiveFunctions(2,IloNumExpr(0)),
  boundaryObjectiveIndex1(0),
  boundaryObjectiveIndex2(1),
  minAngleIncrement(0.0)
{} // Default constructor

RCABase::RCABase (int numPlayersArg, 
		  vector<int> numActionsArg, 
		  int numStatesArg, 
		  int numTypesArg, 
		  int numObjectivesArg):
  env(),
  model(env),
  cplex(env),
  cplexObjective(env),
  variables(env),
  constraints(env),
  objectiveFunctions(2,IloNumExpr(env)),
  numPlayers(numPlayersArg),
  numActions(numActionsArg),
  numStates(numStatesArg),
  numTypes(numTypesArg),
  numObjectives(numObjectivesArg),
  ICslack(0.0),
  numMechanisms(2),
  numICConstraints(numMechanisms,vector<int>(numPlayers,0)),
  minAngleIncrement(0.0),
  boundaryObjectiveIndex1(0),
  boundaryObjectiveIndex2(1)
{
  assert(numPlayers>1);
  assert(numActions[0]>0);
  assert(numActions[1]>0);
  assert(numTypes>0);
  assert(numStates>0);
  assert(numObjectives>=numPlayers*2);

  int mechanismCounter;

  numActions_total=0;
  for (mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
    numActions_total+=numActions[mechanismCounter];
} // Constructor that takes arguments for some parameters. 

RCABase::RCABase (int numPlayersArg, 
		  vector<int> numActionsArg, 
		  int numStatesArg, 
		  int numTypesArg, 
		  int numObjectivesArg,
		  vector<double> distributionArg,
		  double _observedTol):
  env(),
  model(env),
  cplex(env),
  cplexObjective(env),
  variables(env),
  constraints(env),
  numPlayers(numPlayersArg),
  numActions(numActionsArg),
  numStates(numStatesArg),
  numTypes(numTypesArg),
  numObjectives(numObjectivesArg),
  objectiveFunctions(numObjectives,IloNumExpr(env)),
  numMechanisms(2),
  numICConstraints(numMechanisms,vector<int>(numPlayers,0)),
  minAngleIncrement(0.0),
  boundaryObjectiveIndex1(0),
  boundaryObjectiveIndex2(1),
  observedTol(_observedTol),
  observedDistribution(distributionArg)
{
  assert(numPlayers>1);
  assert(numActions[0]>0);
  assert(numActions[1]>0);
  assert(numTypes>0);
  assert(numStates>0);
  assert(numObjectives>=numPlayers*2);
  assert(observedDistribution.size()==pow(numActions[0],numPlayers));

  int mechanismCounter;

  numActions_total=0;
  for (mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
    numActions_total+=numActions[mechanismCounter];

} // Constructor that also takes argument for observed marginal.

// Constructs the constraint matrix and objective function There is a
// variable for every state, profile of types, and action.  We will
// have IC constraints on every action and type profile, a probability
// constraint that the distribution must sum to 1, probability
// constraints that the conditional distribution over states must
// match the specified relationship between types and states, and a
// probabiltiy constraint that the total probability of each action
// profile must match that in the property observedDistribution.
void RCABase::populate ()
{
  int state, type, tempState, deviation;
  vector<int> action(numMechanisms,0); 
  vector<int> types(numPlayers,0), 
    tempTypes(numPlayers,0);

  vector< vector<int> > 
    // Holds current actions for all players and mechanisms
    actions (numMechanisms, vector<int>(numPlayers,0) ),
    // Holds current deviations for all players and mechanisms
    deviations (numMechanisms, vector<int>(numPlayers,0) ),
    // Keeps track of location of IC row for each player & mechanism
    ICRowsCounter(numMechanisms, vector<int>(numPlayers,0) ); 

  int columnCounter,
    variableCounter, 
    ICCounter, 
    probabilityConstraintCounter,
    probabilityLBConstraintsCounter,
    probabilityUBConstraintsCounter,
    playerCounter,
    objectiveCounter,
    mechanismCounter,
    mechanismICRow; // Number of action combinations per player for all mechanisms
  vector<int>::iterator numICConstraintsIterator;

  double LBCoeff, UBCoeff;

  numProbabilityVariables=0;
  nonZeroVariableLocations.resize(numStates*pow(numTypes*numActions_total,numPlayers));

  /*******************************/
  // Step 1: Count the number of probability variables, IC
  // constraints, and probability constraints.
  /*******************************/
  /* Probability variables */
  // There is a probability variable for every combination of actions
  // in the two mechanisms, every profile of types, and every state.
  for (variableCounter=0; 
       variableCounter<numStates*pow(numTypes*numActions_total,numPlayers); 
       variableCounter++)
    {
      indexToStateTypeAction(variableCounter,state,types,actions);

      // Only create variables if no player's action is dominated
      // under the respective mechanism, and if actions0 is observed
      // in the data.
      if (!dominated(actions,types)
	  && observedDistribution[actionToIndex(actions,0)]>0)
	{
	  numProbabilityVariables++;
	  nonZeroVariableLocations[numProbabilityVariables]=variableCounter;
	}
    }
  nonZeroVariableLocations.erase(nonZeroVariableLocations.begin()+numProbabilityVariables,
				 nonZeroVariableLocations.end()); // remove extra elements.

  numICConstraints_totalByMechanism.resize(numMechanisms,0);
  /* IC constraints */
  for (mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
    {  
      for (ICCounter=0; ICCounter<numTypes*numActions_total*numActions[0]; ICCounter++)
	{
	  indexToTypeActionDeviation(ICCounter,type,action,deviation,0);

	  // Create an IC constraint if both actions and the deviation are
	  // not dominated, and if the deviating action is distinct from
	  // the action in the given mechanism.
	  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
	    {
	      if (!(dominated(action,type,playerCounter)
		    || dominated(deviation,type,playerCounter,mechanismCounter)
		    || deviation==action[0]))
		numICConstraints[mechanismCounter][playerCounter]++; 
	    }
	} // ICCounter

      // Sum the IC constraints by mechanism.
      numICConstraints_totalByMechanism[mechanismCounter]=0;
      for (numICConstraintsIterator=numICConstraints[mechanismCounter].begin(); 
	   numICConstraintsIterator!=numICConstraints[mechanismCounter].end(); 
	   numICConstraintsIterator++)
	numICConstraints_totalByMechanism[mechanismCounter] += *numICConstraintsIterator;
    } // mechanismCounter

  // Sum the IC constraints.
  numICConstraints_total=0;
  for (numICConstraintsIterator=numICConstraints_totalByMechanism.begin(); 
       numICConstraintsIterator!=numICConstraints_totalByMechanism.end(); 
       numICConstraintsIterator++)
    numICConstraints_total += *numICConstraintsIterator;

  /* Probability constraints.*/
  // The entire distribution must sum to 1.0. Also, the probability of
  // a given state and type lie be less than the probability of that
  // type times conditionalUB and greater than the probability of that
  // type times conditionalLB. The UB and LB constraints are only
  // relevant when UB<1 and LB>0, respectively. Lastly, the
  // probability of each actions[0] profile must sum to the marginal.

  // Distribution sums to 1, plus the marginal constraints
  numProbabilityEqualityConstraints = 1 + pow(numActions[0],numPlayers);
  numProbabilityUBConstraints = 0;
  numProbabilityLBConstraints = 0;
  for (probabilityConstraintCounter=0; 
       probabilityConstraintCounter<numStates*pow(numTypes,numPlayers); 
       probabilityConstraintCounter++)
    {
      indexToStateType(probabilityConstraintCounter,state,types);
      
      for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
	{
	  if (conditionalLB(state,types[playerCounter],playerCounter)>0.0)
	      numProbabilityLBConstraints++;
	  if (conditionalUB(state,types[playerCounter],playerCounter)<1.0)
	      numProbabilityUBConstraints++;
	}
    }
  numProbabilityConstraints = 
    numProbabilityEqualityConstraints + numProbabilityUBConstraints
    + numProbabilityLBConstraints; 

  numNonBasicVariables=numProbabilityVariables-numProbabilityConstraints-1;

  // Print out summary of the construction so far.
  cout << "Done counting variables." << endl
       << "numProbabilityVariables=" << numProbabilityVariables << endl 
       << "numProbabilityConstraints=" << numProbabilityConstraints << endl;

  for (mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
    {
      for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
	cout << "numICConstraints for player " << playerCounter
	     << " in mechanism " << mechanismCounter
	     << " = " << numICConstraints[mechanismCounter][playerCounter] << endl;
    }
  cout << "numICConstraints_total=" << numICConstraints_total << endl;

  /********************************/
  // Step 2: Initialize variables, constraints, and objectives. Add
  // variables to the model.
  /********************************/
  // Add variables and constraints.
  variables.add(IloNumVarArray(env,numProbabilityVariables,0.0,1.0));
  // variables.add(IloNumVarArray(env,numICConstraints_total,-ICslack,IloInfinity));
  model.add(variables);

  // constraints.add(IloRangeArray(env,numICConstraints_total,0.0,0.0));
  // constraints.add(IloRangeArray(env,numProbabilityEqualityConstraints,1.0,1.0));
  // constraints.add(IloRangeArray(env,numProbabilityUBConstraints,0.0,IloInfinity));
  // constraints.add(IloRangeArray(env,numProbabilityLBConstraints,0.0,IloInfinity));

  // For some reason, the initialization list gives a different environment.
  // We reinitialize objectives with default expressions of 0.
  objectiveFunctions.clear();
  objectiveFunctions.resize(numObjectives);
  for (objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
    objectiveFunctions[objectiveCounter]=IloNumExpr(env,0);
  
  cout << "Done adding variables" << endl;

  /********************************/
  // Step 3: Fill in coefficients of the constraint matrix.
  /********************************/

  // // Set coefficients.
  // columnCounter=0; 
  // probabilityConstraintCounter=0;
  // for (variableCounter=0; 
  //      variableCounter<numStates*pow(numTypes*numActions_total,numPlayers); 
  //      variableCounter++)
  //   {
  //     indexToStateTypeAction(variableCounter,state,types,actions);

  //     if (!dominated(actions,types)
  // 	  && observedDistribution[actionToIndex(actions,0)]>0)
  // 	{
  // 	  // New column

  // 	  // Objective functions
  // 	  for (objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
  // 	    objectiveFunctions[objectiveCounter] += (payoff(state,actions,objectiveCounter)
  // 						     *variables[columnCounter]);

  // 	  // Reset the row counters.
  // 	  // ICRowsCounter[0][0]=0;
  // 	  ICRowsCounter = vector< vector<int> >(2,vector<int>(2,0));
  // 	  for (mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
  // 	    {
  // 	      mechanismICRow=0;
  // 	      for (playerCounter=1; playerCounter<numPlayers; playerCounter++)
  // 		{
  // 		  ICRowsCounter[mechanismCounter][playerCounter]
  // 		    =ICRowsCounter[mechanismCounter][playerCounter-1]
  // 		    +numICConstraints[mechanismCounter][playerCounter-1]
  // 		    +mechanismICRow;
  // 		}
  // 	      mechanismICRow+=numICConstraints_totalByMechanism[mechanismCounter];
  // 	    }

  // 	  deviations=actions;
  // 	  /* IC constraints */
  // 	  for (mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
  // 	    {
  // 	      for (ICCounter=0; 
  // 		   ICCounter<numTypes*numActions_total*numActions[mechanismCounter]; 
  // 		   ICCounter++)
  // 		{
  // 		  indexToTypeActionDeviation(ICCounter,type,action,deviation,0);

  // 		  // The coefficients should be the gain from deviating
  // 		  // from actions[player] to deviation
  // 		  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
  // 		    {
  // 		      if (action[mechanismCounter]==actions[mechanismCounter][playerCounter] 
  // 			  && type==types[playerCounter] 
  // 			  && deviation!=actions[mechanismCounter][playerCounter] 
  // 			  && !dominated(deviation,type,playerCounter,mechanismCounter))
  // 			{
  // 			  deviations[mechanismCounter][playerCounter]=deviation;

  // 			  constraints[ICRowsCounter[mechanismCounter][playerCounter]]
  // 			    .setLinearCoef(variables[columnCounter],
  // 					   (payoff(state,deviations,mechanismCounter*numPlayers+playerCounter)
  // 					    -payoff(state,actions,mechanismCounter*numPlayers+playerCounter)));

  // 			  deviations[mechanismCounter][playerCounter]
  // 			    =actions[mechanismCounter][playerCounter];
  // 			}
  // 		    }
  // 		  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
  // 		    {
  // 		      if (!(dominated(action[mechanismCounter],type,playerCounter,mechanismCounter) 
  // 			    || dominated(deviation,type,playerCounter,mechanismCounter) 
  // 			    || action[mechanismCounter]==deviation))
  // 			ICRowsCounter[mechanismCounter][playerCounter]++;
  // 		    }
  // 		} // ICCounter
  // 	    } // mechanismCounter
  // 	  // row

  // 	  /* Probability constraints */
  // 	  // All probabilities sum to 1.
  // 	  constraints[numICConstraints_total].setLinearCoef(variables[columnCounter],1.0);
	  
  // 	  // Marginal constraints. We put the marginal for actions[0]
  // 	  // in row determined by actionToIndex
  // 	  constraints[numICConstraints_total+1+actionToIndex(actions,0)]
  // 	    .setLinearCoef(variables[columnCounter],1.0);

  // 	  // Conditional distribution bounds. For each player, we need
  // 	  // to add this variable to the LB constraint for this
  // 	  // state/type, and subtract it from the UB constraint, with
  // 	  // coefficient equal to the conditional bound. Also, for
  // 	  // each player, we need to add this variable to the UB
  // 	  // constraint for this type and every state, and subtract it
  // 	  // from the LB constraint for this type and every state.  So
  // 	  // we need a function to map player/state/type to an index.

  // 	  // Note that we just count through all of these constraints
  // 	  // in the same order as we did for part 1, and just check if
  // 	  // each row corresponds to this column in that they
  // 	  // correspond to the same state and type for a given
  // 	  // player. This is an inefficient system, but that is not so
  // 	  // big a deal.
  // 	  for (probabilityConstraintCounter=0; 
  // 	       probabilityConstraintCounter<numStates*pow(numTypes,numPlayers); 
  // 	       probabilityConstraintCounter++)
  // 	    {
  // 	      indexToStateType(probabilityConstraintCounter,tempState,tempTypes);

  // 	      for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
  // 		{
  // 		  if (conditionalLB(state,types[playerCounter],playerCounter)>0.0)
  // 		    {
  // 		      // There exists a constraint for this player/state/type
  // 		      LBCoeff = -conditionalLB(playerCounter,state,types[playerCounter]);

  // 		      if (tempTypes[playerCounter]==types[playerCounter])
  // 			{
  // 			  // This column has a non-zero coefficient
  // 			  // since tempType matches player's type.
  // 			  if (state==tempState)
  // 			    LBCoeff+=1.0;
			  
  // 			  constraints[numICConstraints_total+1+pow(numActions[0],numPlayers)
  // 				      +probabilityLBConstraintsCounter]
  // 			    .setLinearCoef(variables[columnCounter],LBCoeff);
  // 			}

  // 		      probabilityLBConstraintsCounter++;
  // 		    }
  // 		  if (conditionalUB(state,types[playerCounter],playerCounter)<1.0)
  // 		    {
  // 		      // There exists a constraint for this player/state/type
  // 		      UBCoeff = conditionalUB(playerCounter,state,types[playerCounter]);

  // 		      if (tempTypes[playerCounter]==types[playerCounter])
  // 			{
  // 			  // This column has a non-zero coefficient
  // 			  // since tempType matches player's type.
  // 			  if (state==tempState)
  // 			    UBCoeff-=1.0;
			  
  // 			  constraints[numICConstraints_total+1+pow(numActions[0],numPlayers)
  // 				      +numProbabilityLBConstraints
  // 				      +probabilityUBConstraintsCounter]
  // 			    .setLinearCoef(variables[columnCounter],UBCoeff);
  // 			}

  // 		      probabilityUBConstraintsCounter++;
  // 		    }
  // 		} // Player
  // 	    } // probabilityConstraintCounter

  // 	  // Every once in a while, print progress.
  // 	  if (!(columnCounter%(1000)))
  // 	    cout << "columnCounter=" << columnCounter << endl;
	  
  // 	  columnCounter++;
  // 	} // if
  //   } // column

  // // Set the RHS for the marginal constraints. It doesn't really
  // // matter how we map actions[0] into a linear index, as long as we
  // // do it consistently.
  // for (probabilityConstraintCounter=0; 
  //      probabilityConstraintCounter<pow(numActions[0],numPlayers);
  //      probabilityConstraintCounter++)
  //   {
  //     constraints[probabilityConstraintCounter+numICConstraints_total]
  // 	.setBounds(observedDistribution[probabilityConstraintCounter]-observedTol,
  // 		   observedDistribution[probabilityConstraintCounter]+observedTol);
  //   }

  // // The first numICConstraints_total rows need slack variables.
  // for (ICCounter=0; ICCounter<numICConstraints_total; ICCounter++)
  //   {
  //     constraints[ICCounter].setLinearCoef(variables[numProbabilityVariables+ICCounter],1.0);
  //   }

  /* IC constraints */


  

  // Set up the iterator
  MultiIndex stateTypesActions;

  IntIndex * state = new IntIndex(numStates);
  stateTypesActions->push_back(state);

  vector<IntIndex*> types;
  vector<MultiIntIndex*> actions;
  vector<MultiIndex*> typeActions;
  for (int player = 0; player < numPlayers; player++)
    {
      types.push_back(new IntIndex(numTypes));

      actions.push_back(new MultiIntIndex(numActions));

      typeActions[player].push_back(types[player]);
      typeActions[player].push_back(actions[player]);

      stateTypesActions.push_back(typeActions[player]); 
    } // for player
  

  for (player = 0; player < numPlayers; player++)
    {
      // Turn this player's typeActions off for the inner iteration
      stateTypesActions->setActive(1+player,false);
      actionsType[player]->reset();

      while ( (*typeActions)++ )
	{
	  stateTypesActions->reset();
	  
	  for (int mech = 0; mech < numMechanisms; mech++)
	    {
	      IloNumExpr lhs(env,0);
	      
	      // First consider deviations in first mechanism
	      for (int dev = 0; deviate < numActions[mech]; dev++)
		{
		  while (stateTypesActions++)
		    {
		      // lhs += ;
		    } // while
		} // Deviation

	    } // for mech
	} // actions
      
      // Turn this player's typeActions back on for the next player
      stateTypeActions->setActive(1+player,true);
    } // player


      for (ICCounter=0; 
	   ICCounter<numTypes*numActions_total*numActions[mechanismCounter]; 
	   ICCounter++)
	{
	  indexToTypeActionDeviation(ICCounter,type,action,deviation,0);

	  // The coefficients should be the gain from deviating
	  // from actions[player] to deviation
	  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
	    {
	      if (action[mechanismCounter]==actions[mechanismCounter][playerCounter] 
		  && type==types[playerCounter] 
		  && deviation!=actions[mechanismCounter][playerCounter] 
		  && !dominated(deviation,type,playerCounter,mechanismCounter))
		{
		  deviations[mechanismCounter][playerCounter]=deviation;

		  constraints[ICRowsCounter[mechanismCounter][playerCounter]]
		    .setLinearCoef(variables[columnCounter],
				   (payoff(state,deviations,mechanismCounter*numPlayers+playerCounter)
				    -payoff(state,actions,mechanismCounter*numPlayers+playerCounter)));

		  deviations[mechanismCounter][playerCounter]
		    =actions[mechanismCounter][playerCounter];
		}
	    }
	  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
	    {
	      if (!(dominated(action[mechanismCounter],type,playerCounter,mechanismCounter) 
		    || dominated(deviation,type,playerCounter,mechanismCounter) 
		    || action[mechanismCounter]==deviation))
		ICRowsCounter[mechanismCounter][playerCounter]++;
	    }
	} // ICCounter
    } // mechanismCounter
  // row
  

  /********************************/
  // Step 4: Add constraints to the model.
  /********************************/
  // model.add(constraints);

  cplexObjective=IloMaximize(env);
  model.add(cplexObjective);
  
  cplex=IloCplex(model);

  cplexObjective=cplex.getObjective();
  cplexObjective.setExpr(objectiveFunctions[1]);
} // populate

void RCABase::mapBoundary()
{
  cout << "mapping objfun1 vs objfun2..." << endl;

  // Store the defaults so we can restore them at the end.
  IloInt BarDisplayDefault = cplex.getParam(IloCplex::BarDisplay);
  IloInt DisplayDefault = cplex.getParam(IloCplex::SimDisplay);

  cplex.setParam(IloCplex::BarDisplay, 0);
  cplex.setParam(IloCplex::SimDisplay, 0);

  // Clear the equilibria array.
  equilibria.clear();

  // First solve 
  cplexObjective.setExpr(objectiveFunctions[boundaryObjectiveIndex1]);
  cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
  cplex.setParam(IloCplex::BarCrossAlg,IloCplex::Primal);
  cplex.solve();

  cout << "Northeast quadrant..." << endl;
  objectiveIndex1=objectiveFunctions[boundaryObjectiveIndex1]; 
  objectiveIndex2=objectiveFunctions[boundaryObjectiveIndex2];
  mapFrontier(1,1,0);
  cout << "Northwest quadrant..." << endl;
  objectiveIndex1=objectiveFunctions[boundaryObjectiveIndex2]; 
  objectiveIndex2=-objectiveFunctions[boundaryObjectiveIndex1];
  mapFrontier(1,-1,1);
  cout << "Southwest quadrant..." << endl;
  objectiveIndex1=-objectiveFunctions[boundaryObjectiveIndex1]; 
  objectiveIndex2=-objectiveFunctions[boundaryObjectiveIndex2];
  mapFrontier(-1,-1,0);
  cout << "Southeast quadrant..." << endl;
  objectiveIndex1=-objectiveFunctions[boundaryObjectiveIndex2]; 
  objectiveIndex2=objectiveFunctions[boundaryObjectiveIndex1];
  mapFrontier(-1,1,1);

  // Print the bndry points.
  ofstream vertexData;
  list<RCAPoint>::iterator pointIterator;
  vertexData.open("bndry.dat");
  for (pointIterator=boundaryPoints.begin(); 
       pointIterator!=boundaryPoints.end(); 
       pointIterator++)
    vertexData << *pointIterator;
  vertexData.close();
  

  cplex.setParam(IloCplex::BarDisplay,BarDisplayDefault);
  cplex.setParam(IloCplex::SimDisplay,DisplayDefault);
}

void RCABase::mapFrontier(int plusOrMinus1, int plusOrMinus2, bool reversePrint)
{
  IloNum objectiveWeight=0, newWeight, alpha;

  IloCplex::BasisStatusArray basisStatuses(env);
  vector<double> currentEquilibrium;
  IloNumArray values(env);
  IloNumArray reducedCosts1(env);
  IloNumArray reducedCosts2(env);

  IloNum objectiveValue1, objectiveValue2;

  int variableCounter=0;
  int ItLimDefault = cplex.getParam(IloCplex::ItLim);

  double oldAngleIncrement, minalpha;

  int displayVal=0;

  cplexObjective.setSense(IloObjective::Maximize);
  cplex.setParam(IloCplex::EpOpt, 1E-9);

  int iterationCounter = 0;
  while ( iterationCounter < 1E8 )
    {
      // solve the new objective function.
      // Switch to dual for the initial solve phase. Might take awhile.
      cplexObjective.setExpr(objectiveWeight*objectiveIndex2 + objectiveIndex1);
      cplex.setParam(IloCplex::ItLim, ItLimDefault);
      cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
      cplex.solve();

      cplex.setParam(IloCplex::RootAlg, IloCplex::Primal);
      cplex.setParam(IloCplex::ItLim, 0);
      cplex.solve();
      
      cplex.getBasisStatuses(basisStatuses,variables);
      // Print current equilibrium
      distributionToVector(currentEquilibrium);
      equilibria.push_back(RCAEquilibrium(2,numActions,numStates,numTypes,currentEquilibrium));

      // Find the next weight
      cplexObjective.setExpr(objectiveIndex1);
      cplex.solve();
      cplex.getReducedCosts(reducedCosts1,variables);
      objectiveValue1 = cplex.getObjValue();

      cplexObjective.setExpr(objectiveIndex2);
      cplex.solve();
      cplex.getReducedCosts(reducedCosts2,variables);
      objectiveValue2 = cplex.getObjValue();

      // Now find the smallest new weight that is greater than the old weight.
      newWeight=-1;
      for (variableCounter = 0; variableCounter<variables.getSize(); variableCounter++)
	{
	  if (basisStatuses[variableCounter]!=IloCplex::Basic 
	      && reducedCosts1[variableCounter]<0 
	      && reducedCosts2[variableCounter]>0)
	    {
	      alpha = -reducedCosts1[variableCounter]/reducedCosts2[variableCounter]+1E-8;
	      if (alpha>objectiveWeight && ( alpha<newWeight || newWeight<0 ))
		newWeight = alpha;
	    }
	}
      if (newWeight<0)
	break; // Could not find a new weight that met criteria. Done.

      if (minAngleIncrement>0)
      	{
      	  oldAngleIncrement = atan(objectiveWeight);
      	  minalpha = tan(oldAngleIncrement + minAngleIncrement);
	  
      	  if (newWeight < minalpha)
      	    newWeight = minalpha;
      	}

      objectiveWeight = newWeight;

      if (!(iterationCounter%100))
	cout << setprecision(3) 
	     << "weight on objective 2 = " << objectiveWeight 
	     << ", iterationCounter = " << iterationCounter << endl;

      // Add new element to the list.
      if (reversePrint)
	boundaryPoints.push_back(RCAPoint(plusOrMinus2*objectiveValue2,
					  plusOrMinus1*objectiveValue1));
      else
	boundaryPoints.push_back(RCAPoint(plusOrMinus1*objectiveValue1,
					  plusOrMinus2*objectiveValue2));

      basisStatuses.clear();
      reducedCosts1.clear();
      reducedCosts2.clear();

      iterationCounter++;
    }

  basisStatuses.end();
  reducedCosts1.end();
  reducedCosts2.end();

  cout << "Final weight = " << objectiveWeight << ", total iterations = " << iterationCounter << endl;
  // cout << "# of elements of list: " << boundaryPoints.size() << endl;

  cplex.setParam(IloCplex::ItLim,ItLimDefault);
} // mapFrontier

// Clears the variables, constraints, and objectives.
void RCABase::clear()
{
  variables.end();
  constraints.end();
  for (int objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
    objectiveFunctions[objectiveCounter].end();
}

// Overloaded <<
ostream& operator<<(ostream& output, RCABase &rhs)
{
  // list<RCAEquilibrium>::iterator equilibriumIterator;
  // int variableCounter, playerCounter, priorCounter;
  // int state;
  // vector<int> types(rhs.numPlayers,0), actions(rhs.numPlayers,0);

  // // Print out numPlayers, numActions, numStates, and numTypes
  // output << rhs.numPlayers << endl;
  // for (mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
  //   output << rhs.numActions << endl;
  // output << rhs.numStates << endl
  // 	 << rhs.numTypes << endl;

  // // Print the prior.
  // for (priorCounter=0; 
  //      priorCounter<rhs.numStates*(RCABase::pow(rhs.numTypes,rhs.numPlayers)); 
  //      priorCounter++)
  //   {
  //     rhs.indexToStateType(priorCounter,state,types);
  //     output << rhs.prior(state,types) << " ";
  //   }
  // output << endl;

  // // Print the dominated acts
  // for (variableCounter=0; 
  //      variableCounter<RCABase::pow(rhs.numTypes*rhs.numActions,rhs.numPlayers); 
  //      variableCounter++)
  //   {
  //     for (playerCounter=0; playerCounter<rhs.numPlayers; playerCounter++)
  // 	{
  // 	  rhs.indexToTypeAction(variableCounter,types,actions);
  // 	  output << rhs.dominated(actions[mechanismCounter][playerCounter],types[playerCounter],playerCounter) << " ";
  // 	}
  //   }

  // // Now print out the payoffs.
  // for (playerCounter=0; playerCounter<rhs.numPlayers; playerCounter++)
  //   {
  //     for (variableCounter=0; 
  // 	   variableCounter<rhs.numStates*(RCABase::pow(rhs.numActions,rhs.numPlayers)); 
  // 	   variableCounter++)
  // 	{
  // 	  rhs.indexToStateAction(variableCounter,state,actions);
  // 	  output << rhs.payoff(state,actions,playerCounter) << " ";
  // 	}
  //     output << endl;
  //   }

  // // Print the equilibria.
  // for (equilibriumIterator=rhs.equilibria.begin(); 
  //      equilibriumIterator!=rhs.equilibria.end(); 
  //      equilibriumIterator++)
  //   output << *equilibriumIterator << endl;

  return output;
}

void RCABase::distributionToVector(vector<double> & distribution)
{
  int counter;
  IloNumArray values(env);

  cplex.getValues(values,variables);
  distribution.resize(numStates*pow(numTypes*numActions_total,numPlayers),0.0);

  // Initialize everything to zeros.
  for (counter=0; counter<distribution.size(); counter++)
    distribution[counter]=0.0;

  // Populate the distribution vector.
  for (counter=0; counter<numProbabilityVariables; counter++)
    distribution[nonZeroVariableLocations[counter]]=values[counter];

  values.end();
}

void RCABase::actionMarginalToVector(vector<double> & distribution)
{
  int variableCounter;
  IloNumArray cplexValues(env);

  vector<int> types(numPlayers,0);
  vector< vector<int> > actions(numMechanisms, vector<int>(numPlayers,0));
  int state;

  cplex.getValues(cplexValues,variables);
  distribution.resize(numStates*pow(numTypes*numActions_total,numPlayers),0.0);

  // Initialize everything to zeros.
  for (variableCounter=0; variableCounter<distribution.size(); variableCounter++)
    distribution[variableCounter]=0.0;

  for (variableCounter=0; 
       variableCounter<numStates*pow(numTypes*numActions_total,numPlayers); 
       variableCounter++)
    {
      indexToStateTypeAction(variableCounter,state,types,actions);

      if (!dominated(actions,types)
	  && observedDistribution[actionToIndex(actions,0)]>0)
	{
	  distribution[actionToIndex(actions,0)]+=cplexValues[numProbabilityVariables];

	  numProbabilityVariables++;
	}
    }

  cplexValues.end();
}

// Checks if any player has a dominated action/type.
bool RCABase::dominated(const vector<int> &actions, const vector<int> &types, int mechanism)
{
  assert(actions.size()==numPlayers);
  assert(types.size()==numPlayers);
  assert(mechanism==0 || mechanism==1);

  for (int playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      assert(actions[playerCounter]>=0);
      assert(types[playerCounter]>=0);
      assert(actions[playerCounter]<numActions[mechanism]);
      assert(types[playerCounter]<numTypes);

      if (dominated(actions[playerCounter],types[playerCounter],playerCounter,mechanism))
	return (true);
    }
  return (false);
}

// Runs the previous version of dominated for all mechanisms
bool RCABase::dominated(const vector< vector<int> > &actions, const vector<int> &types)
{
  assert(actions.size()==numMechanisms);
  assert(types.size()==numPlayers);

  for (int mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
    {
      assert(actions[mechanismCounter].size()==numPlayers);

      if (dominated(actions[mechanismCounter],types,mechanismCounter))
	return(true);
    }
  return(false);
}

// Similar to above, but for one player at a time.
bool RCABase::dominated(const vector<int> &action, int type, int player)
{
  assert(action.size()==numMechanisms);
  assert(type>=0);
  assert(type<numTypes);
  assert(player>=0);
  assert(player<numPlayers);

  for (int mechanismCounter=0; mechanismCounter<numMechanisms; mechanismCounter++)
    {
      if (dominated(action[mechanismCounter],type,player,mechanismCounter))
	return(true);
    }
  return(false);
}

void RCABase::indexToStateTypeAction(int index,
				      int &state, 
				      vector<int> &types,
				      vector< vector<int> > &actions)
{
  int playerCounter, mechanismCounter;

  // Resize arguments
  types.resize(numPlayers);
  actions.resize(numMechanisms);

  assert(index>=0);
  assert(index<numStates*pow(numActions_total*numTypes,numPlayers));

  state=index%numStates;
  index-=state; index/=numStates;
  assert(state>=0);
  assert(state<numStates);

  for (mechanismCounter=0; mechanismCounter++; mechanismCounter<numMechanisms)
    {
      // Resize the actions array if needed
      actions[mechanismCounter].resize(numActions[mechanismCounter]);

      for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
	{
	  actions[mechanismCounter][playerCounter]=index%numActions[mechanismCounter];
	  index-=actions[mechanismCounter][playerCounter]; 
	  index/=numActions[mechanismCounter];
      
	  assert(actions[mechanismCounter][playerCounter]>=0);
	  assert(actions[mechanismCounter][playerCounter]<numActions[mechanismCounter]);
	}
    }
  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      types[playerCounter]=index%numTypes;
      index-=types[playerCounter]; index/=numTypes;

      assert(types[playerCounter]>=0);
      assert(types[playerCounter]<numTypes);
    }

}

void RCABase::indexToTypeActionDeviation(int index, 
					  int &type, 
					  vector<int> &action, 
					  int &deviation, 
					  int mechanism)
{
  int mechanismCounter;

  assert(index>=0);
  assert(index<numTypes*numActions_total*numActions[mechanism]);

  deviation=index%numActions[mechanism];
  index-=deviation; index/=numActions[mechanism];
  for (mechanismCounter=0; 
       mechanismCounter<numMechanisms;
       mechanismCounter++)
    {
      action[mechanismCounter]=index%numActions[mechanismCounter];
      index-=action[mechanismCounter]; index/=numActions[mechanismCounter];

      assert(action[mechanismCounter]>=0);
      assert(action[mechanismCounter]<numActions[mechanismCounter]);
    }
  type=index;

  assert(type>=0);
  assert(deviation>=0);
  assert(type<numTypes);
  assert(deviation<numActions[mechanism]);
}

void RCABase::indexToStateType(int index, int &state, vector<int> & types)
{
  int playerCounter;

  // Make sure arguments are right size
  types.resize(numPlayers);
 
  assert(index>=0);
  assert(index<numStates*pow(numTypes,numPlayers));

  state=index%numStates;
  index-=state; index/=numStates;
  assert(state>=0);
  assert(state<numStates);

  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      types[playerCounter]=index%numTypes;
      index-=types[playerCounter]; index/=numTypes;

      assert(types[playerCounter]<numTypes);
      assert(types[playerCounter]>=0);
    }
}

void RCABase::indexToStateAction(int index, int &state, vector<int> &actions, int mechanism)
{
  int playerCounter;

  // Make sure arguments are right size
  actions.resize(numPlayers);

  assert(index>=0);
  assert(index<numStates*pow(numActions[mechanism],numPlayers));

  state=index%numStates;
  index-=state; index/=numStates;
  assert(state>=0);
  assert(state<numStates);

  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      actions[playerCounter]=index%numActions[mechanism];
      index-=actions[playerCounter]; index/=numActions[mechanism];

      assert(actions[playerCounter]>=0);
      assert(actions[playerCounter]<numActions[mechanism]);
    }
}

int RCABase::stateTypeToIndex(int state, const vector<int> & types)
{
  int index=state*numStates;
  int playerCounter;

  assert(state>=0);
  assert(state<numStates);

  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      assert(types[playerCounter]>=0);
      assert(types[playerCounter]<numTypes);

      index+=types[playerCounter]; index*=numTypes;
    }
  index+=types[numPlayers-1];

  return (index);
}

int RCABase::actionToIndex(const vector< vector<int> > &actions,
			     int mechanism)
{
  int index=actions[mechanism][0];
  int playerCounter;
  
  assert(mechanism>=0);
  assert(mechanism<numMechanisms);
  for (playerCounter=1; playerCounter<numPlayers; playerCounter++)
    {
      assert(actions[mechanism][playerCounter]>=0);
      assert(actions[mechanism][playerCounter]<numActions[mechanism]);

      index*=numActions[mechanism];
      index+=actions[mechanism][playerCounter];
    }

  return index;
}
