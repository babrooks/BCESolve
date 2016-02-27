// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks, Robert J. Minton
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
      NoEquilibria, /*!< No equilibria. */
      /*! CPLEX was not able to calculate an optimum during the
          BCESolver::mapBoundary routine. */
      MapFrontierNotOptimal, 
      FailedOpen, /*!< Could not open file. */
      /*! Argument was not a probability distribution, probably
          because some variable was negative or probabilities did not
          sum to one. */
      NotProbDistr, 
      BadArgument, /*!< Generic error code for a bad argument. */
      InvalidParameterName, /*!< Unrecognized parameter name. */
      InvalidParameterValue /*!< Parameter value was invalid. */
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

  // //! Returns an error message.
  // /*! Has not been fully implemented. */
  // string getMessage()
  // {
  //   switch (errorType)
  //     {
  //     case OutOfBounds:
  // 	break;
  //     case ConditionFailed:
  // 	break;
  //     case NoEquilibria:
  // 	break;
  //     case MapFrontierNotOptimal:
  // 	break;
  //     case FailedOpen:
  // 	break;
  //     case NotProbDistr:
  // 	break;
  //     case BadArgument:
  // 	break;
  //     case InvalidParameterName:
  // 	break;
  //     case InvalidParameterValue:
  // 	break;
  //     } // switch
  // }
};

#endif
