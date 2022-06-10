#!/bin/sh
LD_LIBRARY_PATH=/data/code/qtbase/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
export LD_LIBRARY_PATH
QT_PLUGIN_PATH=/usr/lib/qt/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec /data/code/qtbase/bin/uic "$@"
