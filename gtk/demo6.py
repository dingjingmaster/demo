#!/usr/bin/env python
# coding=utf-8

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

"""
    glade 和 Gtk.Builder

    glade 文件是一种 xml 文件，用来描述界面
    加载界面
        builder = Gtk.Builder()
        builder.add_from_file("xxx.glade")

        builder.add_objects_from_file("xxx.glade", ("button1", "button2"))

    # glade 连接信号(下例) Handler 可以是字典，键为字符串 值为函数
"""
class Handler:
    def onDestroy(self, *args):
        Gtk.main_quit()

    def onButtonPressed(self, button):
        print("Hello World!")

builder = Gtk.Builder()
builder.add_from_file("builder_example.glade")
builder.connect_signals(Handler())

window = builder.get_object("window1")
window.show_all()
Gtk.main()


