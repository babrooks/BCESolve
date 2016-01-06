#ifndef BCELOGHANDLER_HPP
#define BCELOGHANDLER_HPP

#include<QTextEdit>
#include<QVBoxLayout>
#include <QObject>

//! Log tab for the BCEViewer.
/*! This class handles the log tab in the viewer.
  It contains a QTextEdit for displaying log output.

  \ingroup viewer

 */
class BCELogHandler : public QObject {
  Q_OBJECT;

private:

  QVBoxLayout *layout;

public:

  QTextEdit *logText;

  BCELogHandler() {

    logText = new QTextEdit();
    layout = new QVBoxLayout();

    // logText->setTextFormat(Qt::LogText);

    layout->addWidget(logText);
  }

  QVBoxLayout* getLayout() {
    return layout;
  }

};

#endif
