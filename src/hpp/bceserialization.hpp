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
