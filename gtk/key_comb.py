#!/usr/bin/env python
# -*- coding=utf8 -*-
import gi
import pygame

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk
from pygame.locals import *

key_map = {
    'K_TAB': 'tab',
    'K_SPACE': 'space',
    'K_0': '0'
}


class KeyWindow(Gtk.Window):
    """
        主窗口: 置大小、添加按钮
    """

    def __init__(self):
        Gtk.Window.__init__(self, title='按键处理')
        self.set_border_width(1)
        self.set_default_size(400, 200)

        # 按下的按钮
        self.keyPress = ''

        # 主页按钮
        self.button = Gtk.Button.new_with_mnemonic('Hello world')
        self.button.connect('clicked', self.on_button_clicked_cb)
        self.add(self.button)

    def on_button_clicked_cb(self, widget):
        pygame.init()
        keyPressed = ''
        screen = pygame.display.set_mode((500, 200))
        while True:
            for ev in pygame.event.get():
                if QUIT == ev.type:
                    pygame.quit()
                    return ''
                elif KEYDOWN == ev.type:
                    keyPressed += str(ev.key) + '|'
                    print(str(ev))
                    print(ev.unicode)
                elif KEYUP == ev.type:
                    pygame.quit()
                    print(keyPressed)
                    return keyPressed


if __name__ == '__main__':
    win = KeyWindow()

    win.connect('delete-event', Gtk.main_quit)
    win.show_all()
    Gtk.main()
    exit(0)
