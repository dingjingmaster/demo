/*************************************************************************
> FileName: gl-glfw1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu 04 Jan 2024 04:03:17 PM CST
 ************************************************************************/
#include "glad.h"

#include <stdio.h>
#include <GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main ()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 创建窗口
    // 前两参数分别是窗口 长、宽
    // 第三参数是窗口标题
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // 通知GLFW将我们的窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 在使用 OpenGL 函数指针之前，加载 GLAD
    // 我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
    // GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // 在我们开始渲染之前还有一件重要的事情要做，我们必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，
    // 这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标。我们可以通过调用glViewport函数来设置窗口的维度(Dimension)：
    // glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
    // 
    // 我们实际上也可以将视口的维度设置为比GLFW的维度小，这样子之后所有的OpenGL渲染将会在一个更小的窗口中显示，
    // 这样子的话我们也可以将一些其它元素显示在OpenGL视口之外。
    glViewport(0, 0, 800, 600);

    // 然而，当用户改变窗口的大小的时候，视口也应该被调整。
    // 我们可以对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用。这个回调函数的原型如下：
    // void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    //
    // 这个帧缓冲大小函数需要一个GLFWwindow作为它的第一个参数，以及两个整数表示窗口的新维度。
    // 每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理。

    // 我们还需要注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数：
    // 当窗口被第一次显示的时候framebuffer_size_callback也会被调用。
    //
    // 我们会在创建窗口之后，渲染循环初始化之前注册这些回调函数。
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 准备你的引擎
    // 我们可不希望只绘制一个图像之后我们的应用程序就立即退出并关闭窗口。
    // 我们希望程序在我们主动关闭它之前不断绘制图像并能够接受用户输入。
    // 因此，我们需要在程序中添加一个while循环，我们可以把它称之为渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行。
    // 下面几行的代码就实现了一个简单的渲染循环：
    //
    // 1. glfwWindowShouldClose 函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
    // 2. glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
    // 3. glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 最后：
    // 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。我们可以在main函数的最后调用glfwTerminate函数来完成。
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
