/*************************************************************************
> FileName: demo4.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 12 Aug 2022 04:13:18 PM CST
 ************************************************************************/
#include <stdio.h>
#include <Python.h>
#include <string.h>

#define error(s) puts(s)

int main()
{
    /* run strings with low-level calls */
    char *cstr;
    PyObject *pstr, *pmod, *pdict;               /* with error tests */
    Py_Initialize();

    /* result = string.upper('spam') + '!' */
    pmod = PyImport_ImportModule("string");      /* fetch module */
    if (pmod == NULL)                            /* for name-space */
        error("Can't import module");

    pdict = PyModule_GetDict(pmod);              /* string.__dict__ */
    Py_DECREF(pmod);
    if (pdict == NULL)
        error("Can't get module dict");

    pstr = PyRun_String("upper('spam') + '!'", Py_eval_input, pdict, pdict);
    //pstr = PyRun_String("exit(1)", Py_eval_input, pdict, pdict);
    if (pstr == NULL)
        error("Error while running string");

    /* convert result to C */
    if (!PyArg_Parse(pstr, "s", &cstr))
        error("Bad result type");
    printf("%s\n", cstr);
    Py_DECREF(pstr);        /* free exported objects, not pdict */
}
