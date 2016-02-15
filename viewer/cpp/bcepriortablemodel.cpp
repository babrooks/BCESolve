#include "bcepriortablemodel.hpp"

QVariant BCEPriorTableModel::data(const QModelIndex & index,
				  int role) const
{
   if (role == Qt::SizeHintRole)
    return QSize(1,1);
  else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {

      int state = index.row();

      return QVariant(QString::number(game->prior(state)));
	
    }
  else
    return QVariant();
}

bool BCEPriorTableModel::setData(const QModelIndex & index,
				 const QVariant & value,
				 int role)
{
  if (role == Qt::EditRole)
    {
      int state = index.row();
      QString strValue = value.toString();
      game->setPrior(state,strValue.toDouble());

      emit dataChanged(index,index);
      return true;
    }
  return false;
} // setData

QVariant BCEPriorTableModel::headerData(int section,
					Qt::Orientation orientation,
					int role) const
{
  if (role == Qt::DisplayRole)
    {
      switch (orientation)
	{
	case Qt::Horizontal:
	  return QVariant(QString(""));

	case Qt::Vertical: {
	  return QVariant
	    (QString("S") + QString::number(section));
	}
	}
    }
  else
    return QVariant();
} // headerData
