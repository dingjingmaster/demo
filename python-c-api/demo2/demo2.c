/*************************************************************************
> FileName: demo2.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 12 Aug 2022 10:16:07 AM CST
 ************************************************************************/
#include <stdio.h>
#include <Python.h>

int main(int argc, char* argv[])
{
    printf("Calling Python to find the sum of 2 and 2.\n");

    // Initialize the Python interpreter.
    Py_Initialize();

    printf("init ok\n");

    // Create some Python objects that will later be assigned values.
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue;

    // Convert the file name to a Python string.
    pName = PyUnicode_FromString("Sample");
    if (!pName) {
        printf ("pName null\n");
        return -1;
    }

    // Import the file as a Python module.
    pModule = PyImport_Import(pName);
    if (!pModule) {
        printf ("pModule null\n");
        return -1;
    }

    // Create a dictionary for the contents of the module.
    pDict = PyModule_GetDict(pModule);
    printf ("pDict ok\n");

    // Get the add method from the dictionary.
    pFunc = PyDict_GetItemString(pDict, "add");
    printf ("pFunc ok\n");

    // Create a Python tuple to hold the arguments to the method.
    pArgs = PyTuple_New(2);
    printf ("pArgs ok\n");

    // Convert 2 to a Python integer.
    pValue = PyLong_FromLong(2);
    printf ("pValue ok\n");

    // Set the Python int as the first and second arguments to the method.
    PyTuple_SetItem(pArgs, 0, pValue);
    PyTuple_SetItem(pArgs, 1, pValue);
    printf ("PyTuple_SetItem ok\n");

    // Call the function with the arguments.
    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    printf ("PyObject_CallObject ok\n");

    // Print a message if calling the method failed.
    if (pResult == NULL)
        printf("Calling the add method failed.\n");

    // Convert the result to a long from a Python object.
    long result = PyLong_AsLong(pResult);
    printf ("get result ok\n");

    // Destroy the Python interpreter.
    Py_Finalize();
    printf ("Py_Finalize ok\n");

    // Print the result.
    printf("The result is %d.\n", result);

    return 0; 
}

