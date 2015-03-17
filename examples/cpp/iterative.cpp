// Ben's C++ program to do iterative computation of BCE
// 7-26-2013
// BAB

// Loosely speaking, this program takes a prior distribution over
// values, and starts with everyone bidding their own value. In
// contrast to the revenue maximizing construction, we construct a
// surplus minimizing segmentation for every bidder w.r.t. the
// conditional distribution of others' bids and values. This
// segmentation is used to construct a new conditional distribution of
// others values and bids, etc.

#include "bce.hpp"

class MinRevenue : public BCEGame
{
private:
  int numValues;

public:
  MinRevenue() {};

  MinRevenue(int _numValues)
    : BCEGame(2,_numValues*_numValues,_numValues,1,2), numValues(_numValues)
  {};

  double prior(int state, const vector<int> & types)
  {
    return 1.0/( (double) numStates);
  }

  double objective(int state, const vector<int> & actions, int player)
  {
    int vals[2];
    val[0] = state % numValues;
    val[1] = (state - val[0])/numValues;

    double bids[2]; double values[2];

    bids[0] = actions[0]/(numActions[0]-1.0);
    bids[1] = actions[1]/(numActions[1]-1.0);

    values[0] = val[0]/(numValues-1.0);
    values[1] = val[1]/(numValues-1.0);

    if (action[player]>action[1-player]
	|| (action[player]==action[1-player]
	    && val[player]>val[1-player]))
      return values[player] - bids[player];
    else if (action[player]==action[1-player] 
	     && val[player]==val[1-player])
      return values[player] - bids[player];
    
    return 0.0;
  }
  
  
  bool dominated (int a, int t, int player)
  {
    return false;
  }


  void minSurplus()
  {
    vector<double> distr(numValues*numValues*numValues*numValues);

    vector< vector<double> > condDistr(numValues,
				       vector<double> (numValues*numValues,0));
    
    vector< vector<double> > segmentations(numValues,
					   vector<double> (numValues*numValues*numValues,0));
    
    int v;

    // Initialize the conditional distributions; everyone bids their
    // own value.
    for (v = 0; v<numValues; v++)
      {
	condDistr[v][vp*numValues+vp] = prior(v*numValues+vp,vector<double>(0));
      } // for
    
    // Now for each value construct a surplus minimizing segmentation.
    for (v=0; v < numValues; v++)
      {
	int ihat; 
	double betahat; 
	double pihat;
	double pitemp;
	double pistar; 
	double bstar;
	int b;
	double probsum = 0;

	// Calculate the "monopoly profit"
	for (b = 0; b <= v; b++)
	  {
	    for (vp = 0; vp < v; vp ++)
	      probsum += condDistr[v][vp*numValues+b];

	    probsum += condDistr[v][v*numValues+b]/2.0;

	    pitemp = (v-b)/(numValues-1.0)*probsum;
	    if (pitemp > pistar)
	      {
		pistar = pitemp;
		bstar = b;
	      }

	    probsum += condDistr[v][v*numValues+b]/2.0;
	    for (vp = v; vp < numValues; vp ++)
	      probsum += condDistr[v][vp*numValues+b];
	  } // for b

	// Find ihat
	pitemp = 0;

	while (ihat <= bstar)
	  {
	    for (vp = 0; vp < v; vp ++)
	      pitemp += (v-ihat)/(numValues-1.0)*condDistr[v][vp*numValues + ihat];
	    pitemp += (v-ihat)/(numValues-1.0)*condDistr[v][v*numValues + ihat]/2.0;

	    if (pihat+pitemp >= pistar)
	      {
		betahat = (pistar-pihat)/pitemp;
		break;
	      }

	    pihat += pitemp;
	    pitemp = 0;

	    pitemp += condDistr[v][v*numValues + ihat]/2.0;
	    for (vp = v+1; vp < numValues; vp ++)
	      pitemp += (v-ihat)/(numValues-1.0)*condDistr[v][vp*numValues+ihat];

	    ihat++;
	  } // while

	// Construct the new segmentation.
	for (b = 0; b <= ihat; b++)
	  {
	    segmentations[v][b*numValues+b] = 

	  } // for b

      }

  }

};

int main ()
{
  

  return 0;
}
