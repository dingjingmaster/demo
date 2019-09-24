#!/usr/bin/env python
# coding=utf-8

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk


"""
    gtk+ 使用事件驱动程序模型

    handler_id = widget.connect("事件", 回调, 数据)
      widget 必须是 widget 的实例
      事件是定义好的

    widget.disconnect(handler_id)
    或:
    widget.disconnect_by_func(callback)

    调用 Gtk.main_quit() 导致 Gtk.main() 调用返回

    每个widget都有自己特有属性,要学习查看
      属性修改可以调用 set 和 get 函数
      或:
      widget.props.pros_name = value
      或:
      widget.get_property("属性名")
      widget.set_property("属性名", "值")

    查看widget有哪些属性
      widget = Gtk.Box()
      print(dir(widget.props))
"""

widget = Gtk.Box()
print(dir(widget.props))
