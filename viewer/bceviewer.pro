######################################################################
# Automatically generated by qmake (3.0) Sun Jul 12 21:59:06 2015
######################################################################

QT += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += release
CONFIG += -std=gnu++11
CONFIG += WARN_OFF
CONFIG += debug

QMAKE_CXXFLAGS += -std=c++11
 
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/lib
INCLUDEPATH += ../src/cpp
INCLUDEPATH += ../src/hpp
INCLUDEPATH += ../viewer/cpp
INCLUDEPATH += ../viewer/hpp
INCLUDEPATH += /opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/concert/include
INCLUDEPATH += /opt/ibm/ILOG/CPLEX_Enterprise_Server1262/CPLEX_Studio/cplex/include
VPATH += ../src/cpp
VPATH += ../src/hpp
VPATH += /usr/local/include
VPATH += /usr/local/lib 
VPATH += ../viewer/cpp
VPATH += ../viewer/hpp

HEADERS += bcedatastate.hpp \
bceplothandler.hpp \
qcustomplot.h \
bcesolution.hpp \
bcegame.hpp \
bceabstractgame.hpp \
bceslider.hpp \
bcelineedit.hpp \
bcecheckbox.hpp \
bcevaluesetplot.hpp \
bceenumeration.hpp \
bcelabelhandler.hpp \
bcegamehandler.hpp \
bcepayofftablemodel.hpp \
bcetableview.hpp \
bcetablemodel.hpp

SOURCES += main.cpp \
bceplothandler.cpp \
qcustomplot.cpp \
bceabstractgame.cpp \
bcesolution.cpp \
bcegame.cpp \
bcedatastate.cpp \
bcegamehandler.cpp \
bcepayofftablemodel.cpp 
LIBS += ../lib/libbce.a

LIBS += -L../lib/ -L/usr/local/lib/ /usr/lib/x86_64-linux-gnu/libboost_serialization.a /usr/lib/x86_64-linux-gnu/libboost_system.a /usr/lib/x86_64-linux-gnu/libboost_filesystem.a 
linux-g++ {
LIBS +=  -Bstatic -L../lib -Wl,-Bstatic  -L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem -lboost_serialization -lboost_system -Wl,-Bdynamic
}
macx {
LIBS += -lc++ /usr/local/lib/libboost_serialization.a 
}
TEMPLATE = app
TARGET = bceviewer

