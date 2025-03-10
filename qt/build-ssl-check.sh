#!/bin/bash

g++ -o check.run ./ssl-check.cpp -I /opt/dev/qt-5.15.15/include/ -I /opt/dev/qt-5.15.15/include/QtCore/ -I /opt/dev/qt-5.15.15/include/QtNetwork/ -L /opt/dev/qt-5.15.15/lib/ -lQt5Core -lQt5Network
