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

using namespace std;

//! Callback object to communicate with the gurobi solver.
class BCEGurobiCallback: public GRBCallback
{
public:
  //! Contains if user has cancelled the solve ("1" if cancel has been hit).
  bool cancelFlag;

public:

  //! Constructor
  BCEGurobiCallback():
    cancelFlag(false)
  {}

  //! Sets cancel flag to 1 and attempts to abort the solve.
  void setCancelFlag() {
    // if (cancelFlag != 1)
    cancelFlag = true;
    cout << "set cancel flag hit" << endl;
  }

protected:

  //! Sends algorithm output to the log file.
  void callback () {
    try {
      if (where == GRB_CB_BARRIER) {
	cout << "Here I am" << endl;
	cout << cancelFlag << endl;
	if (cancelFlag) {
	  abort();
	  cout << "abort hit" << endl;
	}
      }
      else if (where == GRB_CB_SIMPLEX) {
	if (cancelFlag) {
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
