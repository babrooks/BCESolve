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

  //! Returns an error message.
  /*! Has not been fully implemented. */
  string getMessage()
  {
    switch (errorType)
      {
      case OutOfBounds:
	break;
      case ConditionFailed:
	break;
      case NoEquilibria:
	break;
      case MapFrontierNotOptimal:
	break;
      case FailedOpen:
	break;
      case NotProbDistr:
	break;
      case BadArgument:
	break;
      case InvalidParameterName:
	break;
      case InvalidParameterValue:
	break;
      } // switch
  }
};

#endif
