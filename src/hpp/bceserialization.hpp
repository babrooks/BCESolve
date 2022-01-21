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

#ifndef BCESERIALIZATION_HPP
#define BCESERIALIZATION_HPP

#include "bcedata.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/utility.hpp>

void saveBCEData(const BCEData & data, const char* filename)
{
  ofstream ofs(filename);

  if (ofs.good())
    {
      boost::archive::text_oarchive oa(ofs);
      oa << data;
    }
  else
    throw(BCEException(BCEException::FailedOpen));
}

void loadBCEData(BCEData & data, const char* filename)
{
  ifstream ifs(filename);

  if (ifs.good())
    {
      boost::archive::text_iarchive ia(ifs);
      ia >> data;
    }
  else
    throw(BCEException(BCEException::FailedOpen));
}

#endif
