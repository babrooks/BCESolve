#ifndef BCEDISTR_HPP
#define BCEDISTR_HPP

#include <cmath>
#include <vector>

using namespace std;

//! Utility class for probability distributions
/*! This is a pure virtual base class for describing bivariate joint
    distributions. You can inherit from this class and reimplement the
    CDF method. The CDF method should return the joint CDF. */
class BCEDistr
{
public:
  //! The joint CDF of the distribution.
  virtual double CDF(double v0, double v1) const = 0;

  //! Discretized PDF for the BCEDistr
  /*! Calculates the mass in the rectangle [v0-incr0,v0] x
    [v1-incr1,v1]. */
  double PDF(double v0, double incr0,
	     double v1, double incr1) const
  {
    return (CDF(v0,v1) + CDF(v0-incr0,v1-incr1)
	    - CDF(v0-incr0,v1) - CDF(v0,v1-incr1));
  }

  //! Discretized PDF for the BCEDistr
  /*! Calculates the mass in the rectangle [v0-incr,v0] x
    [v1-incr,v1]. */
  double PDF(double v0, double v1, double incr) const
  {
    return PDF(v0,incr,v1,incr);
  }
};

//! Weighted sum of distributions
/*! This class creates a new distribution out of a convex combination
    of distributions. It can be used recursively, i.e., to create
    convex combinations of convex combinations. */
class BCEDistrArray : public BCEDistr
{
public:
  vector<BCEDistr*> distributions;
  vector<double> weights;

  //! Default constructor
  /*! Creates an empty BCEDistrArray. */
  BCEDistrArray() {}
  
  //! Destructor
  /*! Deletes all of the member distributions. */
  ~BCEDistrArray()
  {
    vector<BCEDistr*>::iterator distr;
    for (distr = distributions.begin();
	 distr != distributions.end();
	 distr++)
      delete *distr;
  }

  //! Add a new distribution to the array
  /*! This method appends a new distribution to the array with the
      corresponding weight. */
  bool push_back(BCEDistr* newDistr, double newWeight)
  {
    // Avoid recursive catastrophes!
    if (this != newDistr)
      {
	distributions.push_back(newDistr);
	weights.push_back(newWeight);
	
	return false;
      }
    return true;
  }

  //! Deletes all of the distributions in the array.
  void clear()
  {
    vector<BCEDistr*>::iterator distr;
    for (distr = distributions.begin();
	 distr != distributions.end();
	 distr++)
      delete *distr;
    distributions.clear();
    weights.clear();
  }

  //! Implements BCEDistr::CDF to calculate the weighted sum of CDFs.
  double CDF(double v0, double v1) const
  {
    vector<BCEDistr*>::const_iterator distr;
    vector<double>::const_iterator weight;

    double p = 0;
    for (distr = distributions.begin(),
	    weight = weights.begin();
	 distr != distributions.end();
	 distr++, weight++)
      p += *weight * (*distr)->CDF(v0,v1);
    return p;
  }
};

//! Unimodal distribution
/*! Distribution that has a single mode. CDF is of the form
  F(v)=c(dv-(v-m)^k+e), so that PDF is f(v)=c(d-k(v-m)^(k-1)). */
class unimodal: public BCEDistr
{
  double m0;
  double c0;
  double d0;
  double e0;
  int k0;

  double m1;
  double c1;
  double d1;
  double e1;
  int k1;

public:
  unimodal(double _c0, double _m0, int _k0,
	   double _c1, double _m1, int _k1):
    c0(_c0), k0(_k0), m0(_m0),
    c1(_c1), k1(_k1), m1(_m1)
  {
    e0 = pow(-m0,k0);
    d0 = 1.0/c0+pow(1-m0,k0)-e0;
    e1 = pow(-m1,k1);
    d1 = 1.0/c1+pow(1-m1,k1)-e1;
  }

  unimodal(double _c, double _m, int _k):
    c0(_c), k0(_k), m0(_m),
    c1(_c), k1(_k), m1(_m)
  {
    e0 = pow(-m0,k0);
    d0 = 1.0/c0+pow(1-m0,k0)-e0;
    e1 = e0; d1 = d0;
  }

  double CDF (double v0, double v1) const
  {
    double p = 1.0;
    if (v0<0)
      return 0.0;
    else if (v0<1.0)
      p *= c0*( d0*v0 + pow(v0-m0,k0) + e0 );

    if (v1<0)
      return 0.0;
    else if (v1 < 1)
      p *= c1*( d1*v1 + pow(v1-m1,k1) + e1 );
    
    return p;
  }

}; // unimodal

//! Uniform distribution
class uniform: public BCEDistr
{
public:
  double CDF(double v0, double v1) const
  {
    double p = 1.0;
    if (v0<0)
      return 0.0;
    else if (v0<1)
      p *= v0;

    if (v1<0)
      return 0.0;
    else if (v1 < 1)
      p *= v1;
    
    return p;
  }

}; // uniform

//! Uniform w/ mass at zero
class uniformWithMassPnt: public BCEDistr
{
  double alpha;

public:
  uniformWithMassPnt() {}
  uniformWithMassPnt(double _alpha): alpha(_alpha) {}

  double CDF(double v0, double v1) const
  {
    double p = 1.0;
    if (v0<-1e-10)
      return 0.0;
    else if (v0<1e-10)
      p *= alpha;
    else if (v0<1)
      p *= (1-alpha)*v0;

    if (v1<-1e-10)
      return 0.0;
    else if (v1<1e-10)
      p *= alpha;
    else if (v1 < 1)
      p *= (1-alpha)*v1;
    
    return p;
  }

}; // uniform

//! Uniform distribution with no ties
class uniformNoTie: public BCEDistr
{
  int k;
  
public:
  uniformNoTie() {}
  uniformNoTie(int _k): k(_k) {}

  double CDF(double v0, double v1) const
  {
    double p = 1.0;
    if (v0<0)
      return 0.0;
    else if (v0<1)
      p *= v0;

    if (v1<0)
      return 0.0;
    else if (v1 < 1)
      p *= v1;
    
    if (abs(v0-v1)<(0.5/k))
      p -= 1.0/(k*k);

    return p;
  }

}; // uniformNoTie

//! CDF is of the form \f$F(v0,v1)=v_0^{\alpha_0}v_1^{\alpha_1}\f$
class vToTheAlpha: public BCEDistr
{
  double alpha0;
  double alpha1;

public:
  vToTheAlpha() {}

  vToTheAlpha(double a0, double a1): 
    alpha0(a0), 
    alpha1(a1)
  {}

  vToTheAlpha(double a): 
    alpha0(a), 
    alpha1(a)
  {}

  double CDF(double v0, double v1) const
  {
    double p = 1.0;
    if (v0<0)
      return 0.0;
    else if (v0<1)
      p *= pow(v0,alpha0);

    if (v1<0)
      return 0.0;
    else if (v1 < 1)
      p *= pow(v1,alpha1);
    
    return p;
  } // CDF

}; // vToTheAlpha


class independent: public BCEDistr
{
  double probLow0;
  double probLow1;

public:
  independent() {}

  independent (double p0, double p1):
    probLow0(p0),
    probLow1(p1)
  {}

  independent (double p):
    probLow0(p),
    probLow1(p)
  {}

  double CDF(double v0, double v1) const
  {
    double p = 1;
    if (v0 < 0)
      return 0.0;
    else if (v0 < 0.5)
      p *= probLow0*v0/0.5;
    else if (v0<1.0)
      p *= probLow0 + (1-probLow0)*(v0-0.5)/0.5;

    if (v1 < 0)
      return 0.0;
    else if (v1 <0.5)
      p *= probLow1*v1/0.5;
    else if (v1 < 1.0)
      p *= probLow1 + (1-probLow1)*(v1-0.5)/0.5;

    return p;
  } // CDF

}; // independent

class truncated: public BCEDistr
{
public:
  BCEDistr * dist;
  double min0;
  double min1;
  double max0;
  double max1;

  truncated(BCEDistr * _dist):
    dist(_dist),
    min0(0),
    min1(0),
    max0(1),
    max1(1)
  {}

  truncated(BCEDistr * _dist, 
	    double _min, double _max): 
    dist(_dist),
    min0(_min),
    min1(_min),
    max0(_max),
    max1(_max)
  {
    assert(max0 >= min0);
  }
  truncated(BCEDistr * _dist, 
	    double _min0, double _min1,
	    double _max0, double _max1): 
    dist(_dist),
    min0(_min0),
    min1(_min1),
    max0(_max0),
    max1(_max1)
  {
    assert(max0 >= min0);
    assert(max1 >= min1);
  }

  double CDF(double v0, double v1) const
  {
    // if (v0 < min0)
    //   v0 = min0;
    // else if (v0 > max0)
    //   v0 = max0;

    // if (v1 < min1)
    //   v1 = min1;
    // else if (v1 > max1)
    //   v1 = max1;

    if (v0 < min0-1e-13 || v1 < min1-1e-13)
      return 0;

    return dist->CDF(v0,v1)-dist->CDF(min0,min1);
  } // CDF
};

#endif
