#include "position.h"

const int SIMPLE_VALUE[14] = {
  5,1,2,3,4,3,1,
  5,1,2,3,4,3,1
};

int Position::mvLva(int dst, int captured, int lva)
{
    int oppval = captured != 0 ? SIMPLE_VALUE[PIECE_TYPE(captured)] : 0;
    int adjust = 0;

    if( protectedBy(OPP_SIDE(side), dst) > 0)
    {
        adjust = lva;
    }

    return oppval - adjust;
}

void Position::genCapMoves(QVector<quint32> &mvs)
{
    int pc, oppPc;
    int sideTag = SIDE_TAG(this->side);
    Move mv;
    int len;
    int src, dst;

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
            mv.mvlva = mvLva(dst, oppPc, 5);
            mvs.push_back(mv.t);
        }
        len++;
    }

    for(pc = sideTag + ADVISOR_FROM; pc <= sideTag + ADVISOR_TO; pc ++)
    {
        src = pieces[pc];
        len = 0;
        while( ( dst = PreGen.advisor_MoveTable[src][len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 1);
                mvs.push_back(mv.t);
            }
            len++;
        }
    }

    for(pc = sideTag + BISHOP_FROM; pc <= sideTag + BISHOP_TO; pc ++)
    {
        src = pieces[pc];
        len = 0;
        const Bishop_Move& b = PreGen.bishop_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false && board[b.pin[len]] == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 2);
                mvs.push_back(mv.t);
            }
            len++;
        }
    }

    for(pc = sideTag + KNIGHT_FROM; pc <= sideTag + KNIGHT_TO; pc ++)
    {
        src = pieces[pc];
        len = 0;
        const Knight_Move& b = PreGen.knight_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false && board[b.pin[len]] == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 3);
                mvs.push_back(mv.t);
            }
            len++;
        }
    }

    for(pc = sideTag + CANNON_FROM; pc <= sideTag + CANNON_TO; pc ++)
    {
        src = pieces[pc];
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);
        for(len = 0; len <= 1; len++)
        {
            dst = COORD_XY ( GET_ROW(src), horinz.cannonCap[len]);
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) != false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 3);
                mvs.push_back(mv.t);
            }
        }

        for(len = 0; len <= 1; len++)
        {
            dst = COORD_XY ( vertic.cannonCap[len], GET_COL(src));
            oppPc = board[dst];

            if( oppPc != 0 && SAME_SIDE(pc, oppPc) != false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 3);
                mvs.push_back(mv.t);
            }
        }

    }

    for(pc = sideTag + ROOK_FROM; pc <= sideTag + ROOK_TO; pc ++)
    {
        src = pieces[pc];
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);
        for(len = 0; len <= 1; len++)
        {
            dst = COORD_XY ( GET_ROW(src), horinz.rookCap[len]);
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) != false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 4);
                mvs.push_back(mv.t);
            }
        }

        for(len = 0; len <= 1; len++)
        {
            dst = COORD_XY ( vertic.rookCap[len], GET_COL(src));
            oppPc = board[dst];

            if( oppPc != 0 && SAME_SIDE(pc, oppPc) != false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 4);
                mvs.push_back(mv.t);
            }
        }
    }

    for(pc = sideTag + PAWN_FROM; pc <= sideTag + PAWN_TO; pc ++)
    {
        src = pieces[pc];
        len = 0;
        while( ( dst = PreGen.pawn_MoveTable[src][side][len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 1);
                mvs.push_back(mv.t);
            }
            len++;
        }
    }

}

void Position::genNonCapMoves(QVector<quint32> &mvs)
{
    int pc, oppPc;
    int sideTag = SIDE_TAG(this->side);
    Move mv;
    int len;
    int src, dst;

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
            mv.mvlva = mvLva(dst, oppPc, 5);
            mvs.push_back(mv.t);
        }
        len++;
    }

    for(pc = sideTag + ADVISOR_FROM; pc <= sideTag + ADVISOR_TO; pc ++)
    {
        src = pieces[pc];
        len = 0;
        while( ( dst = PreGen.advisor_MoveTable[src][len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 1);
                mvs.push_back(mv.t);
            }
            len++;
        }
    }

    for(pc = sideTag + BISHOP_FROM; pc <= sideTag + BISHOP_TO; pc ++)
    {
        src = pieces[pc];
        len = 0;
        const Bishop_Move& b = PreGen.bishop_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc == 0 && board[b.pin[len]] == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 2);
                mvs.push_back(mv.t);
            }
            len++;
        }
    }

    for(pc = sideTag + KNIGHT_FROM; pc <= sideTag + KNIGHT_TO; pc ++)
    {
        src = pieces[pc];
        len = 0;
        const Knight_Move& b = PreGen.knight_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc == 0 && board[b.pin[len]] == 0)
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 3);
                mvs.push_back(mv.t);
            }
            len++;
        }
    }

    for(pc = sideTag + ROOK_FROM; pc <= sideTag + CANNON_TO; pc ++)
    {
        src = pieces[pc];
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);

        int leftEnd = COORD_XY(GET_ROW(src), horinz.nonCap[0] == 0 ? COL_LEFT : horinz.nonCap[0]);

        for(dst = src - 1; dst >= leftEnd; dst--)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 3);
                mvs.push_back(mv.t);
            }
        }

        int rightEnd = COORD_XY(GET_ROW(src), horinz.nonCap[1] == 0 ? COL_LEFT + 9: horinz.nonCap[1]);

        for(dst = src - 1; dst < rightEnd; dst++)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 3);
                mvs.push_back(mv.t);
            }
        }

        int upperEnd = COORD_XY(vertic.nonCap[0] == 0 ? ROW_TOP : vertic.nonCap[0], GET_COL(src));
        for(dst = src - 0x10; dst >= upperEnd; dst -= 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 3);
                mvs.push_back(mv.t);
            }
        }

        int downEnd = COORD_XY(vertic.nonCap[1] == 0 ? ROW_TOP + 10 : vertic.nonCap[1], GET_COL(src));
        for(dst = src + 0x10; dst < downEnd; dst += 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 3);
                mvs.push_back(mv.t);
            }
        }

    }

    for(pc = sideTag + PAWN_FROM; pc <= sideTag + PAWN_TO; pc ++)
    {
        src = pieces[pc];
        len = 0;
        while( ( dst = PreGen.pawn_MoveTable[src][side][len] ) != 0 )
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mv.mvlva = mvLva(dst, oppPc, 1);
                mvs.push_back(mv.t);
            }
            len++;
        }
    }
}

void Position::genKingMoves(QVector<quint32> &mvs)
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
            mvs.push_back(mv.t);
        }else if(oppPc == 0)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv.t);
        }
        len++;
    }
}

void Position::genAdvisorMoves(QVector<quint32> &mvs, int pos)
{
    int pc, oppPc;
    Move mv;
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
            mvs.push_back(mv.t);
        }else if(oppPc == 0)
        {
            mvs.push_back(mv.t);
        }
        len++;
    }
}

void Position::genBishopMoves(QVector<quint32> &mvs, int pos)
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
            mvs.push_back(mv.t);
        }else if(oppPc == 0 && board[b.pin[len] == 0])
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv.t);
        }
        len++;
    }
}

void Position::genKnightMoves(QVector<quint32> &mvs, int pos)
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
            mvs.push_back(mv.t);
        }else if(oppPc == 0 && board[b.pin[len]] == 0)
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv.t);
        }
        len++;
    }

}

void Position::genRookMoves(QVector<quint32> &mvs, int pos)
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
            dst = COORD_XY ( GET_ROW(src), horinz.rookCap[len]);
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) != false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        for(len = 0; len <= 1; len++)
        {
            dst = COORD_XY ( vertic.rookCap[len], GET_COL(src));
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) != false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        int leftEnd = COORD_XY(GET_ROW(src), horinz.nonCap[0] == 0 ? COL_LEFT : horinz.nonCap[0]);

        for(dst = src - 1; dst >= leftEnd; dst--)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        int rightEnd = COORD_XY(GET_ROW(src), horinz.nonCap[1] == 0 ? COL_LEFT + 9: horinz.nonCap[1]);

        for(dst = src - 1; dst < rightEnd; dst++)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        int upperEnd = COORD_XY(vertic.nonCap[0] == 0 ? ROW_TOP : vertic.nonCap[0], GET_COL(src));
        for(dst = src - 0x10; dst >= upperEnd; dst -= 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        int downEnd = COORD_XY(vertic.nonCap[1] == 0 ? ROW_TOP + 10 : vertic.nonCap[1], GET_COL(src));
        for(dst = src + 0x10; dst < downEnd; dst += 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }
}

void Position::genCannonMoves(QVector<quint32> &mvs, int pos)
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
            dst = COORD_XY ( GET_ROW(src), horinz.cannonCap[len]);
            oppPc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) != false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        for(len = 0; len <= 1; len++)
        {
            dst = COORD_XY ( vertic.cannonCap[len], GET_COL(src));
            oppPc = board[dst];

            if( oppPc != 0 && SAME_SIDE(pc, oppPc) != false )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        int leftEnd = COORD_XY(GET_ROW(src), horinz.nonCap[0] == 0 ? COL_LEFT : horinz.nonCap[0]);

        for(dst = src - 1; dst >= leftEnd; dst--)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        int rightEnd = COORD_XY(GET_ROW(src), horinz.nonCap[1] == 0 ? COL_LEFT + 9: horinz.nonCap[1]);

        for(dst = src - 1; dst < rightEnd; dst++)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        int upperEnd = COORD_XY(vertic.nonCap[0] == 0 ? ROW_TOP : vertic.nonCap[0], GET_COL(src));
        for(dst = src - 0x10; dst >= upperEnd; dst -= 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }

        int downEnd = COORD_XY(vertic.nonCap[1] == 0 ? ROW_TOP + 10 : vertic.nonCap[1], GET_COL(src));
        for(dst = src + 0x10; dst < downEnd; dst += 0x10)
        {
            oppPc = board[dst];
            if( oppPc == 0 )
            {
                mv.oi = src;
                mv.oe1 = dst;
                mvs.push_back(mv.t);
            }
        }


}

void Position::genPawnMoves(QVector<quint32> &mvs, int pos)
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
            mvs.push_back(mv.t);
        }else if( oppPc == 0 )
        {
            mv.oi = src;
            mv.oe1 = dst;
            mvs.push_back(mv.t);
        }
        len++;
    }
}
