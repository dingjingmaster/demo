QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 link_pkgconfig no_keywords
PKGCONFIG += peony

INCLUDEPATH += \
    /usr/include/qt/QGSettings/ \
    /usr/include/aarch64-linux-gnu/qt5/QGSettings

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    navigation-bar.cpp \
    path-bar.cpp \
    side-bar.cpp \
    tool-bar.cpp

HEADERS += \
    mainwindow.h \
    navigation-bar.h \
    path-bar.h \
    side-bar.h \
    tool-bar.h

FORMS   += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
