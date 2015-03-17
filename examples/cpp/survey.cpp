// Survey of different auction models
// BAB 11-1-2012

#include "survey.hpp"

void findLimits(BCESolver& solver, ostream& logFile);

int main(int argc, char ** argv)
{
  int t, a, ahat;
  double entryCost=0.0;
  double reservePrice=0.0;
  int nBids=35;
  int nVals;
  int maxNVals = 26;
  int minNVals = 25;
  double signalAccuracy;
  double probLow = 1.0/3.0;
  vector<int> valueRange;

  stringstream nameStream;

  FangMorris * fangMorris;
  FPAKnown * fpaKnown;

  ofstream logFile("survey.log",ios_base::out);

  cout << "Survey of different first price auction models." << endl;

  try
    {
      // // Fang-Morris
      // logFile << "Fang-Morris computations with binary valuations, "
      // 	      << "independent with prob " << probLow 
      // 	      << " of a low valuation." << endl
      // 	      << "Signal accuracy, "
      // 	      << "maximum revenue, "
      // 	      << "minimum revenue, "
      // 	      << "maximum player 0 surplus, "
      // 	      << "minimum player 0 surplus, "
      // 	      << "maximum bidder surplus, "
      // 	      << "minimum bidder surplus" << endl;
      
      // cout << endl << "Fang Morris" << endl
      // 	   << "Signal accuracy" << endl;
      // for (signalAccuracy = .5;
      // 	   signalAccuracy <= 1.0;
      // 	   signalAccuracy += 0.05)
      // 	{
      // 	  cout << signalAccuracy << endl;
      // 	  logFile << signalAccuracy << ", ";

      // 	  fangMorris = new FangMorris(nBids,entryCost,signalAccuracy);
      // 	  fangMorris->distribution.clear();
      // 	  fangMorris->distribution.push_back(independent(probLow),1.0);
	  
      // 	  BCESolver solver(*fangMorris);
      // 	  solver.setParameter(BCESolver::DisplayLevel,0);
      // 	  solver.populate();

      // 	  findLimits(solver,logFile);

      // 	  delete fangMorris;
      // 	}

      // // FPA with known private values, vary # of valuations with
      // // different distributions.  

      // // Start with unimodal distributions centered on mode, as mode
      // // varies from 0 to 1.
      // reservePrice = 0;
      // logFile << "FPA known private value computations, "
      // 	      << "with CDF (d*v-(v-mode)^3+e) " << endl
      // 	      << "nVals, mode, "
      // 	      << "maximum revenue, "
      // 	      << "minimum revenue, "
      // 	      << "maximum player 0 surplus, "
      // 	      << "minimum player 0 surplus, "
      // 	      << "maximum bidder surplus, "
      // 	      << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with CDF (d*v-(v-mode)^3+e) " << endl
      // 	   << "nVals, mode" << endl;
      // for (double mode=0; mode <= 1; mode += 0.25)
      // 	{
      // 	  for (nVals = 2; nVals < maxNVals; nVals++)
      // 	    {
      // 	      logFile << nVals << ", " << mode << ", ";
      // 	      cout << nVals << ", " << mode << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,entryCost,reservePrice);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(unimodal(1.0,mode,3),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // nVals
      // 	} // mode

      // // Uniform example
      // reservePrice = 0;
      // logFile << "FPA known private value computations, "
      // 	      << "with uniform CDF " << endl
      // 	      << "nVals, "
      // 	      << "maximum revenue, "
      // 	      << "minimum revenue, "
      // 	      << "maximum player 0 surplus, "
      // 	      << "minimum player 0 surplus, "
      // 	      << "maximum bidder surplus, "
      // 	      << "minimum bidder surplus" 
      // 	      << "maximum total surplus, "
      // 	      << "minimum total surplus" << endl;
      
      valueRange = vector<int>(0);
      // valueRange.push_back(2);
      // valueRange.push_back(3);
      // valueRange.push_back(4);
      // valueRange.push_back(5);
      // valueRange.push_back(6);
      // valueRange.push_back(7);
      // valueRange.push_back(8);
      // valueRange.push_back(9);
      // valueRange.push_back(10);
      // valueRange.push_back(15);
      // valueRange.push_back(20);
      // valueRange.push_back(25);
      // valueRange.push_back(30);
      valueRange.push_back(35);

      cout << "Values to cover: ";
      for (int n : valueRange)
      	cout << n << " ";

      cout << endl << "FPA known private value computations, "
      	   << "with uniform CDF " << endl
      	   << "nVals" << endl;
      for (int n : valueRange)
      	{
      	  logFile << n << ", ";
      	  cout << n << endl;

      	  fpaKnown = new FPAKnown(nBids,n,entryCost,reservePrice);
      	  fpaKnown->distribution.clear();
      	  fpaKnown->distribution.push_back(uniform(),1.0);
	      
      	  BCESolver solver(*fpaKnown);
      	  solver.setParameter(BCESolver::DisplayLevel,1);
      	  solver.populate();
      	  findLimits(solver,logFile);

      	  delete fpaKnown;
      	} // for nVals

      // // // Asymmetric example. Repeat above, but keep P 1's mode fixed
      // // at 1/2.
      // logFile << "FPA known private value computations, "
      // 	   << "with CDF (d*v-(v-mode)^3+e) for player 0 and " 
      // 	   << "with CDF (d*v-(v-0.5)^3+e) for player 1." << endl
      // 	   << "nVals, mode, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with CDF (d*v-(v-mode)^3+e) for player 0 and " 
      // 	   << "with CDF (d*v-(v-0.5)^3+e) for player 1." << endl
      // 	   << "nVals, mode, " << endl;
      // for (double mode=0; mode <= 1; mode += 0.25)
      // 	{
      // 	  for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	    {
      // 	      logFile << nVals << ", " << mode << ", ";
      // 	      cout << nVals << ", " << mode << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,entryCost,reservePrice);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(unimodal(1.0,mode,3,1.0,0.5,3),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // nVals
      // 	} // mode

      // // Bimodal example. Symmetric CDF with two modes, separated by 0.5.
      // logFile << "FPA known private value computations, "
      // 	   << "with CDF 0.5*(d0*v-(v-mode)^3+e0) + 0.5*(d1*v-(v-mode-1/2)^3+e1)." << endl
      // 	   << "nVals, mode, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with CDF 0.5*(d0*v-(v-mode)^3+e0) + 0.5*(d1*v-(v-mode-1/2)^3+e1)." << endl
      // 	   << "nVals, mode" << endl;
      // for (double mode=0; mode <= 0.5; mode += 0.25)
      // 	{
      // 	  for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	    {
      // 	      logFile << nVals << ", " << mode << ", ";
      // 	      cout << nVals << ", " << mode << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,entryCost,reservePrice);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(unimodal(1.0,mode,3),0.5);
      // 	      fpaKnown->distribution.push_back(unimodal(1.0,mode+0.5,3),0.5);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // nVals
      // 	} // mode

      // // Symmetric example with CD v^alpha, varying alpha.
      // logFile << "FPA known private value computations, "
      // 	   << "with CDF v^alpha0*v^alpha1 with alpha1=1." << endl
      // 	   << "nVals, alpha0, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with CDF v^alpha0*v^alpha1 with alpha1=1." << endl
      // 	   << "nVals, alpha0, " << endl;
      // for (double alpha=-1; alpha <= minNVals; alpha += 0.5)
      // 	{
      // 	  for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	    {
      // 	      logFile << nVals << ", " << exp(alpha) << ", ";
      // 	      cout << nVals << ", " << exp(alpha) << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,entryCost,reservePrice);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(vToTheAlpha(exp(alpha),1.0),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // nVals
      // 	} // alpha

      // // Check influence of entry fees.
      // // Uniform CDF
      // logFile << "FPA known private value computations, "
      // 	   << "with uniform CDF." << endl
      // 	   << "nVals, entry cost, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with uniform CDF." << endl
      // 	   << "nVals, entry cost, " << endl;
      // for (double entryCost=0; entryCost <= 1; entryCost += 0.1)
      // 	{
      // 	  for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	    {
      // 	      logFile << nVals << ", " << entryCost << ", ";
      // 	      cout << nVals << ", " << entryCost << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,entryCost,reservePrice);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(uniform(),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // nVals
      // 	} // entryCost
      // // v^2
      // logFile << "FPA known private value computations, "
      // 	   << "with CDF v^2." << endl
      // 	   << "nVals, entry cost, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with CDF v^2." << endl
      // 	   << "nVals, entry cost, " << endl;
      // for (double entryCost=0; entryCost <= 1; entryCost += 0.1)
      // 	{
      // 	  for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	    {
      // 	      logFile << nVals << ", " << entryCost << ", ";
      // 	      cout << nVals << ", " << entryCost << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,entryCost,reservePrice);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(vToTheAlpha(2.0),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // nVals
      // 	} // entryCost
      // // Unimodal around 1/2
      // logFile << "FPA known private value computations, "
      // 	   << "with CDF unimodal around 1/2: dv-(v-1/2)^3+e." << endl
      // 	   << "nVals, entry cost, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with CDF unimodal around 1/2: dv-(v-1/2)^3+e." << endl
      // 	   << "nVals, entry cost, " << endl;
      // for (double entryCost=0; entryCost <= 1; entryCost += 0.1)
      // 	{
      // 	  for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	    {
      // 	      logFile << nVals << ", " << entryCost << ", ";
      // 	      cout << nVals << ", " << entryCost << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,entryCost,reservePrice);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(unimodal(1.0,0.5,3),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // nVals
      // 	} // entryCost
      // // Asymmetric: Player 0 is v^2 and player 1 is unimodal around 1/2
      // logFile << "FPA known private value computations, "
      // 	   << "with player 0 CDF v^2 and player 1 CDF "
      // 	   << " dv-(v-1/2)^2+e." << endl
      // 	   << "nVals, entry cost, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with player 0 CDF v^2 and player 1 CDF "
      // 	   << " dv-(v-1/2)^2+e." << endl
      // 	   << "nVals, entry cost, " << endl;
      // for (double entryCost=0; entryCost <= 1; entryCost += 0.1)
      // 	{
      // 	  for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	    {
      // 	      logFile << nVals << ", " << entryCost << ", ";
      // 	      cout << nVals << ", " << entryCost << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,entryCost,reservePrice);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(unimodal(1.0,0.5,3),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // nVals
      // 	} // entryCost

      // // Check influence of reserve prices.
      // // Uniform CDF
      // logFile << "FPA known private value computations, "
      // 	   << "with uniform CDF." << endl
      // 	   << "nVals, reserve price, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   // << "maximum player 0 surplus, "
      // 	   // << "minimum player 0 surplus, "
      // 	   // << "maximum bidder surplus, "
      // 	   // << "minimum bidder surplus" 
      // 	      << endl;      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with uniform CDF." << endl
      // 	   << "nVals, reserve price, " << endl;
      // for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	{
      // 	  for (double reserve=-1e-6; 
      // 	       reserve < 1.0; 
      // 	       reserve += 1.0/(nVals-1.0))
      // 	    {
      // 	      logFile << nVals << ", " << reserve << ", ";
      // 	      cout << nVals << ", " << reserve << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,0.0,reserve-1e-10);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(uniform(),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // reserve
      // 	} // nVals

      // // Check influence of entry fees.
      // // Uniform CDF
      // logFile << "FPA known private value computations, "
      // 	   << "with uniform CDF." << endl
      // 	   << "nVals, entry fee, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   // << "maximum player 0 surplus, "
      // 	   // << "minimum player 0 surplus, "
      // 	   // << "maximum bidder surplus, "
      // 	   // << "minimum bidder surplus" 
      // 	      << endl;      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with uniform CDF." << endl
      // 	   << "nVals, reserve price, " << endl;
      // for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	{
      // 	  for (double fee=-1e-6; 
      // 	       fee < 1.0; 
      // 	       fee += 1.0/(nVals-1.0))
      // 	    {
      // 	      logFile << nVals << ", " << fee << ", ";
      // 	      cout << nVals << ", " << fee << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,fee,0);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(uniform(),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // reserve
      // 	} // nVals

      // // v^2
      // logFile << "FPA known private value computations, "
      // 	   << "with CDF v^2." << endl
      // 	   << "nVals, reserve price, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with CDF v^2." << endl
      // 	   << "nVals, reserve price, " << endl;
      // for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	{
      // 	  for (double reserve=-0.25/(nVals-1.0);
      // 	       reserve < 1.0; 
      // 	       reserve += 1.0/(nVals-1.0))
      // 	    {
      // 	      logFile << nVals << ", " << reserve << ", ";
      // 	      cout << nVals << ", " << reserve << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,0.0,reserve-1e-10);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(vToTheAlpha(2.0),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // reserve
      // 	} // nVals

      // // Unimodal around 1/2
      // logFile << "FPA known private value computations, "
      // 	   << "with CDF unimodal around 1/2: dv-(v-1/2)^3+e." << endl
      // 	   << "nVals, reserve price, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with CDF unimodal around 1/2: dv-(v-1/2)^3+e." << endl
      // 	   << "nVals, reserve price, " << endl;
      // for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	{
      // 	  for (double reserve=-0.25/(nVals-1.0); 
      // 	       reserve < 1.0; 
      // 	       reserve += 1.0/(nVals-1.0))
      // 	    {
      // 	      logFile << nVals << ", " << reserve << ", ";
      // 	      cout << nVals << ", " << reserve << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,0.0,reserve-1e-10);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(unimodal(1.0,0.5,3),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // reserve
      // 	} // nVals
      // // Asymmetric: Player 0 is v^2 and player 1 is unimodal around 1/2
      // logFile << "FPA known private value computations, "
      // 	   << "with player 0 CDF v^2 and player 1 CDF "
      // 	   << " dv-(v-1/2)^2+e." << endl
      // 	   << "nVals, reserve price, "
      // 	   << "maximum revenue, "
      // 	   << "minimum revenue, "
      // 	   << "maximum player 0 surplus, "
      // 	   << "minimum player 0 surplus, "
      // 	   << "maximum bidder surplus, "
      // 	   << "minimum bidder surplus" << endl;
      
      // cout << endl << "FPA known private value computations, "
      // 	   << "with player 0 CDF v^2 and player 1 CDF "
      // 	   << " dv-(v-1/2)^2+e." << endl
      // 	   << "nVals, reserve price, " << endl;
      // for (nVals = minNVals; nVals < maxNVals; nVals++)
      // 	{
      // 	  for (double reserve=-0.25/(nVals-1.0); 
      // 	       reserve < 1.0; 
      // 	       reserve += 1.0/(nVals-1.0))
      // 	    {
      // 	      logFile << nVals << ", " << reserve << ", ";
      // 	      cout << nVals << ", " << reserve << endl;

      // 	      fpaKnown = new FPAKnown(nBids,nVals,0.0,reserve-1e-10);
      // 	      fpaKnown->distribution.clear();
      // 	      fpaKnown->distribution.push_back(unimodal(1.0,0.5,3),1.0);
	      
      // 	      BCESolver solver(*fpaKnown);
      // 	      solver.setParameter(BCESolver::DisplayLevel,0);
      // 	      solver.populate();
      // 	      findLimits(solver,logFile);
      // 	    } // reserve price
      // 	} // nVals

    }
  catch (IloException & e)
    {
      cerr << "Concert exception caught: " << e << endl
	   << e.getMessage() << endl;
    }
  catch (BCEException & err)
    {
      cerr << "BCEException caught." << endl;
    }
  catch (...)
    {
      cerr << "Unknown exception caught." << endl;
    }

  logFile.close();

  return 0;
}

void findLimits(BCESolver& solver, 
		ostream& logFile)
{
  IloCplex cplex = solver.getCplex();

  // Maximum revenue
  cplex.getObjective().setExpr(solver.getObjectiveFunction(2));
  solver.solve();
  logFile << setprecision(16) << cplex.getObjValue() << ", ";

  // Minimum revenue
  cplex.getObjective().setExpr(-solver.getObjectiveFunction(2));
  solver.solve();
  logFile << setprecision(16) << -cplex.getObjValue() << ", ";

  // // Maximum player 1 surplus
  // cplex.getObjective().setExpr(solver.getObjectiveFunction(0));
  // solver.solve();
  // logFile << setprecision(16) << cplex.getObjValue() << ", ";

  // // Minimum player 1 surplus
  // cplex.getObjective().setExpr(-solver.getObjectiveFunction(0));
  // solver.solve();
  // logFile << setprecision(16) << -cplex.getObjValue() << ", ";

  // Maximum total bidder surplus
  cplex.getObjective().setExpr(solver.getObjectiveFunction(0)
  			       +solver.getObjectiveFunction(1));
  solver.solve();
  logFile << setprecision(16) << cplex.getObjValue() << ", ";

  // Minimum total bidder surplus
  cplex.getObjective().setExpr(-(solver.getObjectiveFunction(0)
  				 +solver.getObjectiveFunction(1)));
  solver.solve();
  logFile << setprecision(16) << -cplex.getObjValue() << ", ";

  // // Maximum total surplus
  // cplex.getObjective().setExpr(solver.getObjectiveFunction(0)
  // 			       +solver.getObjectiveFunction(1)
  // 			       +solver.getObjectiveFunction(2));
  // solver.solve();
  // logFile << setprecision(16) << cplex.getObjValue() << ", ";

  // // Minimum total surplus
  // cplex.getObjective().setExpr(-(solver.getObjectiveFunction(0)
  // 				 +solver.getObjectiveFunction(1)
  // 				 +solver.getObjectiveFunction(2)));
  // solver.solve();
  // logFile << setprecision(16) << -cplex.getObjValue(); 
  logFile << endl;
}
