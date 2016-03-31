#include "bce.hpp"

int main()
{
  BCEGame game;

  try
    {
      BCEGame::load(game,"battleofsexesvspd2.bgm");

      BCESolver solver(game);
      
      
      
      solver.populate();
      
      solver.model.setObjective(-1.0*solver.getObjectiveFunction(0)
				- solver.getObjectiveFunction(1));

      solver.solve();

      solver.mapBoundary();

      BCESolution soln;

      solver.getSolution(soln);

      BCESolution::save(soln,"battleofsexesvspdboundary2.bce");
    }
  catch (GRBException & e)
    {
      cout << e.getMessage() << endl;
    }
  catch (BCEException & e)
    {
      cout << e.getMessage() << endl;
    }
  
}
