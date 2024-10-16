/****************************************************************************
** Meta object code from reading C++ file 'serverentry.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../entry/serverentry.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverentry.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ServerEntry_t {
    QByteArrayData data[21];
    char stringdata0[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ServerEntry_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ServerEntry_t qt_meta_stringdata_ServerEntry = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ServerEntry"
QT_MOC_LITERAL(1, 12, 12), // "startChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "_start"
QT_MOC_LITERAL(4, 33, 15), // "messageReceived"
QT_MOC_LITERAL(5, 49, 3), // "str"
QT_MOC_LITERAL(6, 53, 9), // "handleCmd"
QT_MOC_LITERAL(7, 63, 3), // "cmd"
QT_MOC_LITERAL(8, 67, 13), // "onDisonnected"
QT_MOC_LITERAL(9, 81, 4), // "init"
QT_MOC_LITERAL(10, 86, 5), // "close"
QT_MOC_LITERAL(11, 92, 5), // "print"
QT_MOC_LITERAL(12, 98, 16), // "printBitBoardRow"
QT_MOC_LITERAL(13, 115, 16), // "printBitBoardCol"
QT_MOC_LITERAL(14, 132, 13), // "printBitPiece"
QT_MOC_LITERAL(15, 146, 13), // "printMaterial"
QT_MOC_LITERAL(16, 160, 7), // "printAp"
QT_MOC_LITERAL(17, 168, 7), // "printSh"
QT_MOC_LITERAL(18, 176, 7), // "printKT"
QT_MOC_LITERAL(19, 184, 7), // "printRm"
QT_MOC_LITERAL(20, 192, 5) // "start"

    },
    "ServerEntry\0startChanged\0\0_start\0"
    "messageReceived\0str\0handleCmd\0cmd\0"
    "onDisonnected\0init\0close\0print\0"
    "printBitBoardRow\0printBitBoardCol\0"
    "printBitPiece\0printMaterial\0printAp\0"
    "printSh\0printKT\0printRm\0start"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ServerEntry[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       1,  110, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   95,    2, 0x0a /* Public */,
       8,    0,   98,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       9,    0,   99,    2, 0x02 /* Public */,
      10,    0,  100,    2, 0x02 /* Public */,
      11,    0,  101,    2, 0x02 /* Public */,
      12,    0,  102,    2, 0x02 /* Public */,
      13,    0,  103,    2, 0x02 /* Public */,
      14,    0,  104,    2, 0x02 /* Public */,
      15,    0,  105,    2, 0x02 /* Public */,
      16,    0,  106,    2, 0x02 /* Public */,
      17,    0,  107,    2, 0x02 /* Public */,
      18,    0,  108,    2, 0x02 /* Public */,
      19,    0,  109,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      20, QMetaType::Bool, 0x00495103,

 // properties: notify_signal_id
       0,

       0        // eod
};

void ServerEntry::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ServerEntry *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->startChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->handleCmd((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onDisonnected(); break;
        case 4: _t->init(); break;
        case 5: _t->close(); break;
        case 6: _t->print(); break;
        case 7: _t->printBitBoardRow(); break;
        case 8: _t->printBitBoardCol(); break;
        case 9: _t->printBitPiece(); break;
        case 10: _t->printMaterial(); break;
        case 11: _t->printAp(); break;
        case 12: _t->printSh(); break;
        case 13: _t->printKT(); break;
        case 14: _t->printRm(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ServerEntry::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerEntry::startChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ServerEntry::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerEntry::messageReceived)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ServerEntry *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->getStart(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ServerEntry *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setStart(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject ServerEntry::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ServerEntry.data,
    qt_meta_data_ServerEntry,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ServerEntry::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServerEntry::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ServerEntry.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ServerEntry::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ServerEntry::startChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ServerEntry::messageReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
