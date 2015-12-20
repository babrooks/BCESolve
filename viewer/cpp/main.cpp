#include "bcewindow.hpp"
#include "bceloghandler.hpp"
#include "bcelogstream.hpp"

int main(int argc, char ** args)
{
  QApplication app(argc,args);

  BCELogHandler logHandler;
  // Redirect all subsequent cout to the BCELogHandler.
  BCELogStream qout(std::cout,logHandler.logText);

  BCEWindow window(logHandler);

  window.show();

  return app.exec();
}
