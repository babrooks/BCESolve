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
      setWeightData(objective,strValue.toDouble(),type);

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
	case Qt::Horizontal:
	  return QVariant(QString(""));

	case Qt::Vertical: {
	  return QVariant
	    (QString::fromStdString(objectiveLabels[section]));
	}
	}
    }
  else
    return QVariant();
} // headerData

