#ifndef BCELOGHANDLER_HPP
#define BCELOGHANDLER_HPP

#include<QTextEdit>
#include<QVBoxLayout>
#include "bcelogstream.hpp"
#include <QObject>

class BCELogHandler : public QObject {
  Q_OBJECT;

private:

  QVBoxLayout *layout;

public:

  QTextEdit *logText;

  BCELogHandler() {

    logText = new QTextEdit(tr("Solver Log Tab"));
    layout = new QVBoxLayout();

    // logText->setTextFormat(Qt::LogText);
    BCELogStream qout(std::cout, logText);

    layout->addWidget(logText);
  }

  QVBoxLayout* getLayout() {
    return layout;
  }

};

#endif
