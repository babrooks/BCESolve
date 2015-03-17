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

//! Class for storing a BCE
/*! Class for storing equilibria from BCESolver. The class performs
    very few functions, except for serialization and copying. */
class BCEquilibrium
{
public:
  //! The BCE
  /*! BCE are represented as maps from linear indices to doubles. The
      linear index corresponding to a particular state/type/action
      tuple can be easily calculated using BCECounter. */
  map<int,double> distribution;
  //! Unused key for sorting BCE
  double key;

public:
  //! Default constructor
  BCEquilibrium () {}
  //! Constructor
  /*! Initializes the equilibrium with the given map and a key of
      0.0. */
  BCEquilibrium (const map<int,double> & data):
    distribution(data), key(0.0) 
  {}
  //! Destructor
  ~BCEquilibrium() {}

  //! Assignment operator
  BCEquilibrium& operator=(const BCEquilibrium & rhs)
  {
    if (this!=&rhs)
      {
	this->distribution=rhs.distribution;
	this->key=rhs.key;
      }
    return *this;
  }

  //! Serialization routine.
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & distribution & key;
  }

  friend class boost::serialization::access;
  friend class BCEData;
}; // BCEquilibrium


#endif
