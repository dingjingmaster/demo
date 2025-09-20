#!/bin/bash

set +e

./configure -v -recheck-all -prefix /opt/dev/qt-5.15.15-gcc8 \
        -opensource -confirm-license -release \
        -shared \
        -c++std c++17 \
        -nomake examples \
        -opengl -glib -gui -widgets \
	-dbus -ibus -gtk \
	-fcitx -fcitx5 \
        -icu \
        -ssl \
        -qpa xcb \
        -xcb-xlib \
        -qt-freetype \
        -xkbcommon \
	-webengine-icu \
	-feature-im \
        -feature-dialog \
        -feature-qdbus \
        -feature-translation \
        -feature-webengine-widgets \
        -feature-webengine-webchannel \
        -feature-build-qtwebengine-core

