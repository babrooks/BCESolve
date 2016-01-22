#include <QtWidgets>
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

      QRegExp rx("[, ]");
      QStringList list = value.toString().split(rx,QString::SkipEmptyParts);

      int numObjectives = game->getNumObjectives();
      for (int objs = 0; objs < numObjectives; objs++)
	game->setObjective(state,cellLocation,objs,list.at(objs).toDouble());

      emit dataChanged(index,index);
      return true;
    }
  return false;
} // setData
