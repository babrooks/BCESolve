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

