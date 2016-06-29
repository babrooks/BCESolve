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

#include "bceobjweightstablemodel.hpp"

QVariant BCEObjWeightsTableModel::data(const QModelIndex & index,
				  int role) const
{
   if (role == Qt::SizeHintRole)
    return QSize(1,1);
  else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {

    vector<int> cellLocation(2,0);
    cellLocation[0] = index.row();
    cellLocation[1] = index.column();

    if (cellLocation[1]==0)
      return QVariant(QString::number(mainObjWeights[cellLocation[0]]));
    else if (cellLocation[1]==1)
      return QVariant(QString::number(secondaryObjWeights[cellLocation[0]]));
	
    }
  else
    return QVariant();
}

bool BCEObjWeightsTableModel::setData(const QModelIndex & index,
				 const QVariant & value,
				 int role)
{
  if (role == Qt::EditRole)
    {
      vector<int> cellLocation(2,0);
      cellLocation[0]= index.row();
      cellLocation[1] = index.column();

      QString strValue = value.toString();
      setWeightData(cellLocation,strValue.toDouble());

      emit dataChanged(index,index);
      return true;
    }
  return false;
} // setData

QVariant BCEObjWeightsTableModel::headerData(int section,
					Qt::Orientation orientation,
					int role) const
{
  if (role == Qt::DisplayRole)
    {
      switch (orientation)
	{
	case Qt::Horizontal: {
	  if (section==0)
	    return QVariant(QString("Main"));
	  return QVariant(QString("Secondary"));
	}

	case Qt::Vertical: {
	  return QVariant
	    (QString::fromStdString(objectiveLabels[section]));
	}
	}
    }
  else
    return QVariant();
} // headerData

