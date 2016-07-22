#!/bin/bash

rm -rf ./build/bceviewer.app
qmake
make
macdeployqt ./build/bceviewer.app
cp /usr/local/lib/libboost_serialization.dylib ./build/bceviewer.app/Contents/Frameworks
cp /usr/local/lib/libboost_system.dylib ./build/bceviewer.app/Contents/Frameworks
cp /usr/local/lib/libgurobi65.so ./build/bceviewer.app/Contents/Frameworks
