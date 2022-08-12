#include "python-packer.h"

#include <Python.h>

#include <mutex>
#include <iostream>

std::mutex PythonPacker::mLocker;
PythonPacker* PythonPacker::gInstance = nullptr;

PythonPacker *PythonPacker::instance()
{
    static std::mutex lock;

    if (!gInstance) {
        lock.lock ();
        if (!gInstance) {
            gInstance = new PythonPacker;
        }
        lock.unlock ();
    }

    return gInstance;
}

bool PythonPacker::RunPythonString(std::string &script, std::string& param)
{
    PythonParam threadParams (script, param);

    //
    pthread_t thread;
    int ret = pthread_create (&thread, NULL, thread_run_python_string, (void*)&threadParams);
    if (ret) {
        // 输出线程创建日志
        return false;
    }

    void* tRet;
    ret = pthread_join (thread, &tRet);

    return !ret && tRet;
}

PythonPacker::~PythonPacker()
{
    if (Py_IsInitialized ()) {
        Py_Finalize();
    }
}

PythonPacker::PythonPacker()
{
    initPythonEnv ();
}

void PythonPacker::initPythonEnv()
{
    std::string strHome = PYTHON_HOME;

    Py_SetPythonHome ((char*)strHome.c_str ());

    Py_Initialize ();
}

void* PythonPacker::thread_run_python_string(void *param)
{
    PythonParam* p = static_cast<PythonParam*>(param);

    puts("0");
    mLocker.lock ();
    if (!Py_IsInitialized ()) {
        initPythonEnv ();
    }

    puts("1");

    PyErr_Clear ();

    PyObject* pMain = PyImport_AddModule ("__main__");
    if (!pMain) {
        mLocker.unlock ();
        return (void*)false;
    }
    puts("2");

    PyObject* pGlobal = PyModule_GetDict (pMain);

    PyObject* pLocal = PyDict_New ();
    if (!pLocal) {
        mLocker.unlock ();
        return (void*)false;
    }

    PyObject* pValue = Py_BuildValue ("s#", p->mParam.c_str(), p->mParam.length());
    PyDict_SetItemString (pLocal, "p1", pValue);
    puts("3");

    PyObject* pRet = PyRun_StringFlags (p->mScript.c_str(), Py_file_input, pGlobal, pLocal, NULL);
    Py_XDECREF(pValue);
    Py_XDECREF(pLocal);
    if (!pRet) {
        mLocker.unlock ();
        return (void*)false;
    }

    puts("4");

    if (0 != PyLong_AsLong(pRet)) {
        Py_XDECREF(pRet);
        mLocker.unlock ();
        printf ("return val: %ld\n", PyLong_AsLong(pRet));
        return (void*)false;
    }
    puts("5");

    Py_XDECREF(pRet);
    mLocker.unlock ();

    return (void*)true;
}

PythonParam::PythonParam(std::string &script, std::string &param)
    : mParam(param), mScript(script)
{

}

PythonParam::~PythonParam()
{

}
