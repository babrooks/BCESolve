#ifndef _BCEUTILITIES_HPP
#define _BCEUTILITIES_HPP

#include "bce.hpp"

int indexToVector(int index, 
		  vector<int> & v, 
		  const vector<int> & sizes);

int vectorToIndex(const vector<int> & v,
		  const vector<int> & sizes);

#endif
