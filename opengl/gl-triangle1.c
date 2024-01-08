/*************************************************************************
> FileName: gl-triangle1.c
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // 通知GLFW将我们的窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    // 加载 GLAD
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
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // 顶点缓存对象
    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);                      // 生成顶点缓存对象 VBO

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                             // 新创建缓存绑定到 GL_ARRAY_BUFFER 目标上。从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);      // 把顶点数据复制到缓存的内存中，glBufferData 是一个专门用来把用户定义数据复制到当前绑定缓存的函数

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // 设置视窗
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制第一个三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 最后：
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
