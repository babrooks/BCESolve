#ifndef BCEEXCEPTION_HPP
#define BCEEXCEPTION_HPP

// Ben's exception class!
class BCEException: public std::exception
{
public:
  enum ErrorType
    {
      OutOfBounds,
      ConditionFailed,
      NoEquilibria,
      MapFrontierNotOptimal,
      FailedOpen,
      NotProbDistr,
      BadArgument,
      InvalidParameterName,
      InvalidParameterValue
    };

  ErrorType errorType;

  BCEException(ErrorType errorTypeArg): errorType(errorTypeArg)
  { }

  const char* what() const throw()
  {
    return "BCEException was thrown";
  }

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
