#ifndef DISTRIBUTIONS_HPP
#define DISTRIBUTIONS_HPP

#include <cmath>
#include <vector>

using namespace std;

// Base class from which distributions are derived.
class distribution
{
public:
  virtual double operator()(double v0, double v1) const = 0;
  virtual distribution* clone() const = 0;
};

// Class for convex combinations of multiple distributions.
class distributionArray
{
public:
  vector<distribution*> distributions;
  vector<double> weights;

  distributionArray() {}

  // Deallocate all of the space allocated for distributions.
  ~distributionArray()
  {
    vector<distribution*>::iterator distr;
    for (distr = distributions.begin();
	 distr != distributions.end();
	 distr++)
      delete *distr;
  }

  // Add a new distribution to the array. Allocate space and then copy
  // the new distribution.
  template <class Distr>
  void push_back(const Distr& newDistr, double newWeight)
  {
    distributions.push_back(newDistr.clone());
    weights.push_back(newWeight);
  }

  void clear()
  {
    vector<distribution*>::iterator distr;
    for (distr = distributions.begin();
	 distr != distributions.end();
	 distr++)
      delete *distr;
    distributions.clear();
    weights.clear();
  }

  double operator()(double v0, double v1) const
  {
    vector<distribution*>::const_iterator distr;
    vector<double>::const_iterator weight;

    double p = 0;
    for (distr = distributions.begin(),
	    weight = weights.begin();
	 distr != distributions.end();
	 distr++, weight++)
      p += *weight * (**distr)(v0,v1);
    return p;
  }
};

// Distribution that has a single mode. CDF is of the form
// F(v)=c(dv-(v-m)^k+e), so that PDF is f(v)=c(d-k(v-m)^(k-1)). 
class unimodal: public distribution
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

  double operator() (double v0, double v1) const
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

  virtual unimodal* clone() const { return new unimodal(*this); }
}; // unimodal

// Uniform distribution
class uniform: public distribution
{
public:
  double operator()(double v0, double v1) const
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

  virtual uniform* clone() const { return new uniform(*this); }
}; // uniform

// Uniform w/ mass at zero
class uniformWithMassPnt: public distribution
{
  double alpha;

public:
  uniformWithMassPnt() {}
  uniformWithMassPnt(double _alpha): alpha(_alpha) {}

  double operator()(double v0, double v1) const
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

  virtual uniformWithMassPnt* clone() const { return new uniformWithMassPnt(*this); }
}; // uniform

// Uniform distribution with no ties
class uniformNoTie: public distribution
{
  int k;
  
public:
  uniformNoTie() {}
  uniformNoTie(int _k): k(_k) {}

  double operator()(double v0, double v1) const
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

  virtual uniformNoTie* clone() const { return new uniformNoTie(*this); }
}; // uniformNoTie

// f(v0,v1) = v0^alpha0*v1^alpha1
class vToTheAlpha: public distribution
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

  double operator()(double v0, double v1) const
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

  virtual vToTheAlpha* clone() const { return new vToTheAlpha(*this); }
}; // vToTheAlpha


class independent: public distribution
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

  double operator()(double v0, double v1) const
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

  virtual independent* clone() const { return new independent(*this); }
}; // independent

template<class Distr>
class truncated: public distribution
{
public:
  Distr dist;
  double min0;
  double min1;
  double max0;
  double max1;

  truncated(Distr _dist):
    dist(_dist),
    min0(0),
    min1(0),
    max0(1),
    max1(1)
  {}

  truncated(Distr _dist, 
	    double _min, double _max): 
    dist(_dist),
    min0(_min),
    min1(_min),
    max0(_max),
    max1(_max)
  {
    assert(max0 >= min0);
  }
  truncated(Distr _dist, 
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

  double operator()(double v0, double v1) const
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

    return dist(v0,v1)-dist(min0,min1);
  } // CDF

  virtual truncated* clone() const { return new truncated(*this); }
};

// Calculates the mass in the area [v0-incr0,v0] x [v1-incr1,v1].
template <class CDF>
double PDF(const CDF & cdf,
	   double v0, double incr0,
	   double v1, double incr1)
{
  return (cdf(v0,v1) + cdf(v0-incr0,v1-incr1)
	  - cdf(v0-incr0,v1) - cdf(v0,v1-incr1));
}

template <class CDF>
double PDF(const CDF & cdf,
	   double v0, double v1, double incr)
{
  return PDF(cdf,v0,incr,v1,incr);
}
#endif
