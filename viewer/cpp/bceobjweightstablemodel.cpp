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

void BCEObjWeightsTableModel::nameEmptyLabels() {

  int numObjs = game->getNumObjectives();

  for (int obj = 0; obj < 2; obj++) {
    if (objectiveLabels[obj].empty())
      objectiveLabels[obj] = "Player " + std::to_string(obj);
  }

  int currObj = 2;

  while (currObj < numObjs) {
    if (objectiveLabels[currObj].empty())
      objectiveLabels[currObj] = "k";
    currObj++;
  }
}

void BCEObjWeightsTableModel::findLabelRedundancies(int obj) {

  string compareStr = objectiveLabels[obj];
  if (compareStr.empty())
    return;

  for (int i = 0; i < game->getNumObjectives(); i++) {
    if (i != obj) {
      if (compareStr.compare(objectiveLabels[i]) == 0) {
	objectiveLabels[obj] += "*";
	findLabelRedundancies(obj);
      }
    }
  }
}
