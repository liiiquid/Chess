#ifndef PREGEN_H
#define PREGEN_H
#include "hash/zobristkey.h"

const int COL_LEFT = 3;
const int ROW_TOP = 3;

extern const quint8 BoardMask[256];
extern const quint8 FortMask[256];
extern const quint8 LEGAL_SPAN_TAB[512];
extern const qint8 KNIGHT_PIN_TAB[512];
inline bool IN_BOARD(int pos) { return BoardMask[pos] == 1; }
inline bool IN_FORT(int pos) { return FortMask[pos] == 1; }
inline int SQUARE_FORWARD(int pos, int side) { return pos - 0x10 + (side << 5); }
inline int SQUARE_BACKWARD(int pos, int side) { return pos + 0x10 - (side << 5); }

inline bool SAME_HALF(int pos1, int pos2) { return ((pos1^pos2) & 0x80) == 0; }
inline bool KING_SPAN(int src, int dst) { return LEGAL_SPAN_TAB[ src - dst + 256 ] == 1; }
inline bool ADVISOR_SPAN(int src, int dst) { return LEGAL_SPAN_TAB[ src - dst + 256 ] == 2; }
inline bool BISHOP_SPAN(int src, int dst) { return LEGAL_SPAN_TAB[ src - dst + 256 ] == 3; }
inline int  BISHOP_PIN(int src, int dst) {return (src + dst) >> 1;}
inline int  KNIGHT_PIN(int src, int dst) { return src + KNIGHT_PIN_TAB[dst - src + 256]; }

// 将数组棋盘的行号和列号组合成坐标.
inline quint8 COORD_XY(int r, int c) { return r * 16 + c; }
// 将UCCI棋盘的行号转为数组棋盘的行号
inline quint8 TO_ROW(int row){return row + ROW_TOP;}
// 将UCCI棋盘的列号转为数组棋盘的列号
inline quint8 TO_COL(int col) {return col + COL_LEFT;}
// 将UCCI棋盘的行列坐标转为数组棋盘坐标
inline quint8 COORD_PC(int r, int c){return COORD_XY(TO_ROW(r), TO_COL(c));}
// 获取pos在数组棋盘的行号
inline int GET_ROW(int pos){ return pos & 0xf0; }
// 获取pos在数组棋盘的列号
inline int GET_COL(int pos){ return pos & 0x0f; }
// 将数组棋盘上的位置转换到UCCI棋盘对应的列号
inline int GET_REL_COL(int pos){ return pos & 0x0f - COL_LEFT; }
// 将数组棋盘上的位置转换到UCCI棋盘对应的行号
inline int GET_REL_ROW(int pos){ return pos & 0xf0 - ROW_TOP; }

typedef struct{
    int nonCap[2];
    int rookCap[2];
    int cannonCap[2];
}RookCannon_Move;
typedef struct{
    quint16 nonCap;
    quint16 rookCap;
    quint16 cannonCap;
}RookCannon_MoveMask;

typedef struct{
    int tar[12];
    int pin[12];
}Knight_Move;

typedef struct{
    int tar[8];
    int pin[8];
}Bishop_Move;

class PreGen
{

public:
    explicit PreGen();

public:
    void init();


public:
    ZobristKey zobriTable[14][256], zobriPlayer;

    RookCannon_Move rookCannon_MoveTable_row[9][512];
    RookCannon_Move rookCannon_MoveTable_col[10][1024];

    RookCannon_MoveMask rookCannon_MoveMaskTable_row[9][512];
    RookCannon_MoveMask rookCannon_MoveMaskTable_col[10][1024];

    Knight_Move     knight_MoveTable[256];
    Bishop_Move     bishop_MoveTable[256];
    int             advisor_MoveTable[256][8];
    int             king_MoveTable[256][8];
    // 0 表示底部阵容的向上着法表, 1 表示上面阵容的向上着法表.
    int             pawn_MoveTable[256][2][8];

    quint16 PieceMask_row[256];
    quint16 PieceMask_col[256];

};

extern PreGen PreGen;




#endif // PREGEN_H
