#!/usr/bin/env python
# coding=utf-8

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

# 类需要继承 Gtk.Window
class MyWindow(Gtk.Window):

    def __init__(self):
        # 调用父类构造,修改标题
        Gtk.Window.__init__(self, title="Hello World")

        # 创建button控件,链接点击信号
        self.button = Gtk.Button(label="Click Here")
        self.button.connect("clicked", self.on_button_clicked)
        self.add(self.button)

    def on_button_clicked(self, widget):
        print("Hello World")

win = MyWindow()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()



