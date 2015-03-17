#include "mex.h"
#include <cstring>
#include "bcedata.hpp"

using namespace std;

BCEData data;

void mexFunction(int nlhs, mxArray *plhs[],
		 int nrhs, const mxArray *prhs[]) {
    
  // Variable declarations
  char * optionStr, * inputFileName;
  int vectorCounter, rhsCounter,
    variableCounter, equilibriumCounter,
    deviationCounter, objectiveCounter, 
    rowCounter, playerCounter, newCondition, conditionCounter;
  double *outputPtr, * inputPtr;
  bool * logicalPtr;
        
  int player, action, type, rows, cols;
  vector<int> actions, types;
  vector<double> val;
  vector< vector<double> > objectiveValues;
    
  vector<int> stateConditions;
  vector< vector<int> > actionConditions(2), typeConditions(2);
  bool stateMarginal;
  vector<bool> actionMarginal, typeMarginal;
    
  vector<double> marginalDistribution;
    
  // Check inputs
  if (nrhs<1)
    mexErrMsgTxt("At least one input argument required.");
  if (!mxIsChar(prhs[0]))
    mexErrMsgTxt("First argument must be a character array.");

  // First argument is always an option string.
  optionStr = mxArrayToString(prhs[0]);
    
  if (!strcmp(optionStr, "Load")) {
    // bcemex('Load',filename)
    if (nrhs<2)
      mexErrMsgTxt("Must provide input file name.");
    if (!mxIsChar(prhs[1]))
      mexErrMsgTxt("Input file name must be a character array.");
        
    inputFileName = mxArrayToString(prhs[1]);
        
    try
      {
	BCEData::load(data,inputFileName);
	data.currentEquilibrium=0;
      }
    catch (BCEException & e)
      {
	if (e.errorType == BCEException::FailedOpen)
	  mexErrMsgTxt("Could not open file");
      }
  } // Load
  else if (!strcmp(optionStr,"GetNumActions"))
    {
      // numActions = bcemex('NumActions')
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output.");
      if (!data.numPlayers)
	mexErrMsgTxt("No data loaded.");
        
      plhs[0] = mxCreateDoubleMatrix((mwSize) data.numPlayers,1,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
      for (vectorCounter = 0; vectorCounter<data.numPlayers; vectorCounter++)
	outputPtr[vectorCounter] = data.numActions[vectorCounter];
    } // GetNumActions
  else if (!strcmp(optionStr,"GetNumTypes"))
    {
      // numTypes = bcemex('NumTypes')
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output.");
      if (!data.numPlayers)
	mexErrMsgTxt("No data loaded.");
        
      plhs[0] = mxCreateDoubleMatrix((mwSize) data.numPlayers,1,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
      for (vectorCounter = 0; vectorCounter<data.numPlayers; vectorCounter++)
	outputPtr[vectorCounter] = data.numTypes[vectorCounter];
    } // GetNumTypes
  else if (!strcmp(optionStr,"GetNumStates"))
    {
      // numStates = bcemex('NumStates')
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output.");
      if (!data.numPlayers)
	mexErrMsgTxt("No data loaded.");
        
      plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
      outputPtr[0] = data.numStates;
    } // GetNumStates
  else if (!strcmp(optionStr,"GetNumValues"))
    {
      // numValues = bcemex('NumValues')
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output.");
      if (!data.numPlayers)
	mexErrMsgTxt("No data loaded.");
      if (!data.isPrivateValues)
	plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
      else
        {
	  plhs[0] = mxCreateDoubleMatrix((mwSize) data.numPlayers,1,mxREAL);
	  outputPtr = mxGetPr(plhs[0]);
	  for (vectorCounter = 0; vectorCounter<data.numPlayers; vectorCounter++)
	    outputPtr[vectorCounter] = data.numValues[vectorCounter];
        }
    } // GetNumValues
  else if (!strcmp(optionStr,"GetNumEquilibria"))
    {
      // numStates = bcemex('NumStates')
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output.");
      if (!data.numPlayers)
	mexErrMsgTxt("No data loaded.");
        
      plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
      outputPtr[0] = data.equilibria.size();
    } // GetNumEquilibria
  else if (!strcmp(optionStr,"GetNumEquilibria"))
    { 
      // numValues = bcemex('NumValues')
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output.");
      if (!data.numPlayers)
	mexErrMsgTxt("No data loaded.");
        
      plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
      outputPtr[0] = data.equilibria.size();
    } // GetNumEquilibria
  else if (!strcmp(optionStr,"GetObjectives"))
    {
      // Verify correct number of outputs
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output argument.");
      plhs[0]=mxCreateDoubleMatrix((mwSize) data.numActions_total*data.numStates,
				   (mwSize) data.numObjectives,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
        
      rowCounter=0;
      for (objectiveCounter=0; objectiveCounter<data.numObjectives; objectiveCounter++)
        {
	  for (variableCounter=0; variableCounter<data.numActions_total*data.numStates; variableCounter++)
            {
	      outputPtr[rowCounter]=data.objectives[objectiveCounter][variableCounter];
                
	      rowCounter++;
            }
        }
    } // GetObjectives
  else if (!strcmp(optionStr,"GetPrior"))
    {
      // Verify correct number of outputs
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output argument.");
      plhs[0]=mxCreateDoubleMatrix((mwSize) data.numTypes_total*data.numStates,
				   1,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
        
      rowCounter=0;
      for (variableCounter=0; variableCounter<data.numTypes_total*data.numStates; variableCounter++)
        {
	  outputPtr[rowCounter]=data.prior[variableCounter];
            
	  rowCounter++;
        }
    } // GetPrior

  else if (!strcmp(optionStr,"GetExpectedObjectives"))
    {
      // Verify correct number of outputs
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output argument.");
      plhs[0]=mxCreateDoubleMatrix((mwSize) data.equilibria.size(),
				   (mwSize) data.numObjectives,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
        
      data.getExpectedObjectives(objectiveValues);
        
      rowCounter=0;
      for (objectiveCounter=0; objectiveCounter<data.numObjectives; objectiveCounter++)
        {
	  for (equilibriumCounter=0; equilibriumCounter<data.equilibria.size(); equilibriumCounter++)
            {
	      outputPtr[rowCounter]=objectiveValues[equilibriumCounter][objectiveCounter];
                
	      rowCounter++;
            }
        }
    } // GetExpectedObjectives
  else if (!strcmp(optionStr,"GetDistribution"))
    {
      // Collect additional inputs
      if (nrhs!=2)
	mexErrMsgTxt("Must have two input arguments.");
      // Check inputs
      rows = mxGetM(prhs[1]); cols = mxGetM(prhs[1]);
      if (rows!=1 || cols!=1)
	mexErrMsgTxt("Input arguments must be scalars.");
      if (!mxIsDouble(prhs[1]))
	mexErrMsgTxt("Inputs are not doubles.");
      inputPtr = mxGetPr(prhs[1]);
      equilibriumCounter = static_cast<int>(inputPtr[0]);
        
      if (equilibriumCounter<0 || equilibriumCounter>=data.equilibria.size())
	mexErrMsgTxt("Equilibrium must be between 0 and the number of equilibria.");
        
      // Verify correct number of outputs
      if (nlhs!=1)
	mexErrMsgTxt("Must have one output argument.");
        
      if (data.equilibria[equilibriumCounter].distribution.size()!=data.numStates*data.numTypes_total*data.numActions_total)
        {
	  mexPrintf("distr size = %d, nstates = %d, ntypes_total = %d, nactions_total = %d\n",
                    data.equilibria[equilibriumCounter].distribution.size(),data.numStates,data.numTypes_total,data.numActions_total);
	  mexErrMsgTxt("Distribution is not correct size.");
        }
      plhs[0]=mxCreateDoubleMatrix((mwSize) data.numStates*data.numTypes_total,(mwSize) data.numActions_total,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
        
      for (variableCounter=0; variableCounter<data.equilibria[equilibriumCounter].distribution.size(); variableCounter++)
	outputPtr[variableCounter]=data.equilibria[equilibriumCounter].distribution[variableCounter];
    } // GetDistribution
  else if (!strcmp(optionStr,"GetConditionalMarginal"))
    {
      // Calculates a conditional marginal distribution.
        
      // Preliminary check of inputs
      if (nrhs!=9)
	mexErrMsgTxt("Must have nine input arguments.");
      if (nlhs!=2)
	mexErrMsgTxt("Must have two output arguments.");
      for (rhsCounter = 1; rhsCounter<6; rhsCounter++)
        {
	  if (!mxIsNumeric(prhs[rhsCounter]))
	    mexErrMsgTxt("Inputs are not numeric matrices.");
        }
      if (!mxIsLogicalScalar(prhs[6]))
	mexErrMsgTxt("State marginal must be a logical scalar.");
      for (rhsCounter = 7; rhsCounter<9; rhsCounter++)
        {
	  if (!mxIsLogical(prhs[rhsCounter]) || mxGetNumberOfElements(prhs[rhsCounter])!=data.numPlayers)
	    mexErrMsgTxt("Action and type marginals must be logical arrays with numPlayers elements.");
        }
        
      rhsCounter=1;
      // State conditions
      if (!mxIsEmpty(prhs[rhsCounter]))
        {
          inputPtr = mxGetPr(prhs[rhsCounter]);
          stateConditions.resize(mxGetNumberOfElements(prhs[rhsCounter]));
          for (conditionCounter=0; conditionCounter<mxGetNumberOfElements(prhs[rhsCounter]); conditionCounter++)
              stateConditions[conditionCounter]=inputPtr[conditionCounter];
      }
      rhsCounter++;
        
      // Action conditions
      for (playerCounter=0; playerCounter<data.numPlayers; playerCounter++)
        {
	  if (!mxIsEmpty(prhs[rhsCounter]))
            {
	      inputPtr = mxGetPr(prhs[rhsCounter]);
	      actionConditions[playerCounter].resize(mxGetNumberOfElements(prhs[rhsCounter]));
	      for (conditionCounter=0; conditionCounter<mxGetNumberOfElements(prhs[rhsCounter]); conditionCounter++)
                {
		  newCondition = static_cast<int>(inputPtr[conditionCounter]);
		  if (newCondition>=data.numActions[playerCounter] || newCondition<0)
		    mexErrMsgTxt("Conditions must be at least 0 and less than the number of actions.");
		  actionConditions[playerCounter][conditionCounter]=newCondition;
                }
            }
	  rhsCounter++;
        }
        
      // Type conditions
      for (playerCounter=0; playerCounter<data.numPlayers; playerCounter++)
        {
	  if (!mxIsEmpty(prhs[rhsCounter]))
            {
	      inputPtr = mxGetPr(prhs[rhsCounter]);
	      typeConditions[playerCounter].resize(mxGetNumberOfElements(prhs[rhsCounter]));
	      for (conditionCounter=0; conditionCounter<mxGetNumberOfElements(prhs[rhsCounter]); conditionCounter++)
                {
		  newCondition = static_cast<int>(inputPtr[conditionCounter]);
		  if (newCondition>=data.numTypes[playerCounter] || newCondition<0)
		    mexErrMsgTxt("Conditions must be at least 0 and less than the number of types.");
		  typeConditions[playerCounter][conditionCounter]=newCondition;
                }
            }
	  rhsCounter++;
        }
        
      // State marginal
      logicalPtr = mxGetLogicals(prhs[6]);
      stateMarginal = logicalPtr[0];
      // Action marginal
      logicalPtr = mxGetLogicals(prhs[7]);
      actionMarginal.resize(data.numPlayers);
      for (playerCounter=0; playerCounter<data.numPlayers; playerCounter++)
	actionMarginal[playerCounter]=static_cast<bool>(logicalPtr[playerCounter]);
      // Type marginal
      logicalPtr = mxGetLogicals(prhs[8]);
      typeMarginal.resize(data.numPlayers);
      for (playerCounter=0; playerCounter<data.numPlayers; playerCounter++)
	typeMarginal[playerCounter]=static_cast<bool>(logicalPtr[playerCounter]);
        
      // Verify correct number of outputs
      if (nlhs!=2)
          mexErrMsgTxt("Must have two output arguments.");

      plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
      outputPtr = mxGetPr(plhs[0]);
      outputPtr[0] = data.getConditionalMarginal(stateConditions,actionConditions,typeConditions,
				  stateMarginal,actionMarginal,typeMarginal,
				  marginalDistribution);
        
      plhs[1] = mxCreateDoubleMatrix((mwSize) marginalDistribution.size(),1,mxREAL);
      outputPtr = mxGetPr(plhs[1]);

      for (variableCounter=0; variableCounter<marginalDistribution.size(); variableCounter++)
          outputPtr[variableCounter]=marginalDistribution[variableCounter];
      
      
    } // GetConditional
  else if (!strcmp(optionStr,"GetDeviationObjectives"))
    {
      // [devPayoffs ,prob] = bcemex('Deviations',player,type,action)
        double prob;
		
      // Collect additional inputs
	  if (nrhs<4)
		  mexErrMsgTxt("Must have at least four input arguments.");
      // Check inputs
      for (rhsCounter = 1; rhsCounter<4; rhsCounter++)
        {
	  rows = mxGetM(prhs[rhsCounter]); cols = mxGetM(prhs[rhsCounter]);
	  if (rows!=1 || cols!=1)
	    mexErrMsgTxt("Input arguments must be scalars.");
	  if (!mxIsDouble(prhs[rhsCounter]))
	    mexErrMsgTxt("Inputs are not doubles.");
        }
      inputPtr = mxGetPr(prhs[1]);
      player = static_cast<int>(inputPtr[0]);
      inputPtr = mxGetPr(prhs[2]);
      type = static_cast<int>(inputPtr[0]);
      inputPtr = mxGetPr(prhs[3]);
      action = static_cast<int>(inputPtr[0]);
        
	  // Check the inputs
	  if (player<0 || player>=data.numPlayers)
		  mexErrMsgTxt("Player must be at least 0 less than the number of players.");
	  if (type<0 || type>=data.numTypes[player])
		  mexErrMsgTxt("Type must be at least 0 and less than the number of types for player.");
	  if (action<0 || action>=data.numActions[player])
		  mexErrMsgTxt("Action must be at least 0 and less than the number of actions for player.");
	  
	  // Get the deviation payoffs
	  prob = data.getDeviationObjectives(player,action,type,objectiveValues);
	  
	  // Verify correct number of outputs
	  if (nlhs!=2)
		  mexErrMsgTxt("Must have two output arguments.");
	  plhs[0]=mxCreateDoubleMatrix((mwSize) data.numActions[player],
			  (mwSize) data.numObjectives,mxREAL);
	  outputPtr = mxGetPr(plhs[0]);
	  
	  rowCounter = 0;
	  for (objectiveCounter=0; objectiveCounter<data.numObjectives; objectiveCounter++)
	  {
		  for (deviationCounter=0; deviationCounter<data.numActions[player]; deviationCounter++)
		  {
			  outputPtr[rowCounter]=objectiveValues[objectiveCounter][deviationCounter];
			  
			  rowCounter++;
		  }
	  }
	  
	  plhs[1] = mxCreateDoubleMatrix((mwSize) 1,(mwSize) 1,mxREAL);
	  outputPtr = mxGetPr(plhs[1]);
	  outputPtr[0] = prob;
    } // GetDeviationObjectives
  else if (!strcmp(optionStr,"SetCurrentEquilibrium"))
    {
      if (nrhs<2)
	mexErrMsgTxt("Must have at least two input arguments.");
      // Check inputs
      rows = mxGetM(prhs[1]); cols = mxGetM(prhs[1]);
      if (rows!=1 || cols!=1)
	mexErrMsgTxt("Input arguments must be scalars.");
      if (!mxIsDouble(prhs[1]))
	mexErrMsgTxt("Inputs are not doubles.");
      inputPtr = mxGetPr(prhs[1]);
      equilibriumCounter = static_cast<int>(inputPtr[0]);
        
      if (equilibriumCounter<0 || equilibriumCounter>=data.equilibria.size())
	mexErrMsgTxt("Equilibrium must be between 0 and the number of equilibria.");
        
      // Verify correct number of outputs
      if (nlhs)
	mexErrMsgTxt("Must have no output arguments.");
        
      data.setCurrentEquilibrium(equilibriumCounter);
    } // SetCurrentEquilibrium
  else
    mexErrMsgTxt("Not a valid option string.\n");
    
  mxFree(optionStr);
}
