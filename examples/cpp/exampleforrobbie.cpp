#include <bce.hpp>

int main ()
{
  BCEData data;

  BCEData::load(data,"fpaknown_nv=2_nb=15.bce");

  vector<double> distribution;

  // Conditions
  vector<int> stateConditions(1,3); // state = 0
  vector< vector<int> > actionConditions(2,vector<int>(0,0)); // any action
  vector< vector<int> > typeConditions(2,vector<int>(0,0)); // any type

  // Marginals
  bool stateMarginal = false;
  vector<bool> actionMarginal(2,true);
  vector<bool> typeMarginal(2,false);

  double prob
    = data.getConditionalMarginal(stateConditions,
				  actionConditions, 
				  typeConditions,
				  stateMarginal,
				  actionMarginal,
				  typeMarginal,
				  distribution);

  // for (vector<double>::iterator it = distribution.begin();
  //      it != distribution.end();
  //      ++it)
  //   cout << *it << " ";
  // cout << endl;

  assert(data.numActions_total == distribution.size());

  for (int a1 = 0; a1 < data.numActions[1]; a1++)
    {
      for (int a0 = 0; a0 < data.numActions[0]; a0++)
	{
	  int index = a1 * data.numActions[0] + a0;

	  cout << setprecision(2) << setw(2) << distribution[index] << " ";
	} // for a0

      cout << endl;
    } // for a1



  actionConditions[0]=vector<int>(1,5);
  actionMarginal[0]=false;
  prob = data.getConditionalMarginal(vector<int>(0,0), // state
  				     actionConditions,
  				     typeConditions,
  				     false,
  				     actionMarginal,
  				     typeMarginal,
  				     distribution);
 
  vector< vector<double> > objectiveValues;
  data.getDeviationObjectives(1,5,1,objectiveValues);

  for (int a0 = 0; a0 < data.numActions[0]; a0++)
    {
      cout << setprecision(6) << "a0 = " << a0
	   << ", obj0 = " << objectiveValues[0][a0]
	   << ", obj1 = " << objectiveValues[1][a0]
	   << ", obj0array = " << data.objectives[0][3+data.numStates*(a0+data.numActions[0]*5)]
	   << ", prob(a1) = " << distribution[a0]
	   << endl;
    }

  // actionConditions[0]=vector<int>(1,5);
  // actionMarginal[0]=false;
  // prob = data.getConditionalMarginal(vector<int>(0,0), // state
  // 				     actionConditions,
  // 				     typeConditions,
  // 				     true,
  // 				     actionMarginal,
  // 				     typeMarginal,
  // 				     distribution);
 
  // cout << prob << endl;
  // for (int s = 0; s < data.numStates; s++)
  //   {
  //     for (int a1 = 0; a1 < data.numActions[1]; a1++)
  // 	{
  // 	  cout << "pr(s=" << s << ",a1=" << a1 << ") = "
  // 	       << distribution[s+a1*data.numStates] << endl;
  // 	}
  //   }


  actionConditions[0]=vector<int>(1,5);
  actionMarginal[0]=false;
  prob = data.getConditionalMarginal(vector<int>(0,0), // state
  				     actionConditions,
  				     typeConditions,
  				     true,
  				     vector<bool>(2,false),
  				     typeMarginal,
  				     distribution);
 
  for (int index = 0; index < distribution.size(); index++)
    cout << index << " " << distribution[index] << endl;


  return 0;
}
