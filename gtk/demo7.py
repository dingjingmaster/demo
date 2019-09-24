#!/usr/bin/env python
# coding=utf-8

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GObject

"""
    GObject,在 GTK+ 中是最基本的类型，为所有GTK+对象提供了共有的属性、方法和信号

    信号处理阶段(根据信号发送划分的)：
        1. GObject.SIGNAL_RUN_FIRST
        2. GObject.SIGNAL_RUN_LAST
        3. GObject.SIGNAL_RUN_CLEANUP

    属性的权限控制
        1. GObject.ParamFlags.READABLE
        2. GObject.ParamFlags.WRITABLE
        3. GObject.ParamFlags.READWRITE

"""

# 继承 GObject
class MyObject(GObject.GObject):

    def __init__(self):
        GObject.GObject.__init__(self)

# 创建新的信号
class MyObject(GObject.GObject):
    __gsignals__ = {
        # GObject.SIGNAL_FIRST 指出，这个信号将引用 do_my_signal() 来处理
        # 注意总结 信号处理的阶段
        # 第二个参数：信号的返回类型
        # 第三个参数：表示信号的参数
        'my_signal': (GObject.SIGNAL_RUN_FIRST, None,
                      (int,))
    }

    def do_my_signal(self, arg):
        print("method handler for `my_signal' called with argument", arg)
# 信号的发射 和 传惨
#my_obj.emit("my_signal", 42)

# 创建新属性
class MyObject(GObject.GObject):

    foo = GObject.Property(type=str, default='bar')
    # 通过添加flag给属性做读写权限限制
    foo = GObject.Property(type=str, flags = GObject.ParamFlags.READABLE) # not writable
    bar = GObject.Property(type=str, flags = GObject.ParamFlags.WRITABLE) # not readable
    property_float = GObject.Property(type=float)
    def __init__(self):
        GObject.GObject.__init__(self)


# 可自己创建只读属性
class MyObject(GObject.GObject):

    def __init__(self):
        GObject.GObject.__init__(self)

    @GObject.Property
    def readonly(self):
        return 'This is read-only.'

# 设置数字的最大最小值
# 属性必须定义在 GObject.GObject.__gproperties__ 字典中 
# 在do_get_property 和 do_set_property 中处理
class MyObject(GObject.GObject):

    __gproperties__ = {
        "int-prop": (int, # type
                     "integer prop", # nick
                     "A property that contains an integer", # blurb
                     1, # min
                     5, # max
                     2, # default
                     GObject.ParamFlags.READWRITE # flags
                    ),
    }

    def __init__(self):
        GObject.GObject.__init__(self)
        self.int_prop = 2

    def do_get_property(self, prop):
        if prop.name == 'int-prop':
            return self.int_prop
        else:
            raise AttributeError('unknown property %s' % prop.name)

    def do_set_property(self, prop, value):
        if prop.name == 'int-prop':
            self.int_prop = value
        else:
            raise AttributeError('unknown property %s' % prop.name)

# 监控属性
# 当属性被修改的时候，信号会被发射，这就是 "notify::property-name"
my_object = MyObject()

def on_notify_foo(obj, gparamstring):
    print("foo changed")

# 监测
my_object.connect("notify::foo", on_notify_foo)
my_object.set_property("foo", "bar") # on_notify_foo will be called




