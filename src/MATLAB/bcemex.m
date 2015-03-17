function bcemex
% outputs = bcemex(mode,inputs)
% 
%   Interface to BCE C++ library.
%   BAB
%   5-17-2012
% 
%   Options are:
%       bcemex('Load',filename)
% 
%           Loads the BCE data contained in filename.
%    
%       numActions = bcemex('GetNumActions')
%    
%       numTypes = bcemex('GetNumTypes')
%    
%       numStates = bcemex('GetNumStates')
%    
%       numStates = bcemex('GetNumValues')
%    
%       numEquilibria = bcemex('GetNumEquilibria')
%    
%       [deviationObjectives,prob] = bcemex('GetDeviationObjectives',
%                                    player,type,action)
% 
%           Obtains the expected objective function values for the current
%           equilibrium if player with the given type and recommended to
%           play action deviates to another action.
%    
%       condMargDistr = bcemex('GetConditionalMarginal',
%                              stateConditions, 
%                              actionConditions0, actionConditions1, 
%                              typeConditions0, typeConditions1,
%                              stateMarginal, actionMarginal, typeMarginal)
% 
%           Calculates a conditional marginal distribution from the current
%           equilibrium. State must be an element of stateConditions,
%           player 0's action must be an element of actionConditions0, etc.
%           If a condition vector is empty, no restriction is placed on the
%           corresponding variable.
% 
%           The distribution will be projected onto the variables with a
%           logical true, e.g. if stateMarginal is true, 
%           actionMarginal = [true false], and typeMarginal = [false,false],
%           then the marginal distribution over the state and player 1's action
%           will be computed.
%    
%       bcemex('SetCurrentEquilibrium',equilibriumIndex)
% 
%           Sets the current equilibrium.

end % bcemex