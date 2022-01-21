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

#ifndef BCEEXCEPTION_HPP
#define BCEEXCEPTION_HPP

//! Exception class for BCESolve
/*! This class derives from std::exception and implements special
    error codes. 

    \ingroup src
*/
class BCEException: public std::exception
{
public:
  //! Type of error
  enum ErrorType
    {
      OutOfBounds, /*!< Index out of bounds. */
      ConditionFailed, /*!< A condition was not met. */
      BCECounterConditionFailed, /*!< A condition was not met in BCECounter. */
      BCEAbstractGameConditionFailed, /*!< Condition not met in BCEAbstractGame. */
      NoEquilibria, /*!< No equilibria. */
      /*! Gurobi was not able to calculate an optimum during the
          BCESolver::mapBoundary routine. */
      MapFrontierNotOptimal, 
      FailedOpen, /*!< Could not open file. */
      /*! Argument was not a probability distribution, probably
          because some variable was negative or probabilities did not
          sum to one. */
      NotProbDistr, 
      BadArgument, /*!< Generic error code for a bad argument. */
      BadArgumentBCEComparator, /*!< Bad argument in BCEComparator */
      BadArgumentBCEUtilities, /*!< Bad argument in BCEUtilities */
      BadArgumentBCEAbstractGame, /*!< Bad argument in BCEAbstractGame */
      InvalidParameterName, /*!< Unrecognized parameter name. */
      InvalidParameterValue, /*!< Parameter value was invalid. */
      ICConstraintViolated, /*!< Incentive constraint violated. */
      WrongDistnSize, /*!< Size of Distn doesn't reflect the number of actions. */
      WrongWeightSize, /*! Size of Weights given in BCESolver::solve() doesn't match number of objs. */
      AlreadyPopulated /*!< Solver is already populated */
    };

  //! Gives the error code for this BCEException.
  ErrorType errorType;

  //! Constructor
  /*! Initializes a new exception with the given ErrorType. */
  BCEException(ErrorType errorTypeArg): errorType(errorTypeArg)
  { }

  //! Reimplements std::exception::what().
  const char* what() const throw()
  {
    return "BCEException was thrown";
  }

  //! Returns an error message.
  /*! Has not been fully implemented. */
  string getMessage()
  {
    switch (errorType)
      {
      case OutOfBounds: 
	return "An index was out of the provided bounds.";
      case ConditionFailed: 
	return "A condition we imposed failed.";
      case BCECounterConditionFailed:
	return "A condition we imposed in bcecounter.cpp failed.";
      case BCEAbstractGameConditionFailed:
	return "A condition we imposed in bceabstractgame.cpp failed.";
      case NoEquilibria: 
	return "There are no equilibria.";
      case MapFrontierNotOptimal: 
	return "Map frontier is not optimal.";
      case FailedOpen: 
	return "The file failed to open.";
      case NotProbDistr: 
	return "Probabilities are misbehaving. At least one is negative or greater than 1, or they do not sum to 1.";
      case BadArgument: 
	return "A bad argument was received.";
      case BadArgumentBCEComparator:
	return "A bad argument was received in bcecomparator.cpp.";
      case BadArgumentBCEUtilities:
	return "A bad argument was received in bceutilities.cpp.";
      case BadArgumentBCEAbstractGame:
	return "A bad argument was received in bceabstractgame.cpp.";
      case InvalidParameterName: 
	return "Parameter name wasn't recognized.";
      case InvalidParameterValue: 
	return "A parameter value is invalid.";
      case ICConstraintViolated:
	return "An incentive constraint was violated.";
      case WrongDistnSize:
	return "Distribution given in solution data does not reflect the correct number of actions.";
      case WrongWeightSize: 
	return "Size of the weight vector supplied as an argument to BCESolver::solve(weights) does not match the number of objectives. Check your specification in the example file.";
      case AlreadyPopulated:
	return "Solver is already populated.";      
      } // switch
  }
};

#endif
