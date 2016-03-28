#include "bcecomparator.hpp"

bool BCEComparator::operator() (const vector<double> & eq0, 
				const vector<double> & eq1)
{
  double absoluteZero=1E-12;
  int playerCounter;
  vector<double> diff0(2,0.0), diff1(2,0.0);

  switch (currentMode)
    {
    case ANGLE:
      for (playerCounter=0; playerCounter<2; playerCounter++)
	{
	  diff0[playerCounter] = eq0[obj[playerCounter]] - origin[playerCounter];
	  diff1[playerCounter] = eq1[obj[playerCounter]] - origin[playerCounter];
	}

      return ( (cosine(diff0) > cosine(diff1)+absoluteZero)
	       || (abs(cosine(diff0)-cosine(diff1))<absoluteZero
		   && norm(diff0)+absoluteZero<norm(diff1) ) );

    case EQUALITY:
      if (abs(eq0[obj[0]]-eq1[obj[0]])>absoluteZero
	  || abs(eq0[obj[1]]-eq1[obj[1]])>absoluteZero)
	return false;
      return true;
    }
}

double BCEComparator::operator() (const vector<double> & eq0, 
		   const vector<double> & eq1, 
		   const vector<double> & eq2)
{
  return( (eq1[obj[0]]-eq0[obj[0]])*(eq2[obj[1]]-eq0[obj[1]]) 
	  - (eq1[obj[1]]-eq0[obj[1]])*(eq2[obj[0]]-eq0[obj[0]]) );
}

double BCEComparator::cosine(const vector<double> & eq)
{
  double n = norm(eq);
  if (n)
    return (eq[obj[0]]/n);
  else
    return 1.0;
}

double BCEComparator::norm(const vector<double> & eq)
{
  return (sqrt(eq[obj[0]]*eq[obj[0]]+eq[obj[1]]*eq[obj[1]]));
}

void BCEComparator::setObjectives(const vector<int> &_obj)
{
  obj = _obj;
}

void BCEComparator::setOrigin(const vector<double> & input)
{
  if (input.size() != 2)
    throw(BCEException(BCEException::BadArgumentBCEComparator));
  assert(input.size()==2);  
  
  origin=input;
}

void BCEComparator::setMode(Mode newMode)
{
  currentMode = newMode;
}


