// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks, Robert J. Minton
// 
// BCESolve free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// BCESolve is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
// 
// Benjamin A. Brooks
// ben@benjaminbrooks.net
// Chicago, IL

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
    if (cancelFlag != 1)
      cancelFlag = 1;
    abort();
    cout << "set cancel flag hit" << endl;
  }

protected:

  void callback () {
    try {
      if (where == GRB_CB_BARRIER) {
	cout << "barrier callback" << endl;
	if (cancelFlag == 1) {
	  abort();
	  cout << "abort hit" << endl;
	}
      }
      else if (where == GRB_CB_SIMPLEX) {
	if (cancelFlag == 1) {
	  abort();
	  cout << "abort hit" << endl;
	}
      }

      else if (where == GRB_CB_MESSAGE) {
	string algorithmOutput = getStringInfo(GRB_CB_MSG_STRING);
	// Will be redirected to log file by bcelogstream.
	cout << algorithmOutput << endl;
      }
    }
    catch (GRBException e) {
      cout << "======= GRB ERROR =======" << endl;
      cout << "GRB Error Code: " << e.getErrorCode() << endl;
      cout << e.getMessage() << endl;
    } 
    catch (...) {
      cout << "GRBCallback error, unrelated to GRB error codes." << endl;
    }
  }
};
