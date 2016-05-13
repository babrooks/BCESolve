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
      WrongWeightSize /*! Size of Weights given in BCESolver::solve() doesn't match number of objs. */
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
      	break;
      case ConditionFailed: 
	return "A condition we imposed failed.";
	break;
      case BCECounterConditionFailed:
	return "A condition we imposed in bcecounter.cpp failed.";
	break;
      case BCEAbstractGameConditionFailed:
	return "A condition we imposed in bceabstractgame.cpp failed.";
	break;
      case NoEquilibria: 
	return "There are no equilibria.";
	break;
      case MapFrontierNotOptimal: 
	return "Map frontier is not optimal.";
	break;
      case FailedOpen: 
	return "The file failed to open.";
	break;
      case NotProbDistr: 
	return "Probabilities are misbehaving. At least one is negative or greater than 1, or they do not sum to 1.";
	break;
      case BadArgument: 
	return "A bad argument was received.";
	break;
      case BadArgumentBCEComparator:
	return "A bad argument was received in bcecomparator.cpp.";
	break;
      case BadArgumentBCEUtilities:
	return "A bad argument was received in bceutilities.cpp.";
	break;
      case BadArgumentBCEAbstractGame:
	return "A bad argument was received in bceabstractgame.cpp.";
	break;
      case InvalidParameterName: 
	return "Parameter name wasn't recognized.";
	break;
      case InvalidParameterValue: 
	return "A parameter value is invalid.";
	break;
      case ICConstraintViolated:
	return "An incentive constraint was violated.";
	break;
      case WrongDistnSize:
	return "Distribution given in solution data does not reflect the correct number of actions.";
	break;
      case WrongWeightSize: {
	return "Size of the weight vector supplied as an argument to BCESolver::solve(weights) does not match the number of objectives. Check your specification in the example file.";
      }
	break;
      } // switch
  }
};

#endif
