#ifndef GLOBAL_H
#define GLOBAL_H
#include "qdebug.h"
#include <QObject>

const int KING_FROM         = 0;
const int ADVISOR_FROM      = 1;
const int ADVISOR_TO        = 2;
const int BISHOP_FROM       = 3;
const int BISHOP_TO         = 4;
const int KNIGHT_FROM       = 5;
const int KNIGHT_TO         = 6;
const int ROOK_FROM         = 7;
const int ROOK_TO           = 8;
const int CANNON_FROM       = 9;
const int CANNON_TO         = 10;
const int PAWN_FROM         = 11;
const int PAWN_TO           = 15;

const int PieceType[50] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
};

const char FenPieceTable[] = {'K', 'A', 'B', 'N', 'R', 'C', 'P'};

inline const int OPP_SIDE(int side){return 1-side;}
inline const int SIDE_TAG(int side){return side * 16 + 16;}
inline const int PIECE_TYPE(int pc){ return PieceType[pc]; }
inline const int PIECE_TYPE_SIDE(int pc) {int pt = PIECE_TYPE(pc); if( pc > 31 ) pt += 7; return pt;}
inline const bool AWAY_HALF(int pos, int side) { return  ( pos & 0x10 ) == (side << 7);}
inline const int PIECE_SIDE(int pc) {return (pc & 0x10) != 0 ? 0 : 1;}
inline const bool SAME_SIDE(int pc1, int pc2){ return ((pc1 ^ pc2) & 0x10) == 0 ? true : false; }
inline const int PIECE_INDEX(int pc) {return pc & 0x0f;}
inline const QChar ToFenPiece(int pc) {return FenPieceTable[ PIECE_TYPE(pc) ];}

inline const int FenPiece(const QChar& c)
{
    switch(c.unicode())
    {
    case 'K':
    case 'k':
        return 0;
    case 'A':
    case 'a':
        return 1;
    case 'B':
    case 'b':
        return 2;
    case 'N':
    case 'n':
        return 3;
    case 'R':
    case 'r':
        return 4;
    case 'C':
    case 'c':
        return 5;
    case 'P':
    case 'p':
        return 6;
    }
    return -1;
}
typedef union
{
    quint32 t;
    struct{
        quint8 o1, oe;
        quint8 capture, chkchs;
    };
    struct{
        quint8 oi, oe1;
        qint16 mvlva;
    };

    struct{
        quint16 mv;
        quint8 cap;
    };

}Move;

const int COL_LEFT = 3;
const int ROW_TOP = 3;
// 将数组棋盘的行号和列号组合成坐标.
inline quint8 COORD_XY(int r, int c) { return r * 16 + c; }
// 将UCCI棋盘的行号转为数组棋盘的行号
inline quint8 TO_ROW(int row){return row + ROW_TOP;}
// 将UCCI棋盘的列号转为数组棋盘的列号
inline quint8 TO_COL(int col) {return col + COL_LEFT;}
// 将UCCI棋盘的行列坐标转为数组棋盘坐标
inline quint8 COORD_PC(int r, int c){return COORD_XY(TO_ROW(r), TO_COL(c));}
// 获取pos在数组棋盘的行号
inline int GET_ROW(int pos){ return (pos & 0xf0) >> 4; }
// 获取pos在数组棋盘的列号
inline int GET_COL(int pos){ return pos & 0x0f; }
// 将数组棋盘上的位置转换到UCCI棋盘对应的列号
inline int GET_REL_COL(int pos){ return (pos & 0x0f) - COL_LEFT; }
// 将数组棋盘上的位置转换到UCCI棋盘对应的行号
inline int GET_REL_ROW(int pos){ return ( (pos & 0xf0) >> 4) - ROW_TOP; }

typedef union
{
    struct{
        const QString* fen;
        const QVector<quint32>* coordList;
        int side;
    };

}UcciComm;

typedef enum{
    ucci = 0,
    isready,
    position,
    go,
    stop,
    quit,
    getpos,
    getmv,
    makemv,
    POS_ERROR,
}UcciType;

inline const quint32 ConvertToArrayCoord(const QString &mv)
{
    quint16 st = 0, ed = 0;
    st = COORD_PC( 9 - mv[1].toLower().unicode() + '0' , mv[0].toLower().unicode() - 'a');
    ed = COORD_PC( 9 - mv[3].toLower().unicode() + '0', mv[2].toLower().unicode() - 'a');
    return (ed << 8) + st;
}

inline const QString ConvertToUcciCoord(int mv)
{
    QString ret = "";
    int src = mv & 0xff;
    int dst = (mv & 0xff00) >> 8;

    ret += QChar(GET_REL_COL(src) + 'a');
    ret += QChar( 9 - GET_REL_ROW(src) + '0' );
    ret += QChar(GET_REL_COL(dst) + 'a');
    ret += QChar( 9 - GET_REL_ROW(dst) + '0');
    return ret;
}
inline const QString ConvertToUcciPoint(int cr)
{
    QString ret = "";
    int src = cr & 0xff;
    ret += QChar(GET_REL_COL(src) + 'a');
    ret += QChar( 9 - GET_REL_ROW(src) + '0' );
    return ret;
}
#endif // GLOBAL_H
