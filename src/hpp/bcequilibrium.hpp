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
    very few functions, except for serialization and copying.

    \ingroup src
*/
class BCEquilibrium
{
public:
  //! The BCE
  /*! BCE are represented as maps from linear indices to doubles. The
      linear index corresponding to a particular state/type/action
      tuple can be easily calculated using BCECounter. */
  map<int,double> distribution;
  //! The multipliers associated with the incentive constraints of the BCE
  /*! Multipliers are represented as maps from linear indices to doubles. The
      linear index corresponding to a particular type/action/deviation. */
  map<int,double> multipliers;
  //! Unused key for sorting BCE
  double dist_key;
  //! Unused key for sorting BCE multipliers
  double mult_key;

  public:
  //! Default constructor
  BCEquilibrium () {}
  //! Constructor
  /*! Initializes the equilibrium with the given map and a key of
      0.0. */
  BCEquilibrium (const map<int,double> & dist, const map<int,double> & mult):
    distribution(dist), dist_key(0.0), multipliers(mult), mult_key(0.0)
  {}
  //! Destructor
  ~BCEquilibrium() {}

  //! Assignment operator
  BCEquilibrium& operator=(const BCEquilibrium & rhs)
  {
    if (this!=&rhs)
      {
	this->distribution=rhs.distribution;
	this->dist_key=rhs.dist_key;
    this->multipliers=rhs.multipliers;
	this->mult_key=rhs.mult_key;
      }
    return *this;
  }

  //! Serialization routine.
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & distribution & multipliers & dist_key & mult_key;
  }

  friend class boost::serialization::access;
  friend class BCEData;
}; // BCEquilibrium


#endif
