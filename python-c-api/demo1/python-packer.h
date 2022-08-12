#ifndef PYTHONPACKER_H
#define PYTHONPACKER_H

#include <mutex>

class PythonParam
{
public:
    std::string&        mParam;
    std::string&        mScript;

    PythonParam(std::string& script, std::string& param);
    ~PythonParam();
};


class PythonPacker
{
public:
    static void destoryInstance ();
    static PythonPacker* instance ();

    bool RunPythonString (std::string& script, std::string& param);

private:
    ~PythonPacker ();                                           // ok
    explicit PythonPacker ();                                   // ok

    static void initPythonEnv();                                // ok
    static void* thread_run_python_string(void* param);

private:
    static PythonPacker*    gInstance;
    static std::mutex       mLocker;
};

#endif // PYTHONPACKER_H
