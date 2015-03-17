#ifndef BCEPOINT_HPP
#define BCEPOINT_HPP

#include "bcecommon.hpp"
#include "bceexception.hpp"

//! Class for two dimensional vectors.
class BCEPoint
{
public: 
  vector<double> data;
  
  BCEPoint():data(2,0.0) {};
  BCEPoint(vector<double> newData):data(newData) {};
  BCEPoint(double p1, double p2):data(2,0.0) {data[0]=p1; data[1]=p2;};
  ~BCEPoint() {};
  
  BCEPoint &operator=(const BCEPoint &rhs) 
  {
    if (this!=&rhs)
      this->data = rhs.data;

    return *this;
  }

  int operator==(const BCEPoint &rhs)
  {
    if ( this->data != rhs.data ) return 0;
    return 1;
  }

  int operator<(const BCEPoint & rhs)
  {
    return 0;
  }

  friend ostream& operator<<(ostream& output, const BCEPoint &rhs)
  {
    output << rhs.data[0] << " " << rhs.data[1] << endl;

    return output;
  }
}; // BCEPoint

#endif
