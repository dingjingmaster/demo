#ifndef PYTHONPACKER_H
#define PYTHONPACKER_H

#include <QObject>
#include <qglobal.h>

struct PythonParam
{
    bool                failed;
    bool                success;

    std::string&        param;
    std::string&        script;

    PythonParam();
    ~PythonParam();
};


class PythonPacker : public QObject
{
    Q_OBJECT
public:
    static PythonPacker* instance ();
    static void destoryInstance ();

    bool RunPythonString (std::string& script, std::string& param);

private:
    explicit PythonPacker (QObject* parent = nullptr);
    ~PythonPacker ();

private:
    static PythonPacker*    gInstance;
    //static
    static void initPythonEnv();
};

#endif // PYTHONPACKER_H
