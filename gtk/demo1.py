#!/usr/bin/env python3
# -*- coding:utf8 -*-

"""
   导入gtk模块,为了使用 GTK+ 的类和函数
   当有多个 GTK+ 版本安装时候,要确认版本
"""
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

# 创建空窗口
win = Gtk.Window()

# windows 事件和处理函数的连接
win.connect('destroy', Gtk.main_quit)

# 显示窗口
win.show_all()

# 开始gtk处理
Gtk.main()
