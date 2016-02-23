#include "gurobi_c++.h"
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

class BCEGurobiCallback: public GRBCallback
{
public:
  double lastiter;
  BCEGurobiCallback() {
    lastiter = -GRB_INFINITY;
  }
protected:

  template<typename T> void printElement(T t, const int& width)
  {
    cout << left << setw(width) << setfill(' ') << t;
  }

  void callback () {
    try{
      if (where == GRB_CB_MESSAGE) {

	string algorithmOutput = getStringInfo(GRB_CB_MSG_STRING);
	// Will be redirected to log file by bcelogstream.
	cout << algorithmOutput << endl;
      }
    }
    catch (GRBException e) {
      cout << "Error number: " << e.getErrorCode() << endl;
      cout << e.getMessage() << endl;
    } 
    catch (...) {
      cout << "Error during callback" << endl;
    }
  }
};
