#ifndef BCECOMPARATOR_HPP
#define BCECOMPARATOR_HPP

#include "bcecommon.hpp"
#include "bceexception.hpp"

// This class is a function operator for sorting equilibria in a
// BCEData object.
class BCEComparator
{
public:
  enum Mode
    {
      ANGLE, EQUALITY
    };
private:
  vector<int> obj;
  vector<double> origin;
  Mode currentMode;

public:
  BCEComparator(): obj(2,0) {};
  bool operator() (const vector<double> &, 
		   const vector<double> &);
  double operator() (const vector<double> &, 
		     const vector<double> &, 
		     const vector<double> &);
  void setObjectives(const vector<int> &);
  void setOrigin(const vector<double> &);
  void setMode(Mode);
  
  double cosine(const vector<double> &);
  double norm(const vector<double> &);
}; // BCEComparator

#endif
