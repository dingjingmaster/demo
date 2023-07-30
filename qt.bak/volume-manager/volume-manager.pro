QT          -= gui

CONFIG      += c++17 console no_keywords link_pkgconfig
CONFIG      -= app_bundle

PKGCONFIG   += glib-2.0 udisks2 gudev-1.0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES     += \
        main.cpp \
        volume-manager.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS     += \
    volume-manager.h
