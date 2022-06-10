#!/usr/bin/env python
# -*- coding=utf-8 -*-
import sys
from gi.repository import GLib, Gio

if len(sys.argv) < 2:
   sys.stderr.write("Specify number of seconds\n")
   sys.exit(1)

if len(sys.argv) > 2:
   sys.stderr.write("Too many arguments\n")
   sys.exit(1)

connection = Gio.bus_get_sync(Gio.BusType.SESSION, None)
proxy = Gio.DBusProxy.new_sync(connection,
                               Gio.DBusProxyFlags.NONE,
                               None,
                               "es.aleksander.Alarm",
                               "/es/aleksander/Alarm",
                               "es.aleksander.Alarm",
                               None)
try:
    proxy.call_sync("Configure",
                    GLib.Variant("(u)", (int(sys.argv[1]),)),
                    Gio.DBusCallFlags.NONE,
                    -1,
                    None)
except Exception as e:
    sys.stderr.write("Error: %s\n" % str(e))
else:
    print ("Done")
