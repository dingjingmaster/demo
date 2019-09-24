#!/usr/bin/env python
# coding=utf-8

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

"""
    python 编码相关

    str 和 unicode：后者是unicode字符串(字节吗decode)、前者是字节码(encode 之后的)
    unicode.encode()

    python2 允许 unicode 和 str 相连接，但是解码很可能会导致错误

    Gtk+ 使用 UTF-8 编码后的字符串，也就是字节码

    未加 coding=utf8 时，Gtk.Label.set_text() 接受一个字符码，Gtk.Label.get_text()返回一个字节码

    python3 中默认unicode
"""

us = u'Fu\u00dfb\u00e4lle'
print (us)
type(us)

label = Gtk.Label()
unicode_string = u"Fu\u00dfb\u00e4lle"
label.set_text(unicode_string)
txt = label.get_text()
type(txt), txt
print (txt == unicode_string)
