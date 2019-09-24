#!/usr/bin/env python
# coding=utf-8

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, GdkPixbuf

"""
    gtk widget
        Gtk.AboutDialog
        Gtk.AccelLabel
        Gtk.ActionBar
        Gtk.AppChooserButton
        Gtk.AppChooserDialog
        Gtk.Assistant
        Gtk.Button
        Gtk.checkButton
        Gtk.ColorButton
        Gtk.ColorChooserDialog
        Gtk.ComboBox
        Gtk.comboBoxText
        Gtk.Entry
        Gtk.FileChooserButton
        Gtk.FileChooserDialog
        Gtk.FlowBox
        Gtk.FontButton
        Gtk.FontChooserDialog
        Gtk.Frame
        Gtk.GLArea
        Gtk.Grid
        Gtk.HeaderBar
        Gtk.IconView
        Gtk.Image
        Gtk.InfoBar
        Gtk.Label
        Gtk.LevelBar
        Gtk.LinkButton
        Gtk.ListBox
        Gtk.LockButton
        Gtk.MenuBar
        Gtk.MenuButton
        Gtk.MessageDialog
        Gtk.Notebook
        Gtk.Paned
        Gtk.PlacesSidebar
        Gtk.ProgressBar
        Gtk.RadioButton
        Gtk.RecentChooserDialog
        Gtk.Scale
        Gtk.Scrollbar
        Gtk.ScrolledWindow
        Gtk.SearchBar
        Gtk.SearchEntry
        Gtk.Separator
        Gtk.SpinButton
        Gtk.Spinner
        Gtk.Stack
        Gtk.StackSwitcher
        Gtk.Statusbar
        Gtk.Switch
        Gtk.TextView
        Gtk.ToggleButton
        Gtk.ToolPalette
        Gtk.Toolbar
        Gtk.TreeView
        Gtk.VolumeButton
        Gtk.Window
    
    拖拽信号
        拖拽源处信号
            drag-begin
            drag-data-get
            drag-data-delete
            drag-end
        拖拽目的处信号
            drag-motion
            drag-drop
            drag-data-received
        
"""
(TARGET_ENTRY_TEXT, TARGET_ENTRY_PIXBUF) = range(2)
(COLUMN_TEXT, COLUMN_PIXBUF) = range(2)

DRAG_ACTION = Gdk.DragAction.COPY

# 窗口拖动主窗体
class DragDropWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="Drag and Drop Demo")

        # 水平两列
        vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
        self.add(vbox)

        # 水平上部分窗口，一个拖动源，一个拖动目的
        hbox = Gtk.Box(spacing=12)
        vbox.pack_start(hbox, True, True, 0)

        self.iconview = DragSourceIconView()  # 源
        self.drop_area = DropArea()           # 目的

        hbox.pack_start(self.iconview, True, True, 0)
        hbox.pack_start(self.drop_area, True, True, 0)

        # 水平下部分窗口，两个按钮，表示传递的类型
        button_box = Gtk.Box(spacing=6)
        vbox.pack_start(button_box, True, False, 0)

        image_button = Gtk.RadioButton.new_with_label_from_widget(None,
            "Images")
        image_button.connect("toggled", self.add_image_targets)
        button_box.pack_start(image_button, True, False, 0)

        text_button = Gtk.RadioButton.new_with_label_from_widget(image_button,
            "Text")
        text_button.connect("toggled", self.add_text_targets)
        button_box.pack_start(text_button, True, False, 0)

        self.add_image_targets()

    def add_image_targets(self, button=None):
        targets = Gtk.TargetList.new([])
        targets.add_image_targets(TARGET_ENTRY_PIXBUF, True)

        self.drop_area.drag_dest_set_target_list(targets)
        self.iconview.drag_source_set_target_list(targets)

    def add_text_targets(self, button=None):
        self.drop_area.drag_dest_set_target_list(None)
        self.iconview.drag_source_set_target_list(None)

        self.drop_area.drag_dest_add_text_targets()
        self.iconview.drag_source_add_text_targets()

class DragSourceIconView(Gtk.IconView):

    def __init__(self):
        Gtk.IconView.__init__(self)
        self.set_text_column(COLUMN_TEXT)
        self.set_pixbuf_column(COLUMN_PIXBUF)

        model = Gtk.ListStore(str, GdkPixbuf.Pixbuf)
        self.set_model(model)
        self.add_item("Item 1", "image-missing")
        self.add_item("Item 2", "help-about")
        self.add_item("Item 3", "edit-copy")

        self.enable_model_drag_source(Gdk.ModifierType.BUTTON1_MASK, [],
            DRAG_ACTION)
        self.connect("drag-data-get", self.on_drag_data_get)

    def on_drag_data_get(self, widget, drag_context, data, info, time):
        selected_path = self.get_selected_items()[0]
        selected_iter = self.get_model().get_iter(selected_path)

        if info == TARGET_ENTRY_TEXT:
            text = self.get_model().get_value(selected_iter, COLUMN_TEXT)
            data.set_text(text, -1)
        elif info == TARGET_ENTRY_PIXBUF:
            pixbuf = self.get_model().get_value(selected_iter, COLUMN_PIXBUF)
            data.set_pixbuf(pixbuf)

    def add_item(self, text, icon_name):
        pixbuf = Gtk.IconTheme.get_default().load_icon(icon_name, 16, 0)
        self.get_model().append([text, pixbuf])


class DropArea(Gtk.Label):

    def __init__(self):
        Gtk.Label.__init__(self)
        self.set_label("Drop something on me!")
        self.drag_dest_set(Gtk.DestDefaults.ALL, [], DRAG_ACTION)

        self.connect("drag-data-received", self.on_drag_data_received)

    def on_drag_data_received(self, widget, drag_context, x,y, data,info, time):
        if info == TARGET_ENTRY_TEXT:
            text = data.get_text()
            print("Received text: %s" % text)

        elif info == TARGET_ENTRY_PIXBUF:
            pixbuf = data.get_pixbuf()
            width = pixbuf.get_width()
            height = pixbuf.get_height()

            print("Received pixbuf with width %spx and height %spx" % (width,
                height))

# 窗口拖动主窗体
win = DragDropWindow()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()

