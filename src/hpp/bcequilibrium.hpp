#ifndef BCEQUILIBRIUM_HPP
#define BCEQUILIBRIUM_HPP

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

// Class for storing equilibria from PSM. The only reason we don't
// just treat this as a vector<double> is that we might want to add
// functionality later, e.g. for sorting.
class BCEquilibrium
{
public:
  map<int,double> distribution;
  double key;

public:
  BCEquilibrium () {}
  BCEquilibrium (const map<int,double> & data):
    distribution(data), key(0.0) 
  {}

  ~BCEquilibrium() {}

  BCEquilibrium& operator=(const BCEquilibrium & rhs)
  {
    if (this!=&rhs)
      {
	this->distribution=rhs.distribution;
	this->key=rhs.key;
      }
    return *this;
  }

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & distribution & key;
  }

  friend class boost::serialization::access;
  friend class BCEData;
}; // BCEquilibrium


#endif
