#include "bceplothandler.hpp"

int main(int argc, char ** args)
{
  QApplication app(argc,args);

  BCEPlotHandler window;

  window.show();

  return app.exec();
}
