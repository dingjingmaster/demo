# QWebEngine

Qt WebEngine 提供了动态渲染网页内容的机制。

## Qt WebEngine 概述

```
Qt WebEngine Widgets         Qt WebEngine            Qt WebEngine Process
        |                       |                           |
       \|/                     \|/                         \|/
--------------------------------------------------------------------------
Qt WebEngine Core
--------------------------------------------------------------------------
Chromium
```
Qt WebEngine 中的功能分为以下模块
- 用于创建基于QWidget程序的模块
- _用于创建基于Qt Quick程序的模块(此处不考虑这块内容)_
- 用于与Chromium交互的核心模块

### Qt WebEngine Widgets模块

![qtwebenginewidgets-model.png](img/qtwebenginewidgets-model.png)

`Qt WebEngine View` 是 Qt WebEngine 模块的主要小部件。它用于加载网络内容（比如：网页）。在`view`中，web engine页面包含一个主框架，该主框架负责web内容、导航连接的历史记录和操作。`view`和`page`非常相似，因为它们提供了一组共同的功能。

所有`page`都属于`profile`，其中包含了`settings`、`cookies`。

> 注意：Qt WebEngine Widgets模块使用了 Qt Quick graph场景将网页元素组成一个`view`

内容是通过 GPU 渲染的。

### Qt WebEngine 模块

![](img/qtwebengine-model.png)

Qt WebEngine QML 实现包含与 Qt WebEngine Widgets 实现相同的元素，除了没有单独可访问的web engine widgets，支持的页面功能被集成到web engine view 中。

## Qt WebEngine 核心C++类

> 这些类是 QtWebEngine 和 QtWebEngineWidgets 共享的API调用

Qt WebEngine Core基于 Chromium 项目。Chromium提供自己的网络和渲染引擎。

Qt WebEngine Process 是一个单独的可执行文件，用于渲染网页和执行javascript。

## Qt WebEngine Widgets C++类

> 这些类用于在基于QWidget的应用程序中渲染Web内容
