QT -= gui core

CONFIG += console no_keywords link_pkgconfig c++11
CONFIG -= app_bundle

DEFINES     += PYTHON_HOME='\\"/data/env/python2.7\\"'

QMAKE_CXXFLAGS  +=  \
    -I/data/env/python2.7/include/python2.7  \
    -Wl,-rpath,/data/env/python2.7/lib/      \


INCLUDEPATH += /data/env/python2.7/include/python2.7

LIBS        += -L/data/env/python2.7/lib -lpython2.7 -lpthread

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        python-packer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    python-packer.h
