#include "gurobi_c++.h"
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

class BCEGurobiCallback: public GRBCallback
{
private:
  int cancelFlag;

public:
  double lastiter;
  BCEGurobiCallback() {
    cancelFlag = 0;
  }

  void setCancelFlag() {
    cancelFlag = 1;
    abort();
    cout << "set cancel flag hit" << endl;
  }

protected:

  void callback () {
    if (where == GRB_CB_BARRIER) {
      cout << "barrier callback" << endl;
      if (cancelFlag == 1) {
	abort();
	cout << "abort hit" << endl;
      }
    }

    else{
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
  }
};
