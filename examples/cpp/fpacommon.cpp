#include "fpacommon.hpp"

double solveAuction(int nb, int nv, 
		    double entryCost, double reservePrice,
		    double highbid,
		    double weight);

int main(int argc, char ** argv)
{

  int nb = 50;
  int nv = nb;

  double highbid = 1.0;

  stringstream ss;
  
  ss << "/home/babrooks/Dropbox/bergemann brooks morris/first price auctions/thirddraft/reserveData_nv="
     << nv << "_nb=" << nb << ".txt";

  ofstream ofs(ss.str().c_str(),
	       std::ofstream::out);

  ofs << "reserve/fee, minrev, maxrev, minrev, maxrev" << endl;

  for (int r = 0; r < 1; r++)
    {
      double priceFee = static_cast<double>(r)/(nv-1)-1e-6;

      cout << "r=" << r << " of " << nb << endl;

      ofs << priceFee << ", "
	  << solveAuction(nb,nv,0.0,
			  priceFee,
			  highbid,1)  << ", "
	  // << solveAuction(nb,nv,0.0,
	  // 		   priceFee,
	  // 		   highbid,-1)  << ", "
	  << solveAuction(nb,nv,
			  priceFee,0.0,
			  highbid,1)  << ", "
	  // << solveAuction(nb,nv,
	  // 		   priceFee,0.0,
	  // 		   highbid,-1)
	  << endl;
    }


  ofs.close();
  return 0;
}


double solveAuction(int nb, int nv, 
		    double entryCost, double reservePrice,
		    double highbid,
		    double weight)
{

  double minAngleIncrement = 0.025;

  int t, a, ahat;

  char filename[100];
  sprintf(filename,"fpacommon_maxp0_nv=%d_nb=%d_entrycost_%1.2f.bce",
	  nv,nb,entryCost);

  FPACommon fpa(nb,nv,entryCost,reservePrice,highbid);
  
  BCESolver solver(fpa);

  solver.setParameter(BCESolver::MinAngleIncrement,minAngleIncrement);

  try
    {
      // solver.setParameter(BCESolver::DisplayLevel,0);
      solver.populate();

      IloCplex cplex=solver.getCplex();
      cplex.setParam(IloCplex::Threads,2);
      
      cplex.getObjective().setExpr(-weight*solver.getObjectiveFunction(2));
      
      cplex.setParam(IloCplex::BarDisplay,1);
      cplex.setParam(IloCplex::SimDisplay,1);
      // cplex.setParam(IloCplex::RootAlg,IloCplex::Dual);

      solver.solve();
      
      return cplex.getValue(solver.getObjectiveFunction(2));

      // BCEData data;
      // solver.getData(data);
      // data.setNumValues(vector<int>(2,fpa.getNumValues()));

      // BCEData::save(data,filename);


    }
  catch (IloException & e)
    {
      cerr << "Concert exception caught: " << e << endl
	   << e.getMessage() << endl;
    }
  catch (BCEException & bcee)
    {
      cerr << "BCEException caught: " << endl;
      if (bcee.errorType == BCEException::NotProbDistr)
	cerr << "Not a probability distribution" << endl;
    }
  catch (...)
    {
      cout << "Unknown exception caught." << endl;
    }
} // solveAuction
