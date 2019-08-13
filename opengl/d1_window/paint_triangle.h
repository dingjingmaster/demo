//
// Created by dingjing on 2019/8/13.
//

/**
 * 顶点数组对象：Vertex Array Object，VAO
 * 顶点缓冲对象：Vertex Buffer Object，VBO
 * 索引缓冲对象：Element Buffer Object，EBO 或 Index Buffer Object，IBO
 *
 */

#ifndef OPENGL_PAINT_TRIANGLE_H
#define OPENGL_PAINT_TRIANGLE_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

int paint_triangle () {
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

    /////////////////////////////////// 绘制 -- 开始  ////////////////////////////////////////////////
    // 三角形顶点 opengl 在 3D 空间工作，需要三维坐标
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };

    // 生成顶点缓存对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 绑定顶点缓存对象的缓存类型 GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 把之前定义的顶点数据复制到缓存的内存中
    // 该函数专门把用户定义的数据复制到当前绑定缓存的函数，第一参数是目标缓存类型，第二参数制定传输大小，第三参数表示发送的数据
    // 第四参数表示显卡如何管理给定数据：
    //      GL_STATIC_DRAW ：数据不会或几乎不会改变
    //      GL_DYNAMIC_DRAW：数据会被改变很多
    //      GL_STREAM_DRAW ：数据每次绘制时都会改变
    // 比如说一个缓冲中的数据将频繁被改变，那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW，这样就能确保显卡把数据放在能够高速写入的内存部分
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 顶点着色器
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    // 创建一个着色器对象
    unsigned  int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 把着色器源码附加到着色器对象上，并编译
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 片段着色器
    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";
    // 创建一个着色器对象
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 把两个着色器对象链接到一个用来渲染的着色器程序中
    // 着色器程序对象是多个着色器合并之后并最终链接完成的版本
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // 把编译后的着色器对象附加到程序对象上，然后用glLinkProgram 链接它们
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 链接后删除不需要的着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 激活程序对象
    glUseProgram(shaderProgram);

    // 链接顶点属性
    // 第一参数：配置顶点属性
    // 第二参数：指定顶点属性的大小
    // 第三属性：指定数据类型
    // 第四属性：是否希望被标准化
    // 第五属性：步长，告诉连续顶点属性之间的间隔
    // 第六属性：位移数据在缓存中起始位置的偏移量，以0开头
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // 拒绝绘制图像后直接退出，在主动关闭之前，不断绘制图像并接受用户输入
    // glfwPollEvents 检查是否触发了什么事件（比如：键盘输入、鼠标移动、更新窗口状态，并调用对应的回调函数）
    while (!glfwWindowShouldClose(window))
    {

        // 每次渲染前要清屏
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // 设置清屏所用的颜色
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

#endif //OPENGL_PAINT_TRIANGLE_H
