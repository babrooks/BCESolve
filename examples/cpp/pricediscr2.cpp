// Ben's file for studying linear programs of duopoly price
// discrimination.
// 2-12-2013

#include <iostream>
#include <ilcplex/ilocplex.h>
#include <vector>

using namespace std;

void printEquilibrium(const IloCplex & cplex,
		      const IloNumVarArray & var,
		      int numVals);

int main (int argc, char**argv)
{
  int numVals = 10;
  double highVal = 1;
  double lowVal = 0.0;
  double cost = 0.0;
  
  int priceIdx, valIdx, devIdx,
    row, col;
  double price, val, dev;

  double valIncr = (highVal-lowVal)/(numVals-1);

  double prior[numVals*numVals];
  double probsum = 0;
  // Uniform prior
  for (valIdx = 0; valIdx < numVals*numVals; valIdx++)
    prior[valIdx] = 1.0/(numVals*numVals);

  double priorSum[numVals];

  IloEnv env;
  try
    {
      IloModel model(env);
      IloCplex cplex(env);

      IloNumVarArray var(env,numVals*numVals,0.0,1.0);
      IloRangeArray con(env);
      IloNumExpr profit(env,0), 
	surplus(env,0), 
	consumerSurplus(env,0),
	quantitySold(env,0);

      IloObjective obj = IloMaximize(env);

      // Populate
      // Set up equality constraints
      for (valIdx = 0; valIdx < numVals; valIdx ++)
	con.add(IloRange(env,prior[valIdx],prior[valIdx]));
      // Inequality constraints
      con.add(IloRangeArray(env,2*numPrices*numPrices,-IloInfinity,0.0));

      // Set up constraints
      col = 0;
      for (priceIdx = 0, price = lowVal; 
	   priceIdx < numVals; 
	   priceIdx++, price += valIncr )
	{
	  for (valIdx = 0, val = lowVal; 
	       valIdx < numVals; 
	       valIdx++, val += valIncr)
	    {
	      // Equality constraint
	      con[valIdx].setLinearCoef(var[col],1.0);

	      // Deviations. Monopolist gets these guys if he deviates
	      // to a price less than valIdx.
	      row = (priceIdx+1)*numVals;
	      for (devIdx = 0, dev=lowVal; 
		   devIdx < numVals; 
		   devIdx++, dev += valIncr )
		{
		  if (valIdx >= priceIdx && valIdx >= devIdx)
		    con[row].setLinearCoef(var[col],dev-price);
		  else if (valIdx >= priceIdx)
		    con[row].setLinearCoef(var[col],cost-price);
		  else if (valIdx >= devIdx)
		    con[row].setLinearCoef(var[col],dev-cost);

		  row++;
		}
	      
	      if (valIdx >= priceIdx)
		{
		  profit += (price-cost) * var[col];
		  surplus += (val-cost) * var[col];
		  consumerSurplus += (val-price)*var[col];
		  quantitySold += var[col];
		}

	      col++;
	    } // val
	} // price

      model.add(con);
      model.add(IloMaximize(env));

      // Done with populate.
      cplex.setParam(IloCplex::BarDisplay,0);
      cplex.setParam(IloCplex::SimDisplay,0);
      cplex.setParam(IloCplex::EpOpt,1e-9);

      cplex.extract(model);
      
      obj = cplex.getObjective();

      double extraSurplus = 0.4;

      IloRange quantitySoldUB(env,-IloInfinity,quantitySold,monopolyQuantity);
      IloRange surplusLB(env,monopolySurplus,surplus,IloInfinity);
      IloRange zeroCS(env,0.0,consumerSurplus,0.0);
      IloRange zeroAdditionalProfits(env,monopolyProfits,profit,monopolyProfits);
      IloRange surplusEquality(env,monopolyProfits+extraSurplus,
			       surplus,monopolyProfits+extraSurplus);
      double fixedQuantity = monopolyQuantity-0.1;
      IloRange quantityEquality(env,fixedQuantity,quantitySold,fixedQuantity);

      // Max surplus subject to quantity being no more than uniform monopoly.
      model.add(quantityEquality);
      cplex.extract(model);
      obj.setExpr(surplus);
      obj.setSense(IloObjective::Minimize);
      cplex.solve();

      cout << endl
      	   << "Solved for min surplus subject to quantity equals " << fixedQuantity << endl
      	   << "Profit is " << cplex.getValue(profit) << endl
      	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      	   << "Surplus is " << cplex.getValue(surplus) << endl
      	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      printEquilibrium(cplex,var,numVals);

      model.add(zeroCS);
      cplex.extract(model);
      obj.setSense(IloObjective::Minimize);
      cplex.solve();

      cout << endl
      	   << "Solved for min surplus subject to quantity equals " << fixedQuantity << " and zero CS" << endl
      	   << "Profit is " << cplex.getValue(profit) << endl
      	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      	   << "Surplus is " << cplex.getValue(surplus) << endl
      	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      printEquilibrium(cplex,var,numVals);
      model.remove(zeroCS);

      model.add(zeroAdditionalProfits);
      cplex.extract(model);
      obj.setSense(IloObjective::Minimize);
      cplex.solve();

      cout << endl
      	   << "Solved for min surplus subject to quantity equals " << fixedQuantity << " and zero CS" << endl
      	   << "Profit is " << cplex.getValue(profit) << endl
      	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      	   << "Surplus is " << cplex.getValue(surplus) << endl
      	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      printEquilibrium(cplex,var,numVals);
      model.remove(zeroAdditionalProfits);
      model.remove(quantityEquality);



      // // Max surplus subject to quantity being no more than uniform monopoly.
      // model.add(surplusEquality);
      // model.add(zeroCS);
      // cplex.extract(model);
      // obj.setExpr(quantitySold);
      // obj.setSense(IloObjective::Maximize);
      // cplex.solve();
      // cout << endl
      // 	   << "Solved for max surplus subject to quantity no more than uniform monopoly" << endl
      // 	   << "Profit is " << cplex.getValue(profit) << endl
      // 	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      // 	   << "Surplus is " << cplex.getValue(surplus) << endl
      // 	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      // printEquilibrium(cplex,var,numVals);

      double alpha = 0, upperProbSum = 0, upperAlphaSum = 0;
      for (priceIdx = numVals-1, price = highVal; 
	   priceIdx>=monopolyPriceIdx; 
	   priceIdx--, price -= valIncr)
	{
	  alpha = max(0.0,prior[priceIdx]-upperAlphaSum/(price-cost)+upperProbSum);
	  upperAlphaSum += (price-cost)*alpha;
	  upperProbSum += prior[priceIdx];
	  cout << alpha << " | ";
	}
      cout << endl;
      // // Max surplus subject to quantity being no more than uniform monopoly.
      // model.add(quantitySoldUB);
      // cplex.extract(model);
      // obj.setExpr(surplus);
      // obj.setSense(IloObjective::Maximize);
      // cplex.solve();
      // cout << endl
      // 	   << "Solved for max surplus subject to quantity no more than uniform monopoly" << endl
      // 	   << "Profit is " << cplex.getValue(profit) << endl
      // 	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      // 	   << "Surplus is " << cplex.getValue(surplus) << endl
      // 	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      // printEquilibrium(cplex,var,numVals);
      // model.remove(quantitySoldUB);

      // // Min quantity sold subject to surplus being at least the
      // // uniform monopoly level.
      // model.add(surplusLB);
      // cplex.extract(model);
      // obj.setExpr(quantitySold);
      // obj.setSense(IloObjective::Minimize);
      // cplex.solve();
      // cout.precision(5);
      // cout << endl
      // 	   << "Solved for min quantity sold subject to surplus being at least uniform monopoly" << endl
      // 	   << "Profit is " << cplex.getValue(profit) << endl
      // 	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      // 	   << "Surplus is " << cplex.getValue(surplus) << endl
      // 	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      // printEquilibrium(cplex,var,numVals);
      // model.remove(surplusLB);

      // // Minimize profit
      // obj.setExpr(profit);
      // obj.setSense(IloObjective::Minimize);

      // cplex.solve();
      // cout << endl
      // 	   << "Solved for min profit" << endl
      // 	   << "Profit is " << cplex.getValue(profit) << endl
      // 	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      // 	   << "Surplus is " << cplex.getValue(surplus) << endl
      // 	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      // printEquilibrium(cplex,var,numVals);

      // // Try minimizing just quantity sold
      // obj.setExpr(quantitySold);
      // obj.setSense(IloObjective::Minimize);

      // cplex.solve();
      // cout << endl
      // 	   << "Solved for min quantity sold" << endl
      // 	   << "Profit is " << cplex.getValue(profit) << endl
      // 	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      // 	   << "Surplus is " << cplex.getValue(surplus) << endl
      // 	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      // printEquilibrium(cplex,var,numVals);
      

      // // Now min surplus
      // obj.setExpr(surplus);
      // obj.setSense(IloObjective::Minimize);

      // cplex.solve();
      // cout << endl
      // 	   << "Solved for min surplus" << endl
      // 	   << "Profit is " << cplex.getValue(profit) << endl
      // 	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      // 	   << "Surplus is " << cplex.getValue(surplus) << endl
      // 	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      // printEquilibrium(cplex,var,numVals);

      // // Min quantity sold subject to min surpus
      // model.add(IloRange(env,-IloInfinity,surplus,cplex.getObjValue()));
      // obj.setExpr(quantitySold);
      // cplex.solve();
      // cout << endl
      // 	   << "Solved for min quantity sold subject to min surplus" << endl
      // 	   << "Profit is " << cplex.getValue(profit) << endl
      // 	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      // 	   << "Surplus is " << cplex.getValue(surplus) << endl
      // 	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      // printEquilibrium(cplex,var,numVals);

      // obj.setSense(IloObjective::Maximize);
      // cplex.solve();
      // cout << endl
      // 	   << "Solved for max quantity sold subject to min surplus" << endl
      // 	   << "Profit is " << cplex.getValue(profit) << endl
      // 	   << "Consumer Surplus is " << cplex.getValue(consumerSurplus) << endl
      // 	   << "Surplus is " << cplex.getValue(surplus) << endl
      // 	   << "Quantity sold is " << cplex.getValue(quantitySold) << endl;
      // printEquilibrium(cplex,var,numVals);

      

    } // try
  catch (IloException & e)
    {
      cerr << "Caught IloException" << endl
	   << e.getMessage() << endl;
    } // catch
  catch (...)
    {
      cerr << "Unknwn exception caught" << endl;
    }

  env.end();

  return 0;
}

void printEquilibrium(const IloCplex & cplex,
		      const IloNumVarArray & var,
		      int numVals)
{
  double val;
  // cout.width(5);
  cout.setf(ios::fixed);
  int col=0;
  for (int priceIdx = 0; priceIdx < numVals; priceIdx++)
    {
      for (int valIdx = 0; valIdx < numVals; valIdx++)
	{
	  val = cplex.getValue(var[col]);
	  if (val > 1e-10)
	    {
	      cout.width(7);
	      cout << setprecision(5) << cplex.getValue(var[col]) << " ";
	    }
	  else
	    {
	      cout.width(7);
	      cout << setprecision(1) << left << 0.0 << " ";
	    }
	  col++;
	} // val
				   
      cout << endl;
    } // price
} // printEquilibrium

