// This file is part of the BCESolve library for games of incomplete
// information
// Copyright (C) 2022 Benjamin A. Brooks
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

#ifndef MULTIINDEX_HPP
#define MULTIINDEX_HPP

class AbstractIndex
{
public:
  virtual bool operator++() = 0;
  virtual int Index() const = 0;
  virtual int Max() const = 0;
  virtual int reset();
}; // Abstract index

class IntIndex
{
private:
  int index;
  int max;
public:
  IntIndex(int _max,
	   int _index=0):
    max(_max),
    index(_index)
  {}

  bool operator++() {index++;}
  int Index() const {return index;}
  int Max() const {return max;}
  int reset() { index=0; }
}; // Abstract index

class MultiIndex : public AbstractIndex
{
private:
  vector<AbstractIndex*> indices;
  vector<bool> active;
  vector<int> intIndices;
  int index;
  int activeIndex;
  int max;
  
  vector<int> increments;
  vector<int> activeIncrements;

  bool indicesUpToDate;
  
public:
  MultiIndex():
    indicesUpToDate(false)
  {} // MultiIndex

  ~MultiIndex()
  {
    for (int coordinate=0; coordinate < indices.size(); coordinate++)
      delete indices[coordinate];
  }
  
  void push_back(AbstractIndex * newIndex,
		 bool newActive=true):
  {
    indicesUpToDate = false;
    // Add the new index
    indices.push_back(newIndex);
    active.push_back(newActive);

    // Increase max
    max *= newIndex->Max();

    // New increment should be the penultimate increment times the
    // penultimate's maximum index.
    if (isempty(increments))
      increments=vector<int>(1,1);
    else
      increments.push_back(increments.back()*indices.back()->Max());

    // Have to increase the index to reflect the new index.
    index += indices.back()->Index()*increments.back();

    // New active increment should be the penultimate active increment
    // times the penultimate active increment's maximum index.
    activeIncrements.push_back(1);
    for (int coordinate = 0; coordinate < indices.size()-1; coordinate++)
      {
	if (active[coordinate])
	  activeIncrements.back()*=indices[coordinate]->Max();
      } // for
    activeIndex += indices.back()->Index()*activeIncrements.back();
  } // push_back

  const vector<int> & Indices() const
  { 
    // Only update the indices vector when requested
    for (int coordinate=0; coordinate<indices.size(); coordinate++)
      {
	intIndices[coordinate]=indices[coordinate]->Index();
      } // for

    indicesUpToDate = true;
    return indices; 
  }

  int Index() const { return index; }
  const AbstractIndex & Index(int coordinate) const
  { return *indices[coordinate]; }
  
  bool operator++()
  {
    indicesUpToDate = false;
    for (int coordinate=0; 
	 coordinate < indices.size(); 
	 coordinate++)
      {
	if (increment(coordinate))
	  return true;
      }
    return false;
  } // Increment operator

  bool increment(int coordinate)
  {
    indicesUpToDate = false;
    if (active[coordinate]
	&& indices[coordinate]->Index()<indices[coordinate]->Max()-1)
      {
	(*indices[coordinate])++;
	activeIndex++;
	linearIndex += increments[coordinate];

	while (coordinate>0)
	  {
	    coordinate--;
	    if (active[coordinate])
	      {
		linearIndex-=increments[coordinate]*indices[coordinate]->Max();
		indices[coordinate]->reset();
	      }
	  }
	return true;
      }
    return false;
  } // Increment coordinate

  void reset()
  {
    indicesUpToDate = false;
    for (int coordinate=0; coordinate<indices.size(); coordinate++)
      indices[coordinate]->reset();
    index=0;
    activeIndex=0;
  }

  void setActive(int coordinate,bool newActive)
  {

  }

}; // MultiIndex

class MultiIntIndex : public MultiIndex
{
public:
  MultiIntIndex(const vector<int> & sizes):
    MultiIndex()
  {
    for (int coordinate=0; coordinate<sizes.size(); coordinate++)
      push_back(new IntIndex(sizes[coordinate]));
  } // MultiIntIndex

}; // MultiIntIndex


#endif
