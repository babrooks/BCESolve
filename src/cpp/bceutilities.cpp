// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
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

#include "bceutilities.hpp"

int indexToVector(int index, 
		  vector<int> & v, 
		  const vector<int> & sizes)
{

  if (index < 0 || sizes.size() <= 0)
    throw(BCEException(BCEException::BadArgumentBCEUtilities));
  // assert(index>=0);
  // assert(sizes.size()>0);

  v.resize(sizes.size(),0);

  for (int coordinate = 0;
       coordinate < sizes.size();
       coordinate++)
    {
      // assert(sizes.size()>0);
      v[coordinate] = index % sizes[coordinate];
      index -= v[coordinate]; index /= sizes[coordinate];
    }
  return index;
}

int vectorToIndex(const vector<int> & v, 
		  const vector<int> & sizes)
{
  int index = 0;
  if (v.size() != sizes.size())
    throw(BCEException(BCEException::BadArgumentBCEUtilities));
  // assert(v.size() == sizes.size());
  for (int coordinate = sizes.size()-1;
       coordinate >= 0;
       coordinate -- )
    {
      index *= sizes[coordinate];
      index += v[coordinate];
    }
  return index;
}


