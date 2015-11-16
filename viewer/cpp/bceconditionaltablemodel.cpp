#include <QtWidgets>
#include "bceconditionaltablemodel.hpp"

QVariant BCEConditionalTableModel::data(const QModelIndex & index,
				  int role) const
{
   if (role == Qt::SizeHintRole)
    return QSize(1,1);
  else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {

      vector<int> types(2,0);
      types[0] = index.row();
      types[1] = index.column();

      return QVariant(QString::number(game->conditional(state,types)));
	
    }
  else
    return QVariant();
}

QVariant BCEConditionalTableModel::headerData(int section,
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

bool BCEConditionalTableModel::setData(const QModelIndex & index,
				 const QVariant & value,
				 int role)
{
  if (role == Qt::EditRole)
    {
      vector<int> types(2,0);
      types[0] = index.row();
      types[1] = index.column();
 
      QString strValue = value.toString();
      game->setConditional(state,types,strValue.toDouble());

      emit dataChanged(index,index);
      return true;
    }
  return false;
} // setData
