# This file is part of the BCESolve library for games of incomplete
# information
# Copyright (C) 2016 Benjamin A. Brooks and Robert J. Minton
#
# BCESolve free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# BCESolve is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see
# <http://www.gnu.org/licenses/>.
#
# Benjamin A. Brooks
# ben@benjaminbrooks.net
# Chicago, IL

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
QMAKE_MAC_SDK = macosx10.11

QT += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += release
CONFIG += -std=gnu++11
CONFIG += WARN_OFF
CONFIG += debug

QMAKE_CXXFLAGS += -std=gnu++11
QMAKE_CXXFLAGS += -lm -lpthread  -O0 -O -fPIC -fexceptions
QMAKE_LFLAGS += -lm -lpthread  -O -fPIC -fexceptions

INCLUDEPATH += /usr/local/include \
/usr/local/lib \
../src/cpp \
../src/hpp \
../viewer/cpp \
../viewer/hpp \
../examples/hpp \
/usr/include/boost \
"C:\Program Files\boost\boost_1_60_0"

VPATH += ../src/cpp \
../src/hpp \
/usr/local/include \
/usr/local/lib \
/usr/lib/x86_64-linux-gnu \
../viewer/cpp \
../viewer/hpp \
/usr/include/boost

HEADERS += bcedatastate.hpp \
bceplothandler.hpp \
qcustomplot.h \
bcesolution.hpp \
bcegame.hpp \
bce.hpp \
bcedistr.hpp \
bceabstractgame.hpp \
bceslider.hpp \
bcelineedit.hpp \
bcevaluesetplot.hpp \
bceenumeration.hpp \
bcelabel.hpp \
bcegamehandler.hpp \
bcepayofftablemodel.hpp \
bcetableview.hpp \
bcetablemodel.hpp \
bcepriortablemodel.hpp \
bceconditionaltablemodel.hpp \
bcepushbutton.hpp \
bcewindow.hpp \
bcelogstream.hpp \
bceloghandler.hpp \
bceobjweightstablemodel.hpp \
bcesolverworker.hpp \
bcegurobicallback.hpp \
bceplottitle.hpp

SOURCES += main.cpp \
bceplothandler.cpp \
qcustomplot.cpp \
bcedatastate.cpp \
bcegamehandler.cpp \
bcepayofftablemodel.cpp \
bcepriortablemodel.cpp \
bceconditionaltablemodel.cpp \
bcewindow.cpp \
bceobjweightstablemodel.cpp \
bcesolver.cpp \
bcelabel.cpp \
bceplottitle.cpp

LIBS += ../lib/libbce.a

include(../localviewersettings.mk)

linux-g++-64 {
LIBS +=  -Bstatic -L../lib -Wl,-Bstatic  -L/usr/lib/x86_64-linux-gnu/ -lboost_serialization -lboost_system -Wl,-Bdynamic
LIBS += -L/usr/lib/x86_64-linux-gnu/ -L/usr/local/lib -lboost_serialization -lboost_system
}

linux-g++ {
LIBS +=  -Bstatic -L../lib -Wl,-Bstatic  -L/usr/lib/x86_64-linux-gnu/ -lboost_serialization -lboost_system -Wl,-Bdynamic
LIBS += -L/usr/lib/x86_64-linux-gnu/ -L/usr/local/lib -lboost_serialization -lboost_system
}

macx {
LIBS +=
LIBS += -L/usr/lib/x86_64-linux-gnu/ -L/usr/local/lib -lboost_serialization -lboost_system
}
win32 {
LIBS += -L"C:\Program Files\boost\boost_1_60_0\bin.v2\libs\serialization\build\gcc-mingw-4.9.2\debug\link-static\threading-multi" \
 -lboost_serialization-mgw49-mt-d-1_60 -L../lib/ -lbce
}

LIBS += -L../lib/ -L/usr/local/lib/ -L/usr/lib/x86_64-linux-gnu

DESTDIR = ./build

MOC_DIR = ./tmp
OBJECTS_DIR = ./tmp

TEMPLATE = app
TARGET = bceviewer
