/****************************************************************************
** Meta object code from reading C++ file 'main-header.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.10)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "main-header.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main-header.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.10. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainHeader_t {
    QByteArrayData data[9];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainHeader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainHeader_t qt_meta_stringdata_MainHeader = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainHeader"
QT_MOC_LITERAL(1, 11, 9), // "windowMin"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 9), // "windowMax"
QT_MOC_LITERAL(4, 32, 11), // "windowClose"
QT_MOC_LITERAL(5, 44, 20), // "setCloseButtonEnable"
QT_MOC_LITERAL(6, 65, 1), // "e"
QT_MOC_LITERAL(7, 67, 11), // "onApplyData"
QT_MOC_LITERAL(8, 79, 19) // "onApplyDataFinished"

    },
    "MainHeader\0windowMin\0\0windowMax\0"
    "windowClose\0setCloseButtonEnable\0e\0"
    "onApplyData\0onApplyDataFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainHeader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,
       5,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   50,    2, 0x0a /* Public */,
       8,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainHeader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainHeader *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->windowMin(); break;
        case 1: _t->windowMax(); break;
        case 2: _t->windowClose(); break;
        case 3: _t->setCloseButtonEnable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onApplyData(); break;
        case 5: _t->onApplyDataFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainHeader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainHeader::windowMin)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainHeader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainHeader::windowMax)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainHeader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainHeader::windowClose)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainHeader::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainHeader::setCloseButtonEnable)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainHeader::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MainHeader.data,
    qt_meta_data_MainHeader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainHeader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainHeader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainHeader.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainHeader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MainHeader::windowMin()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainHeader::windowMax()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainHeader::windowClose()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MainHeader::setCloseButtonEnable(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
