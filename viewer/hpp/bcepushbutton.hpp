#ifndef BCEPUSHBUTTON_HPP
#define BCEPUSHBUTTON_HPP

#include <QtWidgets>

//! QPushButton that emits the player when clicked.
/*!

  \ingroup viewer
*/

class BCEPushButton : public QPushButton {
  Q_OBJECT;

private:
  int player;

public:

  BCEPushButton(int _player, QString _label):
    QPushButton(_label),player(_player)
  {
    connect(this,SIGNAL(clicked()),
	    this,SLOT(bceClick()));
  }

signals:
  void clickedForPlayer(int player);

public slots:

  void bceClick() {
    emit(clickedForPlayer(player));
  }


};

#endif
