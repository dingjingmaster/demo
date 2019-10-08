#!/usr/bin/env python
# -*- coding=utf-8 -*-
import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, GLib, GObject

if __name__ == '__main__':
    builder = Gtk.Builder()
    builder.add_from_file('ibus_shortcut.glade')

    window = builder.get_object('window1')
    window.show_all()

    Gtk.main()
    exit(0)
