#include <QtWidgets>
#include "bcepayofftablemodel.hpp"

QVariant BCEPayoffTableModel::data(const QModelIndex & index,
				  int role) const 
{
  if (role == Qt::SizeHintRole)
    return QSize(1,1);
  else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {

      vector<int> action(2,0);
      action[0] = index.row();
      action[1] = index.column();

      return QVariant(QString::number(game->objective(state,action,0))
      		      +QString(", ")
      		      +QString::number(game->objective(state,action,1)));
	
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
      vector<int> actions(2,0);
      actions[0]= index.row();
      actions[1] = index.column();

      QRegExp rx("[, ]");
      QStringList list = value.toString().split(rx,QString::SkipEmptyParts);

      for (int player = 0; player < std::min(list.size(),2); player++)
	game->setObjective(state,actions,player,list.at(player).toDouble());

      emit dataChanged(index,index);
      return true;
    }
  return false;
} // setData
