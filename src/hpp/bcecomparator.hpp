#ifndef BCEDATA_HPP
#define BCEDATA_HPP

#include "bcecommon.hpp"
#include "bceexception.hpp"
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>

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
