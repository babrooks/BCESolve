#include "bce.hpp"
#include "bcegurobisolver.hpp"

BCEGurobiSolver::BCEGurobiSolver ():
  env(),
  model(env),
  variables(),
  constraints(),
  objectiveFunctions(2,GRBLinExpr()),
  bndryObjectives(2,GRBLinExpr()),
  boundaryObjectiveIndex1(0),
  boundaryObjectiveIndex2(1),
  minAngleIncrement(0.0),
  displayLevel(1)
{} // Default constructor

BCEGurobiSolver::BCEGurobiSolver (BCEAbstractGame & _game):
  game(&_game),
  env(),
  model(env),
  variables(),
  constraints(),
  objectiveFunctions(2,GRBLinExpr()),
  bndryObjectives(2,GRBLinExpr()),
  numICConstraints(2,0),
  minAngleIncrement(0.0),
  boundaryObjectiveIndex1(0),
  boundaryObjectiveIndex2(1),
  soln(BCEGame(_game)),
  numActions_total(1),
  numTypes_total(1),
  numActionsTypes_total(1),
  numActionsTypesPerPlayer_total(0),
  displayLevel(1)
{
  // Set the total type/action variables
  countActionsTypes();
} // Constructor that takes arguments for 

void BCEGurobiSolver::countActionsTypes()
{
  // Set the numActions_total and numTypes_total
  int playerCounter;

  const int numPlayers = 2;
  const int numStates = game->getNumStates();
  const int numObjectives = game->getNumObjectives();
  const vector<int> & numTypes = game->getNumTypes();
  const vector<int> & numActions = game->getNumActions();
  
  for (playerCounter=0; playerCounter<numPlayers; playerCounter++)
    {
      numActions_total *= numActions[playerCounter];
      numTypes_total *= numTypes[playerCounter];
      numActionsTypesPerPlayer_total += numActions[playerCounter]*numTypes[playerCounter];
      numActionsTypes_total *= numActions[playerCounter]*numTypes[playerCounter];
    }
}

void BCEGurobiSolver::clear()
{
  model.reset();
}

void BCEGurobiSolver::setParameter(BCEGurobiSolver::DoubleParameter param, double arg)
{
  switch (param)
    {
    case MinAngleIncrement:
      if (arg<0)
	throw(BCEException(BCEException::InvalidParameterValue));

      minAngleIncrement = arg;
      break;

    default:
      throw(BCEException(BCEException::InvalidParameterName));
    } // switch
} // setParameter

void BCEGurobiSolver::setParameter(BCEGurobiSolver::IntParameter param, int arg)
{
  switch (param)
    {
    case CurrentObjective:
      if (arg<0 || arg>=game->getNumObjectives())
	throw(BCEException(BCEException::InvalidParameterValue));

      currentObjective = arg;
      break;
      
    case DisplayLevel:
      displayLevel = arg;
      break;

    case BoundaryObjective1:
      boundaryObjectiveIndex1 = arg;
      break;

    case BoundaryObjective2:
      boundaryObjectiveIndex2 = arg;
      break;

    default:
      throw(BCEException(BCEException::InvalidParameterName));
    } // switch
}

void BCEGurobiSolver::setParameter(BCEGurobiSolver::BoolParameter param, bool arg)
{
  switch (param)
    {
    default:
      throw(BCEException(BCEException::InvalidParameterName));
    } // switch
}

double BCEGurobiSolver::getParameter(BCEGurobiSolver::DoubleParameter param)
{
  switch (param)
    {
    case MinAngleIncrement:
      return minAngleIncrement;
    default:
      throw(BCEException(BCEException::InvalidParameterName));
    } // switch
} // getParameter

int BCEGurobiSolver::getParameter(BCEGurobiSolver::IntParameter param)
{
  switch (param)
    {
    case CurrentObjective:
      return currentObjective;
    case DisplayLevel:
      return displayLevel;
    case BoundaryObjective1:
      return boundaryObjectiveIndex1;
    case BoundaryObjective2:
      return boundaryObjectiveIndex2;
    default:
      throw(BCEException(BCEException::InvalidParameterName));
    } // switch
}

bool BCEGurobiSolver::getParameter(BCEGurobiSolver::BoolParameter param)
{
  switch (param)
    {
    default:
      throw(BCEException(BCEException::InvalidParameterName));
    } // switch
}

// Sets the expressions for the boundary objectives
void BCEGurobiSolver::setBndryObjective(int index, const GRBLinExpr & expr)
{
  bndryObjectives[index] = expr;
}


// Constructs the constraint matrix and objective function
void BCEGurobiSolver::populate ()
{
  const int numPlayers = 2;
  const int numStates = game->getNumStates();
  const int numObjectives = game->getNumObjectives();
  const vector<int> & numTypes = game->getNumTypes();
  const vector<int> & numActions = game->getNumActions();

  int state, type, action, deviation;
  vector<int> types(numPlayers,0), 
    actions(numPlayers,0), 
    deviations(numPlayers,0);
  volatile int variableCounter, 
    ICCounter, 
    probabilityConstraintCounter,
    player,
    objectiveCounter,
    priorCounter;
  int row, col;
  vector<int>::iterator numICConstraintsIterator;


  // cplex.end();

  numProbabilityVariables=0;
  numProbabilityConstraints=0;
  nonZeroVariableLocations.clear();
  probabilityConstraintLocations = vector<int>(numStates*numTypes_total,-1);

  if (displayLevel) {
    string printStr = "Starting populate routine";
    cout << printStr << endl;
    // emit(updateLog(printStr));
  }

  // Probability variables are conditional on the state and type. 

  // We will only create variables variables for probabilities of
  // events that can occur, i.e. the prior is non-zero and the
  // action/type pair is undominated.

  // We will only create conditional probability constraints for
  // events with a non-zero prior.

  // Count the number of probability variables and IC constraints.
  // Probability variables
  BCECounter counter(numStates,numActions,numTypes,
		     vector<int>(0),vector<vector<int> >(2),vector< vector<int> > (2),
		     true,vector<bool>(2,true),vector<bool>(2,false));


  numProbabilityVariables=0;
  variableLocationsMap.clear();
  do
    {
      // Only create variables if neither action is dominated and 
      // if the combination of values and types could arise.
      if ((!game->dominated(counter.getActions(),counter.getTypes()))
	  && game->prior(counter.getState(),counter.getTypes())>0.0)
	{
	  nonZeroVariableLocations.push_back(counter.getVariable());
	  // variableLocationsMap.emplace(counter.getVariable(),numProbabilityVariables);
	  numProbabilityVariables++;
	}
    } while (++counter); // variableCounter

  // IC constraints
  numICConstraints = vector<int>(2,0);
  for (player=0; player<numPlayers; player++)
    {
      for (ICCounter=0; 
	   ICCounter<numTypes[player]*numActions[player]*numActions[player]; 
	   ICCounter++)
	{
	  indexToTypeActionDeviation(ICCounter,player,type,action,deviation);

	  if (deviation==action)
	    continue;

	  // Create an IC constraint if both the action and the deviation
	  // are not dominated, and if the deviating action is distinct.
	  if (!(game->dominated(action,type,player) 
		|| game->dominated(deviation,type,player) )
	      && game->feasibleDeviation(action,deviation,type,player) )
	    {
	      numICConstraints[player]++; 
	    } // ! dominated
	} // ICCounter
    } // player


  // Sum the IC constraints.
  numICConstraints_total=0;
  for (numICConstraintsIterator=numICConstraints.begin(); 
       numICConstraintsIterator!=numICConstraints.end(); 
       numICConstraintsIterator++)
    numICConstraints_total += *numICConstraintsIterator;

  // The "adding up" constraints. Conditional probabilities must
  // sum to 1, whenever the prior is non-zero.
  counter = BCECounter(numStates,numActions,numTypes,
		       vector<int>(0),vector< vector<int> >(2,vector<int>(1,1)),
		       vector<vector<int> >(2),
		       true,vector<bool>(2,false),vector<bool>(2,true));
  do
    {
      if (game->prior(counter.getState(),counter.getTypes())>0.0)
	{
	  probabilityConstraintLocations[counter.getMarginal()]=numProbabilityConstraints;
	  numProbabilityConstraints++;
	}
    } while (++counter);
  numNonBasicVariables=numProbabilityVariables-numProbabilityConstraints-1;

  // Print out summary of the construction so far.
  if (displayLevel)
    {
      cout << "Done counting variables." << endl
	   << "numProbabilityVariables=" << numProbabilityVariables << endl 
	   << "numProbabilityConstraints=" << numProbabilityConstraints << endl;
      for (player=0; player<numPlayers; player++)
	cout << "numICConstraints for player " << player << "=" << numICConstraints[player] << endl;
      cout << "numICConstraints_total=" << numICConstraints_total << endl;
    }

  // // Add variables and constraints.
  // for (int var=0; var < numProbabilityVariables; var++) {
  //   variables.addVar(0.0,1.0,1.0,GRB_CONTINUOUS);
  // }
  // // variables.addVars(IloNumVarArray(env,numProbabilityVariables,0.0,1.0));
  // for (int var = 0; var < numICConstraints_total; var++) {
  //   variables.addVar(0.0,GRB.INFINITY,1.0,GRB_CONTINUOUS);
  // }
  // // variables.addVars(IloNumVarArray(env,numICConstraints_total,0.0,IloInfinity));

  variables = model.addVars(numProbabilityVariables+numICConstraints_total);
  model.update();
  for (int prVars = 0; prVars < numProbabilityVariables; prVars++) {
    variables[prVars].set(GRB_DoubleAttr_LB,0.0);
    variables[prVars].set(GRB_DoubleAttr_UB,1.0);
  }
  for (int countIC = numProbabilityVariables; countIC < numICConstraints_total; countIC++) {
    variables[countIC].set(GRB_DoubleAttr_LB,0.0);
    variables[countIC].set(GRB_DoubleAttr_UB,GRB_INFINITY);
  }

  // constraints = model.addConstrs(numProbabilityVariables+numICConstraints_total);
  // model.update();
  //   constraints.add(IloRangeArray(env,numICConstraints_total,0.0,0.0));
  // constraints.add(IloRangeArray(env,numProbabilityConstraints,1.0,1.0));
  constraints = vector<GRBLinExpr>(numICConstraints_total+numProbabilityConstraints,0);
  for (int ctr = 0; ctr < numICConstraints_total+numProbabilityConstraints; ctr++)
    constraints[ctr] = 0;
  // For some reason, the initialization list gives a different environment.
  // We reinitialize objectives with default expressions of 0.
  objectiveFunctions.clear();
  objectiveFunctions.resize(numObjectives);
  for (objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
    objectiveFunctions[objectiveCounter] = 0;
  
  if (displayLevel)
    cout << "Done adding variables" << endl;
  
  vector<int> stateConditions(0);
  vector< vector<int> > actionConditions(2);
  vector< vector<int> > typeConditions(2);
  bool stateMarginal = true;
  vector<bool> actionMarginal(2,true);
  vector<bool> typeMarginal(2,false);

  if (displayLevel)
    cout << "Populating IC constraints." << endl;

  row = 0;

  time_t startIC, nowIC;
  time(&startIC);

  // IC constraints
  for (player=0; player<numPlayers; player++)
    {
      for (ICCounter=0; 
	   ICCounter<numTypes[player]*numActions[player]*numActions[player]; 
	   ICCounter++)
	{
	  indexToTypeActionDeviation(ICCounter,player,type,action,deviation);

	  if (deviation==action)
	    continue;
	  typeConditions[player] = vector<int>(1,type);
	  typeConditions[1-player] = vector<int>(0);
	  actionConditions[player] = vector<int>(1,action);
	  actionConditions[1-player] = vector<int>(0);
	  deviations[player] = deviation;

	  if (!(game->dominated(action,type,player) 
		|| game->dominated(deviations[player],type,player))
	      && game->feasibleDeviation(action,deviation,type,player) )
	    {
	      GRBLinExpr lhs = 0;

	      counter = BCECounter(numStates,numActions,numTypes,
				   stateConditions,actionConditions,typeConditions,
				   stateMarginal,actionMarginal,typeMarginal);
		      
	      assert(counter.getActions()[player]==action);
	      assert(counter.getTypes()[player]==type);

	      do
		{
		  if (variableLocationsMap.count(counter.getVariable()))
		    {
		      deviations[1-player] = counter.getActions()[1-player];
		      types[1-player] = counter.getTypes()[1-player];

		      lhs += variables[variableLocationsMap.at(counter.getVariable())]
			*(game->objective(counter.getState(),deviations,player)
			  -game->objective(counter.getState(),counter.getActions(),player))
			*game->prior(counter.getState(),counter.getTypes());
		    }
		} while (++counter);

	      lhs += variables[numProbabilityVariables + row];
	      
	      constraints[row] += lhs;

	      row++;

	      if (!(row % (numICConstraints_total/20>0? numICConstraints_total/20:1)) && displayLevel)
		{
		  time(&nowIC);
		  double secElapsed = difftime(startIC,nowIC);
		  double remaining = secElapsed * (1-numICConstraints_total/(1.0*row));
		  int minRemaining = remaining/60, 
		    secRemaining = remaining - minRemaining*60;
		  cout << "player = " << player << ", row=" << row << ", ";
		  if (minRemaining>0)
		    cout << minRemaining << " min ";
		  cout << secRemaining << " sec remaining" << endl;
		}

	    } // if not dominated, etc
	} // ICCounter
    } // player

  if (displayLevel)
    cout << "Populating probability constraints " << endl;
  // Probability variables
  for (type = 0; type < numTypes_total; type++)
    {
      for (state = 0; state < numStates; state++)
	{
	  if (probabilityConstraintLocations[type*numStates + state]>=0)
	    {
	      GRBLinExpr lhs = 0;

	      for (action = 0; action < numActions_total; action++)
		{
		  col = type*numActions_total*numStates
		    + action * numStates
		    + state;
		  if (variableLocationsMap.count(col))
		    lhs += variables[variableLocationsMap[col]];
		}
	      
	      constraints[row] += lhs;
	      
	      row++;
	    } // if
	} // state
    } // type

  if (displayLevel)
    cout << "Populating objectives." << endl;
  // Objectives.
  counter = BCECounter(numStates,numActions,numTypes,
		       vector<int>(0),vector<vector<int> >(2),
		       vector< vector<int> > (2),
		       true,vector<bool>(2,true),vector<bool>(2,false));
  col=0; 
  do
    {
      if ((!game->dominated(counter.getActions(),counter.getTypes())) 
	  && game->prior(counter.getState(),counter.getTypes())>0.0)
	{
	  // New column.

	  // Objective functions
	  for (objectiveCounter=0; objectiveCounter<numObjectives; objectiveCounter++)
	    objectiveFunctions[objectiveCounter] 
	      += (game->objective(counter.getState(),counter.getActions(),objectiveCounter)
		  *game->prior(counter.getState(),counter.getTypes())
		  *variables[variableLocationsMap[counter.getVariable()]]);
	  
	  // Every once in a while, print progress.
	  if (!(col%(5000)) && displayLevel)
	    cout << "col=" << col << endl;

	  col++;
	} // if
    } while (++counter);

  for (int constraintCounter = 0;
       constraintCounter < numICConstraints_total;
       constraintCounter++) {
    model.addConstr(constraints[constraintCounter],
		    GRB_EQUAL,0.0);
  }
  std::cout << "IC Constraints Added" << std::endl;
  std::cout << "Size Constraints vec:" << constraints.size() << std::endl;
  std::cout << "No Constraints:" << numICConstraints_total+numProbabilityVariables << std::endl;
  for (int constraintCounter = 0; 
       constraintCounter < numProbabilityConstraints;
       constraintCounter++) {
    model.addConstr(constraints[numICConstraints_total
				+constraintCounter],GRB_EQUAL,1.0);
  }
  std::cout << "Pr Constraints Added" << std::endl;
  // gurobiObjective=IloMaximize(env);
  // model.add(gurobiObjective);
  
  // cplex=IloCplex(model);

  // gurobiObjective=cplex.getObjective();
  // gurobiObjective.setExpr(objectiveFunctions[1]);

  gurobiObjective = 0;
  gurobiObjective += objectiveFunctions[2];
  model.update();
  // model.setObjective(gurobiObjective,GRB_MINIMIZE);
  // model.update();

} // populate

void BCEGurobiSolver::solve()
{
  map<int,double> solutionEquilibrium;

  model.getEnv().set(GRB_IntParam_Crossover,0);
  model.getEnv().set(GRB_IntParam_Method,2);

  cout << "Display level = " << displayLevel << endl;
  // cplex.setParam(IloCplex::SimDisplay,displayLevel);
  // cplex.setParam(IloCplex::BarDisplay,displayLevel);
  // cplex.setParam(IloCplex::RootAlg, IloCplex::Barrier);
  // cplex.setParam(IloCplex::BarCrossAlg,-1);
  // cplex.setParam(IloCplex::EpOpt,1e-9);

  // cplex.solve();
  // if (displayLevel)
  //   cout << "Objective = " << setprecision(16) << cplex.getObjValue() << endl;
  // GRBLinExpr objective = 0;
  // objective += .5 * getObjectiveFunction(0);
  // objective += .5 * getObjectiveFunction(1);
  // model.setObjective(objective,GRB_MAXIMIZE);

  model.update();
  model.optimize();
  if (displayLevel) {
    for (int obj = 0; obj < objectiveFunctions.size(); obj++)
      cout << "Objective " << obj << " = " << setprecision(16) 
	   << objectiveFunctions[obj].getValue() << endl;
  }

  soln.clearEquilibria();
  bceToMap(solutionEquilibrium);
  soln.addEquilibrium(solutionEquilibrium);
  soln.consolidateEquilibria();
}

void BCEGurobiSolver::mapBoundary()
{
  return;
  // mapBoundary("bndry.dat");
}

void BCEGurobiSolver::mapBoundary(const char * fname)
{
  cout << "mapping objfun1 vs objfun2..." << endl;

  // // Store the defaults so we can restore them at the end.
  // IloInt BarDisplayDefault = cplex.getParam(IloCplex::BarDisplay);
  // IloInt DisplayDefault = cplex.getParam(IloCplex::SimDisplay);
  // IloInt NetDisplayDefault = cplex.getParam(IloCplex::NetDisplay);

  // cplex.setParam(IloCplex::BarDisplay, 0);
  // cplex.setParam(IloCplex::SimDisplay, 0);
  // cplex.setParam(IloCplex::NetDisplay, 0);

  // // Clear the equilibria array.
  // soln.clearEquilibria();

  // // First solve 
  // gurobiObjective.setExpr(objectiveFunctions[boundaryObjectiveIndex1]);
  // cplex.setParam(IloCplex::RootAlg, IloCplex::Primal);
  // // cplex.setParam(IloCplex::BarCrossAlg,IloCplex::Primal);
  // cplex.solve();

  // cout << "Northeast quadrant..." << endl;
  // bndryObjectives[0]=objectiveFunctions[boundaryObjectiveIndex1]; 
  // bndryObjectives[1]=objectiveFunctions[boundaryObjectiveIndex2];
  // mapFrontier(1,1,0);
  // cout << "Northwest quadrant..." << endl;
  // bndryObjectives[0]=objectiveFunctions[boundaryObjectiveIndex2]; 
  // bndryObjectives[1]=-objectiveFunctions[boundaryObjectiveIndex1];
  // mapFrontier(1,-1,1);
  // cout << "Southwest quadrant..." << endl;
  // bndryObjectives[0]=-objectiveFunctions[boundaryObjectiveIndex1]; 
  // bndryObjectives[1]=-objectiveFunctions[boundaryObjectiveIndex2];
  // mapFrontier(-1,-1,0);
  // cout << "Southeast quadrant..." << endl;
  // bndryObjectives[0]=-objectiveFunctions[boundaryObjectiveIndex2]; 
  // bndryObjectives[1]=objectiveFunctions[boundaryObjectiveIndex1];
  // mapFrontier(-1,1,1);

  // // Print the bndry points.
  // ofstream vertexData;
  // list<double>::iterator XIterator;
  // list<double>::iterator YIterator;
  // vertexData.open(fname);
  // for (XIterator=boundaryXs.begin(), YIterator=boundaryYs.begin(); 
  //      XIterator!=boundaryXs.end() && YIterator!=boundaryYs.end(); 
  //      XIterator++, YIterator++)
  //   vertexData << *XIterator << " " << *YIterator << endl;
  // vertexData.close();
  
  // soln.consolidateEquilibria();

  // cplex.setParam(IloCplex::BarDisplay,BarDisplayDefault);
  // cplex.setParam(IloCplex::SimDisplay,DisplayDefault);
  // cplex.setParam(IloCplex::NetDisplay,NetDisplayDefault);
}

void BCEGurobiSolver::mapFrontier(int plusOrMinus1, int plusOrMinus2, bool reversePrint)
{
  return;
  // IloNum objectiveWeight=0, newWeight, alpha;

  // IloCplex::BasisStatusArray basisStatuses(env);
  // map<int,double> currentEquilibrium;
  // IloNumArray reducedCosts1(env);
  // IloNumArray reducedCosts2(env);

  // IloNum objectiveValue1, objectiveValue2;

  // int variableCounter=0;
  // int ItLimDefault = cplex.getParam(IloCplex::ItLim);
  // ItLimDefault = 2100000000;

  // double oldAngleIncrement, minalpha;

  // int displayVal=0;

  // gurobiObjective.setSense(IloObjective::Maximize);
  // cplex.setParam(IloCplex::EpOpt, 1E-9);

  // // cplex.setParam(IloCplex::TiLim,1e+75);

  // int iterationCounter = 0;
  // while ( objectiveWeight < 1e11 && iterationCounter < 1E8 )
  //   {
  //     // solve the new objective function.
  //     // Switch to dual for the initial solve phase. Might take awhile.
  //     gurobiObjective.setExpr(objectiveWeight*bndryObjectives[1] + bndryObjectives[0]);
  //     cplex.setParam(IloCplex::ItLim, ItLimDefault);
  //     cplex.setParam(IloCplex::RootAlg, IloCplex::Primal);
  //     cplex.solve();

  //     // cplex.setParam(IloCplex::TiLim,2);
      
  //     if (cplex.getStatus() != IloAlgorithm::Optimal)
  // 	throw(BCEException(BCEException::MapFrontierNotOptimal));

  //     cplex.setParam(IloCplex::RootAlg, IloCplex::Primal);
  //     cplex.setParam(IloCplex::ItLim, 0);
  //     // cplex.solve();
      
  //     cplex.getBasisStatuses(basisStatuses,variables);
  //     // Print current equilibrium
  //     bceToMap(currentEquilibrium);
  //     soln.addEquilibrium(currentEquilibrium);

  //     // Find the next weight
  //     gurobiObjective.setExpr(bndryObjectives[0]);
  //     cplex.solve();
  //     cplex.getReducedCosts(reducedCosts1,variables);
  //     objectiveValue1 = cplex.getObjValue();

  //     gurobiObjective.setExpr(bndryObjectives[1]);
  //     cplex.solve();
  //     cplex.getReducedCosts(reducedCosts2,variables);
  //     objectiveValue2 = cplex.getObjValue();

  //     // Now find the smallest new weight that is greater than the old weight.
  //     newWeight=-1;
  //     for (variableCounter = 0; variableCounter<variables.getSize(); variableCounter++)
  // 	{
  // 	  if (basisStatuses[variableCounter]!=IloCplex::Basic 
  // 	      && reducedCosts1[variableCounter]<0 
  // 	      && reducedCosts2[variableCounter]>0)
  // 	    {
  // 	      alpha = -reducedCosts1[variableCounter]/reducedCosts2[variableCounter]+1E-8;
  // 	      if (alpha>objectiveWeight && ( alpha<newWeight || newWeight<0 ))
  // 		newWeight = alpha;
  // 	    }
  // 	}
  //     if (newWeight<0 || newWeight > 1e13)
  // 	break; // Could not find a new weight that met criteria. Done.

  //     // Add new element to the list.
  //     if (reversePrint)
  // 	{
  // 	  boundaryXs.push_back(plusOrMinus2*objectiveValue2);
  // 	  boundaryYs.push_back(plusOrMinus1*objectiveValue1);
  // 	}
  //     else
  // 	{
  // 	  boundaryXs.push_back(plusOrMinus1*objectiveValue1);
  // 	  boundaryYs.push_back(plusOrMinus2*objectiveValue2);
  // 	}

  //     basisStatuses.clear();
  //     reducedCosts1.clear();
  //     reducedCosts2.clear();

  //     if (minAngleIncrement>0)
  //     	{
  //     	  oldAngleIncrement = atan2(objectiveWeight,1);
  // 	  // assert(oldAngleIncrement>=0);
  // 	  if (oldAngleIncrement+minAngleIncrement > PI/2-1e-12)
  // 	    break;
  // 	  else
  // 	    minalpha = tan(oldAngleIncrement + minAngleIncrement);
	  
  // 	  if (newWeight < minalpha)
  // 	    newWeight = minalpha;
  //     	}

  //     objectiveWeight = newWeight;

  //     if (!(iterationCounter%10))
  // 	cout << setprecision(3) 
  // 	     << "weight on objective 2 = " << objectiveWeight 
  // 	     << ", iterationCounter = " << iterationCounter << endl;


  //     iterationCounter++;
  //   }

  // basisStatuses.end();
  // reducedCosts1.end();
  // reducedCosts2.end();

  // cout << "Final weight = " << objectiveWeight << ", total iterations = " << iterationCounter << endl;
  // // cout << "# of elements of list: " << boundaryXs.size() << endl;

  // cplex.setParam(IloCplex::ItLim,ItLimDefault);
} // mapFrontier

void BCEGurobiSolver::getSolution(BCESolution & output)
{
  output=soln;
}

void BCEGurobiSolver::bceToMap(map<int,double> & distribution)
{
  int variable;
  list<int>::const_iterator it;
  vector<double> gurobiValues(numProbabilityVariables,0.0);
  for(int var = 0; var<numProbabilityVariables; var++)
    gurobiValues[var] = variables[var].get(GRB_DoubleAttr_X);

  // cplex.getValues(cplexValues,variables);
  distribution = map<int,double>();

  // Populate the distribution vector.
  for (variable=0, it = nonZeroVariableLocations.begin(); 
       variable<numProbabilityVariables; 
       variable++, ++it)
    {
      if (abs(gurobiValues[variable])>1e-12)
  	distribution.insert(pair<int,double>(*it,
  					     gurobiValues[variable]));
    }

  // cplexValues.end();
}


void BCEGurobiSolver::indexToTypeActionDeviation(int index, int player, int &type, int &action, int &deviation)
{
  assert(index>=0);
  assert(index<game->getNumTypes()[player]*
	 game->getNumActions()[player]*game->getNumActions()[player]);

  deviation=index%game->getNumActions()[player];
  index-=deviation; index/=game->getNumActions()[player];
  action=index%game->getNumActions()[player];
  index-=action; index/=game->getNumActions()[player];
  type=index;

  assert(type>=0);
  assert(action>=0);
  assert(deviation>=0);
  assert(type<game->getNumTypes()[player]);
  assert(action<game->getNumActions()[player]);
  assert(deviation<game->getNumActions()[player]);
}

