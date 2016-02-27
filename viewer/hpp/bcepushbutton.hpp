#ifndef BCEPUSHBUTTON_HPP
#define BCEPUSHBUTTON_HPP

#include <QtWidgets>

//! QPushButton that emits the player when clicked.
/*! This class is a QPushButton that emits the index
  of the player associated with it when it is clicked.

  \ingroup viewer
*/

class BCEPushButton : public QPushButton {
  Q_OBJECT;

private:
  //! Associated player
  int player;

public:

  //! Constructor
  BCEPushButton(int _player, QString _label, QWidget *parent):
    QPushButton(_label),player(_player)
  {
    connect(this,SIGNAL(clicked()),
	    this,SLOT(bceClick()));
    setParent(parent);
  }

signals:
  //! Signals when a player has clicked a BCEPushButton
  void clickedForPlayer(int player);

public slots:

  void bceClick() {
    emit(clickedForPlayer(player));
  }


};

#endif
