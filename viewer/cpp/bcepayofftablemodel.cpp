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

#include "bcepayofftablemodel.hpp"

QVariant BCEPayoffTableModel::data(const QModelIndex & index,
				   int role) const 
{
  if (role == Qt::SizeHintRole)
    return QSize(1,1);

  else if (role == Qt::DisplayRole || role == Qt::EditRole) {
    vector<int> cellLocation(2,0);
    cellLocation[0] = index.row();
    cellLocation[1] = index.column();

    int numObjectives = game->getNumObjectives();

    if (numObjectives == 2) {
      return QVariant(QString::number(game->objective(state,cellLocation,0))
		      +QString(", ")
		      +QString::number(game->objective(state,cellLocation,1)));
    }

    else if (numObjectives > 2) {
      QString dynamicString = QString::number(game->objective(state,cellLocation,0));
      for (int objs = 1; objs < numObjectives; objs++) {
	dynamicString += QString(", ");
	dynamicString += QString::number(game->objective(state,cellLocation,objs));
      }
      return QVariant(dynamicString);	
    }  
    
  }
 else
   return QVariant();
} // data
    
QVariant BCEPayoffTableModel::headerData(int section,
					Qt::Orientation orientation,
					int role) const
{
  if (role == Qt::DisplayRole)
    {
      switch (orientation)
	{
	case Qt::Horizontal:
	  return QVariant(QString("C")
			  +QString::number(section));

	case Qt::Vertical:
	  return QVariant(QString("R")
			  +QString::number(section));
	}
    }
  else
    return QVariant();
} // headerData

bool BCEPayoffTableModel::setData(const QModelIndex & index,
				 const QVariant & value,
				 int role)
{
  if (role == Qt::EditRole)
    {
      vector<int> cellLocation(2,0);
      cellLocation[0]= index.row();
      cellLocation[1] = index.column();

      QRegularExpression rx("[, ]");
      QStringList list = value.toString().split(rx,Qt::SkipEmptyParts);

      if (list.size() != game->getNumObjectives()) {
	data(index,role);
	return false;
      }

      int numObjectives = game->getNumObjectives();
      for (int objs = 0; objs < numObjectives; objs++)
	game->setObjective(state,cellLocation,objs,list.at(objs).toDouble());

      emit dataChanged(index,index);
      return true;
    }
  return false;
} // setData
