#include "position.h"
#include "./entry/ucci.h"
#include "qdebug.h"
const int SIMPLE_VALUE[14] = {
  5,1,2,3,4,3,1,
  5,1,2,3,4,3,1
};

// 被将着法不收集
// 若着法分值为负,不吃子
// 着法分值为大于等于0, 在原值基础加1.
int Position::mvLva(int mv, int captured, int lva)
{
    int dst = DST(mv);
    int oppval = captured != 0 ? SIMPLE_VALUE[PIECE_TYPE(captured)] : 0;
    int adjust = 0;
    int ret = makeMove(mv);
    if( ret == -1 )
    {
        return -5;
    }else if(ret == 0)
    {
        if( protectedBy(side, dst) > 0)
        {
            adjust = lva;
        }
        undoMakeMove();
        oppval = oppval - adjust;
        if( oppval < 0)
        {
            return 0;
        }else
        {
            return oppval + 1;
        }
    }

    return 0;
}

int Position::genCapMoves(Move* mvs)
{
    int pc, oppPc;
    int sideTag = SIDE_TAG(this->side);
    Move mv;
    int len;
    int src, dst;
    int size = 0;

    pc = sideTag + KING_FROM;
    src = pieces[pc];
    len = 0;
    while( ( dst = PreGen.king_MoveTable[src][len] ) != 0 )
    {
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mv.mvlva = mvLva(mv.mv, oppPc, 5);
            if(mv.mvlva != -5)
            {
                mvs[size++] = mv;
            }
#ifdef GENMOVE_DEBUG
            qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
        }
        len++;
    }


    for(pc = sideTag + ADVISOR_FROM; pc <= sideTag + ADVISOR_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        len = 0;
        while( ( dst = PreGen.advisor_MoveTable[src][len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(mv.mv, oppPc, 1);
                if(mv.mvlva != -5)
                {
                    mvs[size++] = mv;
                }

#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
            len++;
        }
    }

    for(pc = sideTag + BISHOP_FROM; pc <= sideTag + BISHOP_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        len = 0;
        const Bishop_Move& b = PreGen.bishop_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false && board[b.pin[len]] == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(mv.mv, oppPc, 2);
                if(mv.mvlva != -5)
                {
                    mvs[size++] = mv;
                }
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
            len++;
        }
    }

    for(pc = sideTag + KNIGHT_FROM; pc <= sideTag + KNIGHT_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        len = 0;
        const Knight_Move& b = PreGen.knight_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false && board[b.pin[len]] == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(mv.mv, oppPc, 3);
                if(mv.mvlva != -5)
                {
                    mvs[size++] = mv;
                }
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
            len++;
        }
    }

    for(pc = sideTag + CANNON_FROM; pc <= sideTag + CANNON_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);

        for(len = 0; len <= 1; len++)
        {
            if(horinz.cannonCap[len] == GET_COL(src)) continue;
            dst = COORD_XY ( GET_ROW(src), horinz.cannonCap[len]);
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(mv.mv, oppPc, 3);
                if(mv.mvlva != -5)
                {
                    mvs[size++] = mv;
                }
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

        for(len = 0; len <= 1; len++)
        {
            if(vertic.cannonCap[len] == GET_ROW(src)) continue;
            dst = COORD_XY ( vertic.cannonCap[len], GET_COL(src));
            oppPc = board[dst];

            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(mv.mv, oppPc, 3);
                if(mv.mvlva != -5)
                {
                    mvs[size++] = mv;
                }
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

    }

    for(pc = sideTag + ROOK_FROM; pc <= sideTag + ROOK_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);

        for(len = 0; len <= 1; len++)
        {
            if(horinz.rookCap[len] == GET_COL(src)) continue;
            dst = COORD_XY ( GET_ROW(src), horinz.rookCap[len]);
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(mv.mv, oppPc, 4);
                if(mv.mvlva != -5)
                {
                    mvs[size++] = mv;
                }
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

        for(len = 0; len <= 1; len++)
        {
            if(vertic.rookCap[len] == GET_ROW(src)) continue;
            dst = COORD_XY ( vertic.rookCap[len], GET_COL(src));
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(mv.mv, oppPc, 4);
                if(mv.mvlva != -5)
                {
                    mvs[size++] = mv;
                }
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }
    }

    for(pc = sideTag + PAWN_FROM; pc <= sideTag + PAWN_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        len = 0;
        while( ( dst = PreGen.pawn_MoveTable[src][side][len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(mv.mv, oppPc, 1);
                if(mv.mvlva != -5)
                {
                    mvs[size++] = mv;
                }
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
            len++;
        }
    }
    return size;

}

int Position::genNonCapMoves(Move* mvs)
{
    int pc, oppPc;
    int sideTag = SIDE_TAG(this->side);
    Move mv;
    int len;
    int src, dst;
    int size = 0;
    pc = sideTag + KING_FROM;
    src = pieces[pc];
    len = 0;
    while( ( dst = PreGen.king_MoveTable[src][len] ) != 0 )
    {
        oppPc = board[dst];
        if( oppPc == 0)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
            qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
        }
        len++;
    }

    for(pc = sideTag + ADVISOR_FROM; pc <= sideTag + ADVISOR_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        len = 0;
        while( ( dst = PreGen.advisor_MoveTable[src][len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
            len++;
        }
    }

    for(pc = sideTag + BISHOP_FROM; pc <= sideTag + BISHOP_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        len = 0;
        const Bishop_Move& b = PreGen.bishop_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc == 0 && board[b.pin[len]] == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
            len++;
        }
    }

    for(pc = sideTag + KNIGHT_FROM; pc <= sideTag + KNIGHT_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        len = 0;
        const Knight_Move& b = PreGen.knight_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc == 0 && board[b.pin[len]] == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
            len++;
        }
    }

    for(pc = sideTag + ROOK_FROM; pc <= sideTag + ROOK_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);

        int leftEnd = COORD_XY(GET_ROW(src), ( horinz.nonCap[0]) - 1  );

        for(dst = src - 1; dst > leftEnd; dst--)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

        int rightEnd = COORD_XY(GET_ROW(src), ( horinz.nonCap[1] ) + 1  );

        for(dst = src + 1; dst < rightEnd; dst++)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

        int upperEnd = COORD_XY( (vertic.nonCap[0]) - 1, GET_COL(src));
        for(dst = src - 0x10; dst > upperEnd; dst -= 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

        int downEnd = COORD_XY( ( vertic.nonCap[1]) + 1, GET_COL(src));
        for(dst = src + 0x10; dst < downEnd; dst += 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }
    }

    for(pc = sideTag + CANNON_FROM; pc <= sideTag + CANNON_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);

        int leftEnd = COORD_XY(GET_ROW(src), ( horinz.nonCap[0]) - 1  );

        for(dst = src - 1; dst > leftEnd; dst--)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

        int rightEnd = COORD_XY(GET_ROW(src), ( horinz.nonCap[1] ) + 1  );

        for(dst = src + 1; dst < rightEnd; dst++)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

        int upperEnd = COORD_XY( (vertic.nonCap[0]) - 1, GET_COL(src));
        for(dst = src - 0x10; dst > upperEnd; dst -= 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }

        int downEnd = COORD_XY( ( vertic.nonCap[1]) + 1, GET_COL(src));
        for(dst = src + 0x10; dst < downEnd; dst += 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
        }
    }



    for(pc = sideTag + PAWN_FROM; pc <= sideTag + PAWN_TO; pc ++)
    {
        src = pieces[pc];
        if(src == 0) continue;
        len = 0;
        while( ( dst = PreGen.pawn_MoveTable[src][side][len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs[size++] = mv;
#ifdef GENMOVE_DEBUG
                qDebug() << pc << ConvertToUcciCoord(mvs[size-1].mv) << QString::number(mvs[size-1].val);
#endif
            }
            len++;
        }
    }

    return size;
}

void Position::genKingMoves(QVector<Move>& mvs)
{
    int pc, oppPc;
    int sideTag = SIDE_TAG(this->side);
    Move mv;
    int len;
    int src, dst;
    // 将吃子着法
    pc = sideTag + KING_FROM;
    src = pieces[pc];
    len = 0;
    while( ( dst = PreGen.king_MoveTable[src][len] ) != 0 )
    {
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }else if(oppPc == 0)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
        len++;
    }
}

void Position::genAdvisorMoves(QVector<Move>& mvs, int pos)
{
    int pc, oppPc;
    Move mv;
    mv.t = 0;
    int len;
    int src, dst;

    src = pos;
    pc = board[src];

    len = 0;
    while( ( dst = PreGen.advisor_MoveTable[src][len] ) != 0 )
    {
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }else if(oppPc == 0)
        {
            mv.oi = src;
            mv.oe = dst;
            mvs.push_back(mv);
        }
        len++;
    }
}

void Position::genBishopMoves(QVector<Move>& mvs,int pos)
{
    int pc, oppPc;
    Move mv;
    int len;
    int src, dst;

    src = pos;
    pc = board[src];
    len = 0;
    const Bishop_Move& b = PreGen.bishop_MoveTable[src];

    while( ( dst = b.tar[len] ) != 0 )
    {
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false && board[b.pin[len]] == 0)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }else if(oppPc == 0 && board[b.pin[len]] == 0)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }

        len++;
    }
}

void Position::genKnightMoves(QVector<Move>& mvs, int pos)
{
    int pc, oppPc;
    Move mv;
    int len;
    int src, dst;
    src = pos;
    pc = board[src];
    len = 0;
    const Knight_Move& b = PreGen.knight_MoveTable[src];
    while( ( dst = b.tar[len] ) != 0 )
    {
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false && board[b.pin[len]] == 0)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }else if(oppPc == 0 && board[b.pin[len]] == 0)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }

        len++;
    }

}

void Position::genRookMoves(QVector<Move>& mvs, int pos)
{
    int pc, oppPc;
    Move mv;
    int len;
    int src, dst;

    src = pos;
    pc = board[src];
    const RookCannon_Move& horinz = *HorizonMovePtr(src);
    const RookCannon_Move& vertic = *VerticMovePtr(src);

    for(len = 0; len <= 1; len++)
    {
        if(horinz.rookCap[len] == GET_COL(src)) continue;
        dst = COORD_XY ( GET_ROW(src), horinz.rookCap[len]);
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    for(len = 0; len <= 1; len++)
    {
        if(vertic.rookCap[len] == GET_ROW(src)) continue;
        dst = COORD_XY ( vertic.rookCap[len], GET_COL(src));
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    int leftEnd = COORD_XY( GET_ROW(src), ( horinz.nonCap[0] ) - 1);

    for(dst = src - 1; dst > leftEnd; dst--)
    {
        oppPc = board[dst];
        if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    int rightEnd = COORD_XY(GET_ROW(src), ( horinz.nonCap[1] ) + 1);

    for(dst = src + 1; dst < rightEnd; dst++)
    {
        oppPc = board[dst];
        if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    int upperEnd = COORD_XY( (vertic.nonCap[0] ) - 1, GET_COL(src));
    for(dst = src - 0x10; dst > upperEnd; dst -= 0x10)
    {
        oppPc = board[dst];
        if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    int downEnd = COORD_XY( (vertic.nonCap[1] ) + 1, GET_COL(src));
    for(dst = src + 0x10; dst < downEnd; dst += 0x10)
    {
        oppPc = board[dst];
        if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }
}

void Position::genCannonMoves(QVector<Move>& mvs, int pos)
{
    int pc, oppPc;
    Move mv;
    int len;
    int src, dst;
    src = pos;
    pc = board[src];


    src = pieces[pc];
    const RookCannon_Move& horinz = *HorizonMovePtr(src);
    const RookCannon_Move& vertic = *VerticMovePtr(src);

    // qDebug() << "horizontal:";
    // qDebug() << horinz.nonCap[0] - 3 << horinz.nonCap[1] - 3;
    // qDebug() << horinz.cannonCap[0] - 3 << horinz.cannonCap[1] - 3;
    // printBitRow(GET_REL_ROW(src));
    // qDebug() << "vertical:";
    // qDebug() << vertic.nonCap[0] - 3 << vertic.nonCap[1] - 3;
    // qDebug() << vertic.cannonCap[0] - 3 << vertic.cannonCap[1] - 3;
    // printBitCol(GET_REL_COL(src));

    for(len = 0; len <= 1; len++)
    {
        if(horinz.cannonCap[len] == GET_COL(src)) continue;
        dst = COORD_XY ( GET_ROW(src), horinz.cannonCap[len]);
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    for(len = 0; len <= 1; len++)
    {
        if(vertic.cannonCap[len] == GET_ROW(src)) continue;
        dst = COORD_XY ( vertic.cannonCap[len], GET_COL(src));
        oppPc = board[dst];

        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }


    int leftEnd = COORD_XY(GET_ROW(src), ( horinz.nonCap[0]) - 1  );

    for(dst = src - 1; dst > leftEnd; dst--)
    {
        oppPc = board[dst];
        if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    int rightEnd = COORD_XY(GET_ROW(src), ( horinz.nonCap[1] ) + 1  );

    for(dst = src + 1; dst < rightEnd; dst++)
    {
        oppPc = board[dst];
        if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    int upperEnd = COORD_XY( (vertic.nonCap[0]) - 1, GET_COL(src));
    for(dst = src - 0x10; dst > upperEnd; dst -= 0x10)
    {
        oppPc = board[dst];
        if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }

    int downEnd = COORD_XY( ( vertic.nonCap[1]) + 1, GET_COL(src));
    for(dst = src + 0x10; dst < downEnd; dst += 0x10)
    {
        oppPc = board[dst];
        if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
    }
}

void Position::genPawnMoves(QVector<Move>& mvs, int pos)
{
    int pc, oppPc;
    Move mv;
    int len;
    int src, dst;
    int sd;
    src = pos;
    pc = board[src];
    sd = PIECE_SIDE(pc);
    len = 0;
    while( ( dst = PreGen.pawn_MoveTable[src][sd][len] ) != 0 )
    {
        oppPc = board[dst];
        if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }else if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv);
        }
        len++;
    }
}

void Position::genPieceMoves(QVector<Move>& mvs, int pos)
{
    int pc = board[pos];
    int index = PIECE_INDEX(pc);
    // printBoard();
    // qDebug() << pc << index;
    switch(index )
    {
    case KING_FROM:
        genKingMoves(mvs);
        break;
    case ADVISOR_FROM:
    case ADVISOR_TO:
        genAdvisorMoves(mvs, pos);
        break;
    case BISHOP_FROM:
    case BISHOP_TO:
        genBishopMoves(mvs, pos);
        break;
    case KNIGHT_FROM:
    case KNIGHT_TO:
        genKnightMoves(mvs, pos);
        break;
    case ROOK_FROM:
    case ROOK_TO:
        genRookMoves(mvs, pos);
        break;
    case CANNON_FROM:
    case CANNON_TO:
        genCannonMoves(mvs, pos);
        break;
    default:
        genPawnMoves(mvs, pos);
        break;
    }

}


