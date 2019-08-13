//
// Created by dingjing on 2019/8/12.
//

/**
 * 双缓冲(Double Buffer)
 *  应用程序使用单缓冲绘图时可能会存在图像闪烁的问题。
 *  这是因为生成的图像不是一下子被绘制出来的，而是按照从左到右，由上而下逐像素地绘制而成的。
 *  最终图像不是在瞬间显示给用户，而是通过一步一步生成的，这会导致渲染的结果很不真实。
 *  为了规避这些问题，我们应用双缓冲渲染窗口应用程序。
 *  前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
 *  当所有的渲染指令执行完毕后，我们交换(Swap)前缓冲和后缓冲，这样图像就立即呈显出来，之前提到的不真实感就消除了。
 *
 */
#ifndef OPENGL_CREATING_WINDOW_H
#define OPENGL_CREATING_WINDOW_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int demo_hello() {
    /* glfw 窗口实例化 */
    glfwInit();                                                         // 初始化 GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);              // 配置窗口：设置opengl朱版本和次版本都是 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      // 使用核心模式

#ifdef __APPLE__        // mac 系统需要添加起作用
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // 创建窗口：宽度、高度、标题名 最后两暂时不解释
    // 返回一个 GLFWWindow 对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("创建窗口失败");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 允许注册一个灰调函数，在每次改变窗口大小的时候被调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD 用来管理 openGL 的函数指针，所以在调用任何 openGL 函数之前都要初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("初始化 glad 失败");
        return -1;
    }

    // 拒绝绘制图像后直接退出，在主动关闭之前，不断绘制图像并接受用户输入
    // glfwPollEvents 检查是否触发了什么事件（比如：键盘输入、鼠标移动、更新窗口状态，并调用对应的回调函数）
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 每次渲染前要清屏
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);       // 设置清屏所用的颜色
        glClear(GL_COLOR_BUFFER_BIT);               // 清除什么？GL_STENCIL_BUFFER_BIT GL_DEPTH_BUFFER_BIT GL_COLOR_BUFFER_BIT

        // 交换颜色缓存（它存储这 GLFW 窗口每一个像素颜色值）
        glfwSwapBuffers(window);
        // glfwPollEvents 检查是否触发了什么事件（比如：键盘输入、鼠标移动、更新窗口状态，并调用对应的回调函数）
        glfwPollEvents();
    }

    // 销毁所有 GLFW 分配的资源
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    // 监听 Esc 按键
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 要渲染的窗口大小，前两参数控制窗口左下角的位置，第三第四参数控制渲染窗口的大小
    glViewport(0, 0, width, height);
}

#endif //OPENGL_CREATING_WINDOW_H
