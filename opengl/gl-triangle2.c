/*************************************************************************
> FileName: gl-triangle2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 05 Jan 2024 02:35:31 PM CST
 ************************************************************************/
#include "glad.h"

#include <stdio.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>


void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main ()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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


    ///////////////////////////////
    // 顶点着色器
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);                   // 创建着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);                     // 将着色器源码附加到着色器对象
    glCompileShader(vertexShader);                                                  // 编译着色器源码

    int success;
    char infoBuf[512] = {0};
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);                       // 检测着色器源码是否编译成功？(非必需操作)
    if (!success) {
        glGetShaderInfoLog(vertexShader, sizeof(infoBuf) - 1, NULL, infoBuf);
        printf ("%s\n", infoBuf);
        return -1;
    }

    // 片段着色器
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // 将之前编译的着色器程序附加到 着色器 程序对象上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);                                                   // 链接

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoBuf) - 1, NULL, infoBuf);
        printf ("%s\n", infoBuf);
        return -1;
    }

    // 着色器对象链接到 着色器程序后 就可以删除
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // 三角形顶点
    float vertices[] = {
        0.5f,  0.5f, 0.0f,      // top right
        0.5f, -0.5f, 0.0f,      // bottom right
        -0.5f, -0.5f, 0.0f,     // bottom left
        -0.5f,  0.5f, 0.0f      // top left
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,                // first Triangle
        1, 2, 3                 // second Triangle
    };

    // 顶点缓存对象
    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);                      // 生成顶点缓存对象 VBO

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                             // 新创建缓存绑定到 GL_ARRAY_BUFFER 目标上。从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      // 把顶点数据复制到缓存的内存中，glBufferData 是一个专门用来把用户定义数据复制到当前绑定缓存的函数

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    ///////////////////////////////
    // 在我们开始渲染之前还有一件重要的事情要做，我们必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，
    // 这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标。我们可以通过调用glViewport函数来设置窗口的维度(Dimension)：
    // glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
    // 
    // 我们实际上也可以将视口的维度设置为比GLFW的维度小，这样子之后所有的OpenGL渲染将会在一个更小的窗口中显示，
    // 这样子的话我们也可以将一些其它元素显示在OpenGL视口之外。
    //glViewport(0, 0, 800, 600); // 有回调，可省

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
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制第一个三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // 绘制第二个
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 最后：
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。我们可以在main函数的最后调用glfwTerminate函数来完成。
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
