# OpenGL

## OpenGL相关库

### OpenGL(Open Graphics Library)

提供跨平台的2D和3D图形渲染功能，是图形开发的基础库。

特点：仅定义了图形处理的API接口，本身不负责窗口管理或用户交互。

### GLUT(OpenGL Utility Toolkit)

提供跨平台的窗口创建、事件处理和基本图形功能，方便快速创建OpenGL应用。

特点：适合学习和小型项目，但不够灵活，已逐渐被GLFW等替代。

### GLFW

提供窗口创建、输入处理和上下文管理功能，是现代OpenGL开发的常用工具。

特点：轻量、高效，支持多线程和高分辨率显示器

### GLEW(OpenGL Extension Wrangler Library)

帮助开发者加载和使用不同平台上的OpenGL扩展功能。

解决了不同显卡和驱动对OpenGL版本和扩展支持的兼容问题

### GLM(OpenGL Mathematics Library)

提供教学功能（如向量、矩阵、变换等）以支持OpenGL的3D图形运算

特点：基于 C++，语法与GLSL一致，便于开发者统一代码逻辑。

### FreeType

用于在 OpenGL程序中渲染字体和文本

特点：提供高质量的字体处理功能，支持多种字体格式

### SOIL(Simple OpenGL Image Library)或stb_image

加载纹理图像(如：PNG、JPEG)到OpenGL程序中

特点：轻量、简单、适合快速集成。

### Assimp(Open Asset Import Library)

加载3D模型文件(如OBJ、FBX)并与OpenGL集成

特点：支持多种文件格式，简化3D模型的加载和处理

### ImGui(immediate Mode GUI)

提供简单的图形用户界面(GUI)以便调试或构建工具界面

特点：与OpenGL无缝集成，轻量且易于使用

