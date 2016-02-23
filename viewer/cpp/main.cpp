#include "bcewindow.hpp"
#include "bceloghandler.hpp"
#include "bcelogstream.hpp"

int main(int argc, char ** args)
{
  QApplication app(argc,args);

  BCELogHandler logHandler;
  // Redirect all subsequent cout to the BCELogHandler.
  BCELogStream cout(std::cout,logHandler.logText);
  BCELogStream cerr(std::cerr,logHandler.logText);
  BCELogStream clog(std::clog,logHandler.logText);

  BCEWindow window(logHandler);

  window.show();

  return app.exec();
}
