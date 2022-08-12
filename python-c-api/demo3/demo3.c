/*************************************************************************
> FileName: demo3.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 12 Aug 2022 10:46:10 AM CST
 ************************************************************************/

#include <stdio.h>
#include <Python.h>

int main ()
{
    // 虚拟机初始化
    Py_Initialize();

    // 导入环境变量
    PyRun_SimpleString("import sys");

    // python 脚本路径
    PyRun_SimpleString("sys.path.append('.')");

    PyObject* pModule = NULL;
    PyObject* pFunc = NULL;
    PyObject* pClass = NULL;
    PyObject* pInstance = NULL;

    // PyIport_ImportModule 函数载入 python 脚本，参数为 python 脚本名称，因为是模块化导入，
    // 代码运行时候会生成.pyc文件，直接使用 .pyc 文件效率高
    pModule = PyImport_ImportModule("nounpy");

    // 调用的函数名
    pFunc = PyObject_GetAttrString(pModule, "hello");

    // 调用函数
    PyEval_CallObject(pFunc, NULL);
    Py_DECREF(pFunc);

    // 调用python的类
    pClass = PyObject_GetAttrString(pModule, "Student");
    if (!pClass) {
        printf ("无法找到 Student 类\n");
        return -1;
    }

    pInstance = PyInstance_New (pClass, NULL, NULL);
    if (!pInstance) {
        printf ("无法实例化 Student\n");
        return -1;
    }

    // 调用其中的类方法
    PyObject_CallMethod (pInstance, "SetName", "(s)", "I am c++ args");
    PyObject_CallMethod (pInstance, "PrintName", NULL, NULL);

    // 销毁虚拟机
    Py_Finalize();

    return 0;
}
