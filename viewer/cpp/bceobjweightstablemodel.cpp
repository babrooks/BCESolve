#include <QtWidgets>
#include "bceobjweightstablemodel.hpp"

QVariant BCEObjWeightsTableModel::data(const QModelIndex & index,
				  int role) const
{
   if (role == Qt::SizeHintRole)
    return QSize(1,1);
  else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {

      int objective = index.row();

      return QVariant(QString::number(weightData[objective]));
	
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
      int objective = index.row();
      QString strValue = value.toString();
      setWeightData(objective,strValue.toDouble());

      emit dataChanged(index,index);
      return true;
    }
  return false;
} // setData
