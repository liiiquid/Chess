#ifndef POSITION_H
#define POSITION_H
#include "pregen.h"

extern const int PieceType[50];
extern const int KING_FROM     ;
extern const int ADVISOR_FROM  ;
extern const int ADVISOR_TO    ;
extern const int BISHOP_FROM   ;
extern const int BISHOP_TO     ;
extern const int KNIGHT_FROM   ;
extern const int KNIGHT_TO     ;
extern const int ROOK_FROM     ;
extern const int ROOK_TO       ;
extern const int CANNON_FROM   ;
extern const int CANNON_TO     ;
extern const int PAWN_FROM     ;
extern const int PAWN_TO       ;
extern inline int OPP_SIDE(int side);
extern inline int SIDE_TAG(int side);
extern inline int PIECE_TYPE(int pc);
// return the piece type used to recognise the side pf piece, 0-6 respresent red side.
extern inline int PIECE_TYPE_SIDE(int);
extern inline bool AWAY_HALF(int pos, int side);
extern inline int PIECE_SIDE(int pc);
extern inline bool SAME_SIDE(int pc1, int pc2);
extern inline int PIECE_INDEX(int pc);

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

}Move;

typedef struct Trace
{
    ZobristKey zobri;
    Move mv;
    Trace(const ZobristKey& k){zobri = k;}
    Trace(const ZobristKey& k, const Move& v){zobri = k; mv = v;}
}Trace;

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
    void movePiece(Move* move);
    int undoMakeMove();
    void undoMovePiece(const Move* move);
public:
    bool legalMove(const Move* mv);
    int mvLva(int dst, int captured, int lva);
    void genCapMoves(QVector<quint32>& mvs);
    void genNonCapMoves(QVector<quint32>& mvs);
    void genAllMoves(QVector<quint32>& mvs);
    void genPieceMoves(QVector<quint32>& mvs, int pos);

public:
    void saveStatus();
    void rollback();
    void changeSide();
public:
    int checkedBy();
    int protectedBy(int side, int dst);
    int chasedBy(Move* mv);

private:
    void genKingMoves(QVector<quint32> &mvs);
    void genAdvisorMoves(QVector<quint32> &mvs, int pos);
    void genBishopMoves(QVector<quint32> &mvs, int pos);
    void genKnightMoves(QVector<quint32> &mvs, int pos);
    void genRookMoves(QVector<quint32> &mvs, int pos);
    void genCannonMoves(QVector<quint32> &mvs, int pos);
    void genPawnMoves(QVector<quint32> &mvs, int pos);
private:
    ZobristKey zobri;
    quint8 board[256];
    // the distribution of piece in the ten rows.
    quint16 bitBoard_row[10];
    // the distribution of piece in the nine cols.
    quint16 bitBoard_col[9];
    quint8 pieces[50];
    int side;

    QVector<Trace> trace;

};

#endif // POSITION_H
