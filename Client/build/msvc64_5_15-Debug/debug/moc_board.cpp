/****************************************************************************
** Meta object code from reading C++ file 'board.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../board/board.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'board.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Board_t {
    QByteArrayData data[34];
    char stringdata0[312];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Board_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Board_t qt_meta_stringdata_Board = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Board"
QT_MOC_LITERAL(1, 6, 15), // "messageReceived"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 3), // "rec"
QT_MOC_LITERAL(4, 27, 15), // "netStateChanged"
QT_MOC_LITERAL(5, 43, 3), // "str"
QT_MOC_LITERAL(6, 47, 11), // "invalidMove"
QT_MOC_LITERAL(7, 59, 16), // "selectPieceError"
QT_MOC_LITERAL(8, 76, 13), // "undomakeError"
QT_MOC_LITERAL(9, 90, 7), // "gamewin"
QT_MOC_LITERAL(10, 98, 6), // "winner"
QT_MOC_LITERAL(11, 105, 11), // "goRecovered"
QT_MOC_LITERAL(12, 117, 14), // "onPieceClicked"
QT_MOC_LITERAL(13, 132, 15), // "onReceivePacket"
QT_MOC_LITERAL(14, 148, 8), // "instruct"
QT_MOC_LITERAL(15, 157, 11), // "onConnected"
QT_MOC_LITERAL(16, 169, 14), // "onDisconnected"
QT_MOC_LITERAL(17, 184, 14), // "onReconnecting"
QT_MOC_LITERAL(18, 199, 12), // "undoMakeMove"
QT_MOC_LITERAL(19, 212, 9), // "mirror_k1"
QT_MOC_LITERAL(20, 222, 5), // "print"
QT_MOC_LITERAL(21, 228, 6), // "getpos"
QT_MOC_LITERAL(22, 235, 6), // "setpos"
QT_MOC_LITERAL(23, 242, 5), // "getmv"
QT_MOC_LITERAL(24, 248, 3), // "pos"
QT_MOC_LITERAL(25, 252, 4), // "ucci"
QT_MOC_LITERAL(26, 257, 7), // "isready"
QT_MOC_LITERAL(27, 265, 8), // "startpos"
QT_MOC_LITERAL(28, 274, 8), // "position"
QT_MOC_LITERAL(29, 283, 11), // "selectPiece"
QT_MOC_LITERAL(30, 295, 2), // "pc"
QT_MOC_LITERAL(31, 298, 2), // "go"
QT_MOC_LITERAL(32, 301, 7), // "protect"
QT_MOC_LITERAL(33, 309, 2) // "cr"

    },
    "Board\0messageReceived\0\0rec\0netStateChanged\0"
    "str\0invalidMove\0selectPieceError\0"
    "undomakeError\0gamewin\0winner\0goRecovered\0"
    "onPieceClicked\0onReceivePacket\0instruct\0"
    "onConnected\0onDisconnected\0onReconnecting\0"
    "undoMakeMove\0mirror_k1\0print\0getpos\0"
    "setpos\0getmv\0pos\0ucci\0isready\0startpos\0"
    "position\0selectPiece\0pc\0go\0protect\0"
    "cr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Board[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  139,    2, 0x06 /* Public */,
       4,    1,  142,    2, 0x06 /* Public */,
       6,    0,  145,    2, 0x06 /* Public */,
       7,    0,  146,    2, 0x06 /* Public */,
       8,    0,  147,    2, 0x06 /* Public */,
       9,    1,  148,    2, 0x06 /* Public */,
      11,    0,  151,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  152,    2, 0x0a /* Public */,
      13,    1,  153,    2, 0x08 /* Private */,
      15,    0,  156,    2, 0x08 /* Private */,
      16,    0,  157,    2, 0x08 /* Private */,
      17,    0,  158,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      18,    0,  159,    2, 0x02 /* Public */,
      19,    0,  160,    2, 0x02 /* Public */,
      20,    0,  161,    2, 0x02 /* Public */,
      21,    0,  162,    2, 0x02 /* Public */,
      22,    0,  163,    2, 0x02 /* Public */,
      23,    1,  164,    2, 0x02 /* Public */,
      25,    0,  167,    2, 0x02 /* Public */,
      26,    0,  168,    2, 0x02 /* Public */,
      27,    0,  169,    2, 0x02 /* Public */,
      28,    1,  170,    2, 0x02 /* Public */,
      29,    1,  173,    2, 0x02 /* Public */,
      31,    1,  176,    2, 0x02 /* Public */,
      32,    1,  179,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void, QMetaType::Int,   30,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   33,

       0        // eod
};

void Board::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Board *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->netStateChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->invalidMove(); break;
        case 3: _t->selectPieceError(); break;
        case 4: _t->undomakeError(); break;
        case 5: _t->gamewin((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->goRecovered(); break;
        case 7: _t->onPieceClicked(); break;
        case 8: _t->onReceivePacket((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->onConnected(); break;
        case 10: _t->onDisconnected(); break;
        case 11: _t->onReconnecting(); break;
        case 12: _t->undoMakeMove(); break;
        case 13: _t->mirror_k1(); break;
        case 14: _t->print(); break;
        case 15: _t->getpos(); break;
        case 16: _t->setpos(); break;
        case 17: _t->getmv((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->ucci(); break;
        case 19: _t->isready(); break;
        case 20: _t->startpos(); break;
        case 21: _t->position((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: _t->selectPiece((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->go((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 24: _t->protect((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Board::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Board::messageReceived)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Board::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Board::netStateChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Board::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Board::invalidMove)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Board::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Board::selectPieceError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Board::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Board::undomakeError)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Board::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Board::gamewin)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Board::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Board::goRecovered)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Board::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickPaintedItem::staticMetaObject>(),
    qt_meta_stringdata_Board.data,
    qt_meta_data_Board,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Board::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Board::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Board.stringdata0))
        return static_cast<void*>(this);
    return QQuickPaintedItem::qt_metacast(_clname);
}

int Board::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickPaintedItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void Board::messageReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Board::netStateChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Board::invalidMove()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Board::selectPieceError()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Board::undomakeError()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Board::gamewin(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Board::goRecovered()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
