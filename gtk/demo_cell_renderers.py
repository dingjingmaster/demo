#!/usr/bin/env python
# -*- coding=utf-8 -*-

"""
CellRenderers:
    Gtk.CellRenderers Widget一般用来显示 widget 的信息，比如：Gtk.TreeView 或 Gtk.ComboBox。
    它与其他 widget 紧密协作,功能很强大，有很多配置选项来以不同方式显示大量数据。
    以下不同 widget 用于不同目的
        1. Gtk.CellRendererText
            使用其属性提供的字体、颜色和样式信息在其单元格中呈现给定的文本，如果文本太长并且“Ellipsize”属性允许，则文本将被省略
            默认这里的文字不允许修改，可以设置允许修改
            cell.set_property('editable', True)
        2. Gtk.CellRendererToggle
            在cell中渲染选择按钮，按钮样式是单选或多选,选项按钮依赖于“radio”属性。当激活的时候，它发射“toggled”信号
            它有两个状态：激活和未激活
        3. Gtk.CellRendererPixbuf
            可以用来在 cell 中渲染图片。
            它既允许渲染 Gdk.Pixbuf（设置pixbuf属性） 又允许渲染 icon（设置icon-name属性）
        4. Gtk.CellRendererCombo
            下拉选项框
        5. Gtk.CellRendererProgress
            进度条
        6. Gtk.CellRendererSpinner
            等待的 旋转 图标
        7. Gtk.CellRendererSpin
            加
        8. Gtk.CellRendererAccel
"""

import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, GLib, GObject


class CellRendererTextWindow(Gtk.Window):
    """
        demo
    """
    def __init__(self):
        Gtk.Window.__init__(self, title="CellRendererText 例子")

        # 设置默认窗口大小
        self.set_default_size(200, 200)

        self.liststore = Gtk.ListStore(str, str, str)
        self.liststore.append(["Fedora", "http://fedoraproject.org/", "aa"])
        self.liststore.append(["Slackware", "http://www.slackware.com/", "bb"])
        self.liststore.append(["Sidux", "http://sidux.com/", "cc"])

        treeview = Gtk.TreeView(model=self.liststore)

        renderer_text = Gtk.CellRendererText()
        column_text = Gtk.TreeViewColumn("Text", renderer_text, text=0)     # 第一列名
        treeview.append_column(column_text)                                 # 添加第一列

        renderer_editabletext = Gtk.CellRendererText()
        renderer_editabletext.set_property("editable", True)                # 第二列允许修改

        column_editabletext = Gtk.TreeViewColumn("Editable Text", renderer_editabletext, text=1)
        treeview.append_column(column_editabletext)                         # 添加第二列
        renderer_editabletext.connect("edited", self.text_edited)

        # 第三列，自己加
        renderer_3 = Gtk.CellRendererText()
        renderer_3.set_property('editable', True)
        column_3 = Gtk.TreeViewColumn('test', renderer_3, text=2)
        treeview.append_column(column_3)
        renderer_3.connect('edited', self.edit_three)

        self.add(treeview)

    def text_edited(self, widget, path, text):
        self.liststore[path][1] = text
        print(self.liststore)
        print(path)

    def edit_three(self, widget, path, text):
        self.liststore[path][2] = text
        print(path)


class CellRendererToggleWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="CellRendererToggle 例子")

        self.set_default_size(200, 200)

        self.liststore = Gtk.ListStore(str, bool, bool)
        self.liststore.append(["Debian", False, True])
        self.liststore.append(["OpenSuse", True, False])
        self.liststore.append(["Fedora", False, False])

        treeview = Gtk.TreeView(model=self.liststore)

        renderer_text = Gtk.CellRendererText()
        column_text = Gtk.TreeViewColumn("Text", renderer_text, text=0)
        treeview.append_column(column_text)

        renderer_toggle = Gtk.CellRendererToggle()
        renderer_toggle.connect("toggled", self.on_cell_toggled)

        column_toggle = Gtk.TreeViewColumn("Toggle", renderer_toggle, active=1)
        treeview.append_column(column_toggle)

        renderer_radio = Gtk.CellRendererToggle()
        renderer_radio.set_radio(True)
        renderer_radio.connect("toggled", self.on_cell_radio_toggled)

        column_radio = Gtk.TreeViewColumn("Radio", renderer_radio, active=2)
        treeview.append_column(column_radio)

        self.add(treeview)

    def on_cell_toggled(self, widget, path):
        self.liststore[path][1] = not self.liststore[path][1]

    def on_cell_radio_toggled(self, widget, path):
        selected_path = Gtk.TreePath(path)
        print(selected_path)
        for row in self.liststore:
            row[2] = (row.path == selected_path)    # 置选中的为 1 其他为0


class CellRendererPixbufWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="CellRendererPixbuf Example")

        self.set_default_size(200, 200)

        self.liststore = Gtk.ListStore(str, str)
        self.liststore.append(["New", "document-new"])
        self.liststore.append(["Open", "document-open"])
        self.liststore.append(["Save", "document-save"])

        treeview = Gtk.TreeView(model=self.liststore)

        renderer_text = Gtk.CellRendererText()
        column_text = Gtk.TreeViewColumn("Text", renderer_text, text=0)
        treeview.append_column(column_text)

        renderer_pixbuf = Gtk.CellRendererPixbuf()

        column_pixbuf = Gtk.TreeViewColumn("Image", renderer_pixbuf, icon_name=1)
        treeview.append_column(column_pixbuf)

        self.add(treeview)


class CellRendererComboWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="CellRendererCombo Example")

        self.set_default_size(200, 200)

        liststore_manufacturers = Gtk.ListStore(str)
        manufacturers = ["Sony", "LG",
            "Panasonic", "Toshiba", "Nokia", "Samsung"]
        for item in manufacturers:
            liststore_manufacturers.append([item])

        self.liststore_hardware = Gtk.ListStore(str, str)
        self.liststore_hardware.append(["Television", "Samsung"])
        self.liststore_hardware.append(["Mobile Phone", "LG"])
        self.liststore_hardware.append(["DVD Player", "Sony"])

        treeview = Gtk.TreeView(model=self.liststore_hardware)

        renderer_text = Gtk.CellRendererText()
        column_text = Gtk.TreeViewColumn("Text", renderer_text, text=0)
        treeview.append_column(column_text)

        renderer_combo = Gtk.CellRendererCombo()
        renderer_combo.set_property("editable", True)
        renderer_combo.set_property("model", liststore_manufacturers)
        renderer_combo.set_property("text-column", 0)
        renderer_combo.set_property("has-entry", False)
        renderer_combo.connect("edited", self.on_combo_changed)

        column_combo = Gtk.TreeViewColumn("Combo", renderer_combo, text=1)
        treeview.append_column(column_combo)

        self.add(treeview)

    def on_combo_changed(self, widget, path, text):
        self.liststore_hardware[path][1] = text


class CellRendererProgressWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="CellRendererProgress Example")

        self.set_default_size(200, 200)

        self.liststore = Gtk.ListStore(str, int, bool)
        self.current_iter = self.liststore.append(["Sabayon", 0, False])
        self.liststore.append(["Zenwalk", 0, False])
        self.liststore.append(["SimplyMepis", 0, False])

        treeview = Gtk.TreeView(model=self.liststore)

        renderer_text = Gtk.CellRendererText()
        column_text = Gtk.TreeViewColumn("Text", renderer_text, text=0)
        treeview.append_column(column_text)

        renderer_progress = Gtk.CellRendererProgress()
        column_progress = Gtk.TreeViewColumn("Progress", renderer_progress,
            value=1, inverted=2)
        treeview.append_column(column_progress)

        renderer_toggle = Gtk.CellRendererToggle()
        renderer_toggle.connect("toggled", self.on_inverted_toggled)
        column_toggle = Gtk.TreeViewColumn("Inverted", renderer_toggle,
            active=2)
        treeview.append_column(column_toggle)

        self.add(treeview)

        self.timeout_id = GLib.timeout_add(100, self.on_timeout, None)

    def on_inverted_toggled(self, widget, path):
        self.liststore[path][2] = not self.liststore[path][2]

    def on_timeout(self, user_data):
        new_value = self.liststore[self.current_iter][1] + 1
        if new_value > 100:
            self.current_iter = self.liststore.iter_next(self.current_iter)
            if self.current_iter is None:
                self.reset_model()
            new_value = self.liststore[self.current_iter][1] + 1

        self.liststore[self.current_iter][1] = new_value
        return True

    def reset_model(self):
        for row in self.liststore:
            row[1] = 0
        self.current_iter = self.liststore.get_iter_first()


class CellRendererSpinWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="CellRendererSpin Example")

        self.set_default_size(200, 200)

        self.liststore = Gtk.ListStore(str, int)
        self.liststore.append(["Oranges", 5])
        self.liststore.append(["Apples", 4])
        self.liststore.append(["Bananas", 2])

        treeview = Gtk.TreeView(model=self.liststore)

        renderer_text = Gtk.CellRendererText()
        column_text = Gtk.TreeViewColumn("Fruit", renderer_text, text=0)
        treeview.append_column(column_text)

        renderer_spin = Gtk.CellRendererSpin()
        renderer_spin.connect("edited", self.on_amount_edited)
        renderer_spin.set_property("editable", True)

        adjustment = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        renderer_spin.set_property("adjustment", adjustment)

        column_spin = Gtk.TreeViewColumn("Amount", renderer_spin, text=1)
        treeview.append_column(column_spin)

        self.add(treeview)

    def on_amount_edited(self, widget, path, value):
        self.liststore[path][1] = int(value)

def cell_render_text():
    win = CellRendererTextWindow()
    win.connect("destroy", Gtk.main_quit)
    win.show_all()
    Gtk.main()


def cell_render_toggle():
    win = CellRendererToggleWindow()
    win.connect("destroy", Gtk.main_quit)
    win.show_all()
    Gtk.main()


def cell_render_pixbuf():
    win = CellRendererPixbufWindow()
    win.connect("destroy", Gtk.main_quit)
    win.show_all()
    Gtk.main()


def cell_render_combo():
    win = CellRendererComboWindow()
    win.connect("destroy", Gtk.main_quit)
    win.show_all()
    Gtk.main()


def cell_render_progress():
    win = CellRendererProgressWindow()
    win.connect("destroy", Gtk.main_quit)
    win.show_all()
    Gtk.main()


def cell_render_spin():
    win = CellRendererSpinWindow()
    win.connect("destroy", Gtk.main_quit)
    win.show_all()
    Gtk.main()


def cell_render_accel():
    out = []
    dlg = Gtk.MessageDialog(buttons=Gtk.ButtonsType.CLOSE)
    message = "按一.\n""松开窗口关闭."
    dlg.set_markup(message)
    dlg.set_title("Please press a key (or a key combination)")
    sw = Gtk.ScrolledWindow()

    def __accel_edited_cb(c, path, keyval, state, keycode):
        out.append(keyval)
        out.append(state)
        out.append(keycode)
        print('--------->')
        print('keyval:' + str(keyval))
        print('state:' + str(state))
        print('keycode:' + str(keycode))
        if len(out) >= 3:
            shortcut = Gtk.accelerator_name_with_keycode(None, keyval, keycode, state)
            print('shortcut:' + str(shortcut))
        print('--------<')

        dlg.response(Gtk.ResponseType.OK)

    model = Gtk.ListStore(GObject.TYPE_INT, GObject.TYPE_UINT, GObject.TYPE_UINT)
    accel_view = Gtk.TreeView(model=model)
    accel_view.set_headers_visible(False)
    sw.add(accel_view)
    sw.set_min_content_height(30)
    column = Gtk.TreeViewColumn()
    renderer = Gtk.CellRendererAccel(accel_mode=Gtk.CellRendererAccelMode.OTHER,
                                     accel_mods=Gdk.KEY_Shift_L,
                                     editable=True)
    renderer.connect('accel-edited', __accel_edited_cb)
    column.pack_start(renderer, True)
    # column.add_attribute(renderer, 'accel-mods', 0)
    # column.add_attribute(renderer, 'accel-key', 1)
    # column.add_attribute(renderer, 'keycode', 2)
    accel_view.append_column(column)
    it = model.append(None)
    area = dlg.get_message_area()
    area.pack_end(sw, True, True, 0)
    sw.show_all()
    id = dlg.run()
    if id != Gtk.ResponseType.OK or len(out) < 3:
        print('退出')
        exit(1)
    keyval = out[0]
    state = out[1]
    keycode = out[2]

    shortcut = Gtk.accelerator_name_with_keycode(None, keyval, keycode, state)
    # print(shortcut)
    Gtk.main()


if __name__ == '__main__':
    # cell_render_text()
    # cell_render_toggle()
    # cell_render_pixbuf()
    cell_render_combo()
    # cell_render_progress()
    # cell_render_spin()
    # cell_render_accel()
    exit(0)



