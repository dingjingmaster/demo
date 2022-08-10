/*************************************************************************
> FileName: run-python.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 08 Aug 2022 04:44:07 PM CST
 ************************************************************************/
#ifndef _RUN_PYTHON_H
#define _RUN_PYTHON_H

struct PythonParam
{
    bool        failed;
    bool        success;

    string&     param;
    string&     script;

    PythonParam();
    ~PythonParam();
};

class PythonPacker
{
public:
    static PythonPacker* instance ();
    static void destoryInstance ();

    bool RunPythonString (string& script, string& param);

private:
    PythonPacker ();
    ~PythonPacker ();

private:
    static PythonPacker*    gInstance;
    //static 
    static void initPythonEnv();
};

#endif
