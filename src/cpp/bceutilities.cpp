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


