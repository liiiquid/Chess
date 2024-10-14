#ifndef POSITION_H
#define POSITION_H
#include "pregen.h"
#include "qdebug.h"

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
inline const bool AWAY_HALF(int pos, int side) { return  ( pos & 0x80 ) == (side << 7);}
inline const int PIECE_SIDE(int pc) {return (pc & 0x10) == 0;}
inline const bool SAME_SIDE(int pc1, int pc2){ return ((pc1 ^ pc2) & 0x10) == 0; }
inline const int DST(int mv) {return ( mv & 0xff00) >> 8;}
inline const int SRC(int mv) {return (mv & 0x00ff);}
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
typedef union Move
{
    quint32 t;
    struct{
        quint8 o1, oe;
        qint8 capture;//大于0表示被吃子,小于0表示和棋着法数.
        qint8 chkchs; // 大于0表示着法的将军子, 小于0表示被捉子.
    };
    struct{
        quint8 oi, oe1;
        qint16 mvlva;
    };

    struct{
        quint16 mv;
        quint16 val;
    };

    Move(){
        t = 0;
    }

}Move;

typedef struct Trace
{
    ZobristKey zobri;
    Move mv;
    int vlRed, vlBlack;
    Trace(){}
    Trace(const Trace& t){Trace(t.zobri, t.vlRed, t.vlBlack); mv = t.mv;}
    Trace(const ZobristKey&k, int vr, int vb){zobri = k; vlRed = vr; vlBlack = vb; }
}Trace;

const quint16 BIT_KING = (1 << KING_FROM);
const quint16 BIT_ADVISOR = (1 << ADVISOR_FROM) | (1 << ADVISOR_TO);
const quint16 BIT_BISHOP = (1 << BISHOP_FROM) | (1 << BISHOP_TO);
const quint16 BIT_KNIGHT = (1 << KNIGHT_FROM) | (1 << KNIGHT_TO);
const quint16 BIT_CANNON = (1 << CANNON_FROM) | (1 << CANNON_TO);
const quint16 BIT_ROOK = (1 << ROOK_FROM) | (1 << ROOK_TO);
const quint16 BIT_PAWN = (1 << PAWN_FROM) | (1 << (PAWN_FROM + 1)) | (1 << (PAWN_FROM + 2)) | (1 << (PAWN_FROM + 3)) | (1 << PAWN_TO);

typedef struct
{
    int advance;
    int valueRed[7][256];
    int valueBlack[7][256];
    int valueRedHollowThreat[16];
    int valueBlackHollowThreat[16];
    int valueRedCentralThreat[16];
    int valueBlackCentralThreat[16];
    int valRedBottomThreat[16];
    int valBlackBottomThreat[16];
    int valRedAdvisorLeakage;
    int valBlackAdvisorLeakage;
}PreEval;

inline const int SIDE_VALUE(int side, int val ) { return side == 0 ? val : -val; }

const int REP_NONE = 0;
const int REP_DRAW = 1;
const int REP_WIN  = 2;
const int REP_LOSE = 3;

const int REP_LEN = 4096;
const int REP_VALUE = 9000;
class Position
{
public:
    Position();
    void init();

public:
    int FromFen(const QString& fen);
    QString toFen();
public:
    const RookCannon_MoveMask* HorizonMaskPtr(int pos);
    const RookCannon_MoveMask* VerticMaskPtr(int pos);
public:
    const RookCannon_Move* HorizonMovePtr(int pos);
    const RookCannon_Move* VerticMovePtr(int pos);
public:
    int addPiece(int pos, int pc);
    int delPiece(int pos);
    int makeMove(quint32 mv);
    int movePiece(Move* move);
    int undoMakeMove();
    void undoMovePiece(const Move* move);

    void nullMove();
    void undoNullMove();
public:
    Move lastMove(){
        if(steps > 0 )
            return trace[steps-1].mv;
        return Move();
    }
public:
    bool legalMove(int mv);
    int mvLva(int dst, int captured, int lva);
    int genCapMoves(Move* mvs);
    int genNonCapMoves(Move* mvs);
    int genAllMoves(Move* mvs);
    void genPieceMoves(QVector<Move>& mvs, int pos);

public:
    void saveStatus();
    void rollback();
    void changeSide();
public:
    int checkedBy();
    int protectedBy(int side, int dst);
    int chasedBy(int mv);
    int repStatus(int repOccur = 1);
    bool bMate();

private:
    void genKingMoves(QVector<Move>& mvs);
    void genAdvisorMoves(QVector<Move>& mvs, int pos);
    void genBishopMoves(QVector<Move>& mvs, int pos);
    void genKnightMoves(QVector<Move>& mvs, int pos);
    void genRookMoves(QVector<Move>& mvs, int pos);
    void genCannonMoves(QVector<Move>& mvs, int pos);
    void genPawnMoves(QVector<Move>& mvs, int pos);

public:
    void preEvaluate();
    // 预估子力评价.
    int material() {return SIDE_VALUE(side, vlRed - vlBlack) + preEval.advance;}
    int evaluate(int alpha, int beta);
    int advisorShape();
    int knightTrap();
    int stringHold();
    int rookMobility();

    bool nullOkay() {return ( side == 0 ? vlRed : vlBlack) > 500;}
    bool nullSafe() {return (side == 0 ? vlRed: vlBlack) > 500;}
    int drawValue(int vlRep) { return vlRep == REP_WIN ? REP_VALUE : vlRep == REP_DRAW ? -REP_VALUE : 0;  }

public:
    int vlRed;
    int vlBlack;

public:
    ZobristKey zobri;
    quint8 board[256];
    // 棋子在每行的分布情况
    quint16 bitBoard_row[10];
    // 棋子在每列的分布情况
    quint16 bitBoard_col[9];
    quint8 pieces[50];
    quint16 bitPiece[2];
    int side;
public:
    int repHash[REP_LEN];
    int repLenMask;
public:
    Trace trace[1024];
    int steps;
    int distance;
public:
    PreEval preEval;

public:
    void printBoard();
    void printPieces();
    void printBitRow(int r) {qDebug() << QString("%1 ").arg(bitBoard_row[r], 16, 2, QLatin1Char('0'));}
    void printBitCol(int c){qDebug() << QString("%1 ").arg(bitBoard_col[c], 16, 2, QLatin1Char('0'));}
    void printBitPiece(){qDebug() << QString("red: %1 ").arg(bitPiece[0], 16, 2, QLatin1Char('0')); qDebug() << QString("black: %1 ").arg(bitPiece[1], 16, 2, QLatin1Char('0'));}

public:
    int makeCnt;
    int unmakeCnt;

};

#endif // POSITION_H
