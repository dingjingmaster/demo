#!/usr/bin/env python
# coding=utf-8

import sys
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, GLib, Gio, Gtk

"""
    Application
        Gtk.Application 包含很多可复用的功能，这些功能都是一个程序需要的。
        比如处理多个实例，d-bus激活、打开文件、命令行解析、启动/关闭、菜单管理、窗口管理等
        虽然Gtk.Application能够和普通Gtk.Windows一起正常的工作，不过通常建议与Gtk.ApplicationWindow一起使用。

    Action
        Gio.Action表示应用程序或部件的一个任务，通常有一个名字来标识这个任务。
        可以在运行时启用或者禁用Gio.Action，它们可以被激活或这改变状态（如果它们包含状态）。
        使用Actions的原因是为了将UI界面和业务逻辑进行分离。
        通常我们使用Gio.Action的子类Gio.SimpleAction来定义一个Action

        很多类，如Gio.MenuItem和Gtk.ModelButton支持在属性中设置Action名称。
        这些Action可以一起分组到Gio.ActionGroup并使用Gtk.Widget.insert_action_group（name, group)）
        将其添加到一个Widget中。这个name就是前缀。当添加到Gtk.ApplicationWindow中时这个前缀就是“win”。
        当你引用Action时，需要使用完整的Action名称，如“app.about”。
        你也可以很容易通过设置Gio.Menu文件中的“accel”属性或使用Gtk.Application.add_accelerator方法
        做出Action和按键绑定。

    Menu
        通常我们使用XML文件来定义菜单，并引用前面提到的Action。
        Gtk.Application使用Gtk.Application.set_app_menu（）或Gtk.Application.set_menubar（）方法来设置菜单。

    Command Line
        创建Gtk.Application需要Gio.ApplicationFlags参数。使用这个你可以让它处理它自身的所有行为，
        还可以处理更多的自定义行为。您可以
        使用HANDLES_COMMAND_LINE允许在Gio.Application.do_command_line方法中获取给定的命令并自定它要执行的事务。
        使用Gio.Application.add_main_option（）方法将命令添加到命令行中。
        使用HANDLES_OPEN会在Gio.Application.do_open（）方法打开你参数列表中所给的文件
        如果你的应用程序已经打开这些都将被发送到现有实例，除非你用NON_UNIQUE允许多个实例。
"""


# This would typically be its own file
MENU_XML="""
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <menu id="app-menu">
    <section>
      <attribute name="label" translatable="yes">Change label</attribute>
      <item>
        <attribute name="action">win.change_label</attribute>
        <attribute name="target">String 1</attribute>
        <attribute name="label" translatable="yes">String 1</attribute>
      </item>
      <item>
        <attribute name="action">win.change_label</attribute>
        <attribute name="target">String 2</attribute>
        <attribute name="label" translatable="yes">String 2</attribute>
      </item>
      <item>
        <attribute name="action">win.change_label</attribute>
        <attribute name="target">String 3</attribute>
        <attribute name="label" translatable="yes">String 3</attribute>
      </item>
    </section>
    <section>
      <item>
        <attribute name="action">win.maximize</attribute>
        <attribute name="label" translatable="yes">Maximize</attribute>
      </item>
    </section>
    <section>
      <item>
        <attribute name="action">app.about</attribute>
        <attribute name="label" translatable="yes">_About</attribute>
      </item>
      <item>
        <attribute name="action">app.quit</attribute>
        <attribute name="label" translatable="yes">_Quit</attribute>
        <attribute name="accel">&lt;Primary&gt;q</attribute>
    </item>
    </section>
  </menu>
</interface>
"""


class AppWindow(Gtk.ApplicationWindow):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        # This will be in the windows group and have the "win" prefix
        max_action = Gio.SimpleAction.new_stateful("maximize", None,
                                           GLib.Variant.new_boolean(False))
        max_action.connect("change-state", self.on_maximize_toggle)
        self.add_action(max_action)

        # Keep it in sync with the actual state
        self.connect("notify::is-maximized",
                            lambda obj, pspec: max_action.set_state(
                                               GLib.Variant.new_boolean(obj.props.is_maximized)))

        lbl_variant = GLib.Variant.new_string("String 1")
        lbl_action = Gio.SimpleAction.new_stateful("change_label", lbl_variant.get_type(),
                                               lbl_variant)
        lbl_action.connect("change-state", self.on_change_label_state)
        self.add_action(lbl_action)

        self.label = Gtk.Label(label=lbl_variant.get_string(),
                               margin=30)
        self.add(self.label)
        self.label.show()

    def on_change_label_state(self, action, value):
        action.set_state(value)
        self.label.set_text(value.get_string())

    def on_maximize_toggle(self, action, value):
        action.set_state(value)
        if value.get_boolean():
            self.maximize()
        else:
            self.unmaximize()


class Application(Gtk.Application):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, application_id="org.example.myapp",
                         flags=Gio.ApplicationFlags.HANDLES_COMMAND_LINE,
                         **kwargs)
        self.window = None

        self.add_main_option("test", ord("t"), GLib.OptionFlags.NONE,
                             GLib.OptionArg.NONE, "Command line test", None)

    def do_startup(self):
        Gtk.Application.do_startup(self)

        action = Gio.SimpleAction.new("about", None)
        action.connect("activate", self.on_about)
        self.add_action(action)

        action = Gio.SimpleAction.new("quit", None)
        action.connect("activate", self.on_quit)
        self.add_action(action)

        builder = Gtk.Builder.new_from_string(MENU_XML, -1)
        self.set_app_menu(builder.get_object("app-menu"))

    def do_activate(self):
        # We only allow a single window and raise any existing ones
        if not self.window:
            # Windows are associated with the application
            # when the last one is closed the application shuts down
            self.window = AppWindow(application=self, title="Main Window")

        self.window.present()

    def do_command_line(self, command_line):
        options = command_line.get_options_dict()
        # convert GVariantDict -> GVariant -> dict
        options = options.end().unpack()

        if "test" in options:
            # This is printed on the main instance
            print("Test argument recieved: %s" % options["test"])

        self.activate()
        return 0

    def on_about(self, action, param):
        about_dialog = Gtk.AboutDialog(transient_for=self.window, modal=True)
        about_dialog.present()

    def on_quit(self, action, param):
        self.quit()


if __name__ == "__main__":
    app = Application()
    app.run(sys.argv)

