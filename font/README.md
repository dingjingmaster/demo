# 字体相关库

1. Pango是一个用于处理文本布局和渲染的开源库。它是 GNOME 桌面环境的一部分，用于提供跨平台的高质量文本渲染支持。

## Pango

### Pango 主要功能

- 文本布局
- 字体管理
- 文本渲染：将文本渲染为图像或绘制到图形上下文。支持抗锯齿、子像素渲染、多种渲染效果

### Pango相关库

- PangoCairo：支持Cairo
- PangoFc：Pango字体配置
- PangoFT2：支持FreeType
- PangoOT：支持OpenType
- PangoXft：支持 Xft

> 补充：<br/>
> Xft(X FreeType)是一个用于在 X11 图形系统上进行字体渲染的库。它结合了X11和FreeType，提供了高质量的字体渲染功能，以实现平滑和清晰的文字显示。<br/>
> Xft主要功能包括：<br/>
> 1. 字体渲染<br/>
> 2. 字体选择<br/>
> 3. 抗锯齿和子像素渲染

### Pango 依赖

- GObject-2.0
- HarfBuzz-0.0
