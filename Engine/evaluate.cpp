#include "position.h"

const int PENALTY_CENTER_KING = 20;

const int AS_GAP = 160;
const int SH_GAP = 80;
const int RM_GAP = 30;
const int KT_GAP = 20;
const int SHAPE_NONE = 0;
const int SHAPE_LEFT = 1;
const int SHAPE_CENTER = 2;
const int SHAPE_RIGHT = 3;

int Position::advisorShape()
{

    int i,j;
    int ap0, ap1;
    int sideTag;
    int oppSideTag;
    int shape;

    int kingPos;
    int pos;
    int oppPos;
    int rp = 0;
    int bp = 0;

    shape = SHAPE_NONE;
    sideTag = SIDE_TAG(0);
    oppSideTag = SIDE_TAG(1);
    kingPos = pieces[sideTag + KING_FROM];

    // 计算红方士形罚分
    if( ( bitPiece[0] & BIT_ADVISOR ) == BIT_ADVISOR )
    {
        if( kingPos == 0xc7 )
        {
            ap0 = pieces[sideTag + ADVISOR_FROM];
            ap1 = pieces[sideTag + ADVISOR_TO];

            if(ap0 == 0xc6)
            {
                shape = (ap1 == 0xb7 ? SHAPE_LEFT : ( ap1 == 0xc8 ? SHAPE_CENTER : SHAPE_NONE) );
            }else if(ap0 == 0xb7)
            {
                shape = (ap1 == 0xc6 ? SHAPE_LEFT : (ap1 == 0xc8 ? SHAPE_RIGHT : SHAPE_NONE));
            }else if(ap0 == 0xc8)
            {
                shape = (ap1 == 0xc6 ? SHAPE_CENTER : (ap1 == 0xb7 ? SHAPE_RIGHT : SHAPE_NONE));
            }

            switch(shape)
            {
            case SHAPE_CENTER:
            {
                for(i = oppSideTag + CANNON_FROM; i <= oppSideTag + CANNON_TO; i++)
                {
                    oppPos = pieces[i];
                    if(oppPos == 0) continue;
                    const auto& vertic = VerticMaskPtr(oppPos);

                    if( GET_COL(oppPos) == 0x07)
                    {
                        if( (vertic->rookCap & PreGen.PieceMask_row[kingPos]) != 0 )
                        {
                            rp += preEval.valueBlackHollowThreat[GET_ROW(oppPos)];
                        }
                        else if( (vertic->superCannonCap & PreGen.PieceMask_row[kingPos] ) != 0 )
                        {
                            if( board[0xb7] == sideTag + KNIGHT_FROM || board[0xb7] == sideTag + KNIGHT_TO )
                            {
                                rp += preEval.valueBlackCentralThreat[GET_ROW(oppPos)];
                            }
                        }
                    }
                }
            }
                break;
            case SHAPE_LEFT:
            case SHAPE_RIGHT:
                for(i = oppSideTag + CANNON_FROM; i <= oppSideTag + CANNON_TO; i++)
                {
                    oppPos = pieces[i];
                    if(oppPos == 0) continue;
                    const auto& vertic = VerticMaskPtr(oppPos);

                    if( GET_COL(oppPos) == 0x07)
                    {
                        if( (vertic->superCannonCap & PreGen.PieceMask_row[kingPos] ) != 0 )
                        {
                            rp += ( preEval.valueBlackCentralThreat[GET_ROW(oppPos)] >> 2 )
                                  + (protectedBy(1, (shape == SHAPE_LEFT ? 0xc8 : 0xc6) ) ? 20 : 0);
                            for( j =  sideTag + ROOK_FROM; j <= sideTag + ROOK_TO; j++)
                            {
                                pos = pieces[j];
                                if( pos != 0 )
                                {
                                    const auto& h = HorizonMaskPtr(pos);
                                    if( GET_ROW(kingPos) == GET_ROW(pos) )
                                    {
                                        if( (h->rookCap & PreGen.PieceMask_col[kingPos]) != 0 )
                                        {
                                            rp += 80;
                                        }
                                    }
                                }
                            }
                        }
                    }else if(GET_ROW(oppPos) == GET_ROW(kingPos))
                    {
                        const auto& horiz = HorizonMaskPtr(oppPos);
                        if( (horiz->rookCap & PreGen.PieceMask_col[kingPos] ) != 0 )
                        {
                            rp += preEval.valBlackBottomThreat[ GET_COL(oppPos) ];
                        }
                    }
                }
                break;
            }
        }else if( kingPos == 0xb7 )
        {
            rp += PENALTY_CENTER_KING;
        }
    }else
    {
        if( ( bitPiece[1] & BIT_ROOK ) == BIT_ROOK )
        {
            rp += preEval.valRedAdvisorLeakage;
        }
    }

    shape = SHAPE_NONE;
    sideTag = SIDE_TAG(1);
    oppSideTag = SIDE_TAG(0);
    kingPos = pieces[sideTag + KING_FROM];

    // 计算黑方士形罚分
    if( ( bitPiece[0] & BIT_ADVISOR ) == BIT_ADVISOR )
    {
        if( kingPos == 0x37 )
        {
            ap0 = pieces[sideTag + ADVISOR_FROM];
            ap1 = pieces[sideTag + ADVISOR_TO];

            if(ap0 == 0x36)
            {
                shape = (ap1 == 0x47 ? SHAPE_LEFT : ( ap1 == 0x38 ? SHAPE_CENTER : SHAPE_NONE) );
            }else if(ap0 == 0x47)
            {
                shape = (ap1 == 0x36 ? SHAPE_LEFT : (ap1 == 0x38 ? SHAPE_RIGHT : SHAPE_NONE));
            }else if(ap0 == 0x38)
            {
                shape = (ap1 == 0x36 ? SHAPE_CENTER : (ap1 == 0x47 ? SHAPE_RIGHT : SHAPE_NONE));
            }

            switch(shape)
            {
            case SHAPE_CENTER:
            {
                for(i = oppSideTag + CANNON_FROM; i <= oppSideTag + CANNON_TO; i++)
                {
                    oppPos = pieces[i];
                    if(oppPos == 0) continue;
                    const auto& vertic = VerticMaskPtr(oppPos);

                    if( GET_COL(oppPos) == 0x07)
                    {
                        if( (vertic->rookCap & PreGen.PieceMask_row[kingPos]) != 0 )
                        {
                            bp += preEval.valueRedHollowThreat[GET_ROW(oppPos)];
                        }
                        else if( (vertic->superCannonCap & PreGen.PieceMask_row[kingPos] ) != 0 )
                        {
                            if( board[0x47] == sideTag + KNIGHT_FROM || board[0x47] == sideTag + KNIGHT_TO )
                            {
                                bp += preEval.valueRedCentralThreat[GET_ROW(oppPos)];
                            }
                        }
                    }
                }
            }
            break;
            case SHAPE_LEFT:
            case SHAPE_RIGHT:
                for(i = oppSideTag + CANNON_FROM; i <= oppSideTag + CANNON_TO; i++)
                {
                    oppPos = pieces[i];
                    if(oppPos == 0) continue;
                    const auto& vertic = VerticMaskPtr(oppPos);

                    if( GET_COL(oppPos) == 0x07)
                    {
                        if( (vertic->superCannonCap & PreGen.PieceMask_row[kingPos] ) != 0 )
                        {
                            bp += ( preEval.valueRedCentralThreat[GET_ROW(oppPos)] >> 2 )
                            + (protectedBy(0, (shape == SHAPE_LEFT ? 0x38 : 0x36) ) ? 20 : 0);
                            for( j =  sideTag + ROOK_FROM; j <= sideTag + ROOK_TO; j++)
                            {
                                pos = pieces[j];
                                if( pos != 0 )
                                {
                                    const auto& h = HorizonMaskPtr(pos);
                                    if( GET_ROW(kingPos) == GET_ROW(pos) )
                                    {
                                        if( (h->rookCap & PreGen.PieceMask_col[kingPos]) != 0 )
                                        {
                                            bp += 80;
                                        }
                                    }
                                }
                            }
                        }
                    }else if(GET_ROW(oppPos) == GET_ROW(kingPos))
                    {
                        const auto& horiz = HorizonMaskPtr(oppPos);
                        if( (horiz->rookCap & PreGen.PieceMask_col[kingPos] ) != 0 )
                        {
                            bp += preEval.valRedBottomThreat[ GET_COL(oppPos) ];
                        }
                    }
                }
                break;
            }
        }else if( kingPos == 0x47 )
        {
            bp += PENALTY_CENTER_KING;
        }
    }else
    {
        if( ( bitPiece[0] & BIT_ROOK ) == BIT_ROOK )
        {
            bp += preEval.valBlackAdvisorLeakage;
        }
    }

    return SIDE_VALUE(side, bp - rp);

}



quint8 KnightBadPosition[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int Position::knightTrap()
{
    int src;
    int dst;
    int i;
    int sideTag;
    int oppPc;
    int pc;
    int len;
    int pin;
    Knight_Move km;
    int moveNum;
    int penalty[2] = {0};
    for(i = 0; i < 2; i++)
    {
        sideTag = SIDE_TAG(i);
        for(pc = sideTag + KNIGHT_FROM; pc <= sideTag + KNIGHT_TO; pc++)
        {
            src = pieces[pc];
            if(src == 0) continue;
            len = 0;
            km = PreGen.knight_MoveTable[src];
            moveNum = 0;
            while(  ( dst = km.tar[len] ) != 0)
            {
                pin = km.pin[len];
                oppPc = board[dst];
                if( !KnightBadPosition[dst] && board[pin] == 0 && ((oppPc != 0 && !SAME_SIDE(oppPc, pc)) || (oppPc == 0) ) && !protectedBy(OPP_SIDE(i), dst) )
                {
                    moveNum += 1;
                    if(moveNum > 1)
                    {
                        break;
                    }
                }
                len++;
            }

            if(moveNum == 0)
            {
                penalty[i] += 10;
            }else if(moveNum == 1)
            {
                penalty[i] += 5;
            }
        }
    }

    return SIDE_VALUE(side, penalty[1] - penalty[0]);
}

const int StringHold[512] = {
                                0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
    12, 16, 20, 24, 28, 32, 36, 0, 36, 32, 28, 24, 20, 16, 12,  0,
    0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0
};

int Position::stringHold()
{


    int kingPos;
    int pc;
    int pos;
    int oppPc;
    int oppPos;
    int penalty[2] = {0};


    for(int i = 0; i < 2; i++)
    {
        int sideTag = SIDE_TAG(i);
        int oppSideTag = SIDE_TAG(OPP_SIDE(i));

        kingPos = pieces[sideTag + KING_FROM];

        // 对方棋子牵制己方将
        for( oppPc = oppSideTag + ROOK_FROM; oppPc <= oppSideTag + ROOK_TO; oppPc++ )
        {
            oppPos = pieces[oppPc];
            if(oppPos == 0) continue;
            const auto& vertic = VerticMovePtr(oppPos);
            const auto& horiz = HorizonMovePtr(oppPos);
            if( GET_COL(oppPos) == GET_COL(kingPos) )
            {
                int dir = oppPos > kingPos ? 0 : 1;
                if( vertic->cannonCap[dir] == GET_ROW(kingPos) )
                {
                    int tarPos = COORD_XY(vertic->rookCap[dir], GET_COL(oppPos));
                    int tarPc = board[tarPos];
                    if( tarPc != 0 && !SAME_SIDE(tarPc, oppPc) && !protectedBy(i, tarPos) )
                    {
                        penalty[i] += StringHold[ tarPos - oppPos + 256 ];
                    }
                }


            }else if(GET_ROW(oppPos) == GET_ROW(kingPos))
            {
                int dir = oppPos > kingPos ? 0 : 1;
                if( horiz->cannonCap[dir] == GET_COL(kingPos) )
                {
                    int tarPos = COORD_XY(GET_ROW(oppPos), horiz->rookCap[dir]);
                    int tarPc = board[tarPos];
                    if( tarPc != 0 && !SAME_SIDE(tarPc, oppPc) && !protectedBy(i, tarPos) )
                    {
                        penalty[i] += StringHold[ tarPos - oppPos + 256 ];
                    }
                }
            }
        }

        // 对方车牵制己方车
        for( pc = sideTag + ROOK_FROM; pc <= sideTag + ROOK_TO; pc++ )
        {
            pos = pieces[pc];
            if(pos == 0 ) continue;

            for( oppPc = oppSideTag + ROOK_FROM; oppPc <= oppSideTag + ROOK_TO; oppPc++ )
            {
                oppPos = pieces[oppPc];
                if(oppPos == 0) continue;
                const auto& vertic = VerticMovePtr(oppPos);
                const auto& horiz = HorizonMovePtr(oppPos);

                if( GET_COL(oppPos) == GET_COL(pos) )
                {
                    int dir = oppPos > pos ? 0 : 1;
                    if( vertic->cannonCap[dir] == GET_ROW(pos) )
                    {
                        int tarPos = COORD_XY(vertic->rookCap[dir], GET_COL(oppPos));
                        int tarPc = board[tarPos];
                        if( tarPc != 0 && !SAME_SIDE(tarPc, oppPc) && !protectedBy(i, tarPos) )
                        {
                            penalty[i] += StringHold[ tarPos - oppPos + 256 ];
                        }
                    }
                }else if(GET_ROW(oppPos) == GET_ROW(pos))
                {
                    int dir = oppPos > pos ? 0 : 1;
                    if( horiz->cannonCap[dir] == GET_COL(pos) )
                    {
                        int tarPos = COORD_XY(GET_ROW(oppPos), horiz->rookCap[dir]);
                        int tarPc = board[tarPos];
                        if( tarPc != 0 && !SAME_SIDE(tarPc, oppPc) && !protectedBy(i, tarPos) )
                        {
                            penalty[i] += StringHold[ tarPos - oppPos + 256 ];
                        }
                    }
                }
            }
        }

        // 对方炮牵制己方将
        for( oppPc = oppSideTag + CANNON_FROM; oppPc <= oppSideTag + CANNON_TO; oppPc++ )
        {
            oppPos = pieces[oppPc];
            if(oppPos == 0) continue;
            const auto& vertic = VerticMovePtr(oppPos);
            const auto& horiz = HorizonMovePtr(oppPos);

            if( GET_COL(oppPos) == GET_COL(kingPos) )
            {
                int dir = oppPos > kingPos ? 0 : 1;
                if( vertic->superCannonCap[dir] == GET_ROW(kingPos) )
                {
                    int tarPos = COORD_XY(vertic->cannonCap[dir], GET_COL(oppPos));
                    int tarPc = board[tarPos];
                    if( tarPc != 0 && !SAME_SIDE(tarPc, oppPc) && !protectedBy(i, tarPos) )
                    {
                        penalty[i] += StringHold[ tarPos - oppPos + 256 ];
                    }
                }
            }else if(GET_ROW(oppPos) == GET_ROW(kingPos))
            {
                int dir = oppPos > kingPos ? 0 : 1;
                if( horiz->superCannonCap[dir] == GET_COL(kingPos) )
                {
                    int tarPos = COORD_XY(GET_ROW(oppPos), horiz->cannonCap[dir]);
                    int tarPc = board[tarPos];
                    if( tarPc != 0 && !SAME_SIDE(tarPc, oppPc) && !protectedBy(i, tarPos) )
                    {
                        penalty[i] += StringHold[ tarPos - oppPos + 256 ];
                    }
                }
            }
        }

        // 对方炮牵制己方车
        for( pc = sideTag + ROOK_FROM; pc <= sideTag + ROOK_TO; pc++ )
        {
            pos = pieces[pc];
            if(pos == 0 ) continue;

            for( oppPc = oppSideTag + CANNON_FROM; oppPc <= oppSideTag + CANNON_TO; oppPc++ )
            {
                oppPos = pieces[oppPc];
                if(oppPos == 0) continue;
                if(oppPos != 0)
                {
                    const auto& vertic = VerticMovePtr(oppPos);
                    const auto& horiz = HorizonMovePtr(oppPos);

                    if( GET_COL(oppPos) == GET_COL(pos) )
                    {
                        int dir = oppPos > pos ? 0 : 1;
                        if( vertic->superCannonCap[dir] == GET_ROW(pos) )
                        {
                            int tarPos = COORD_XY(vertic->cannonCap[dir], GET_COL(oppPos));
                            int tarPc = board[tarPos];
                            if( tarPc != 0 && !SAME_SIDE(tarPc, oppPc) && !protectedBy(i, tarPos) )
                            {
                                penalty[i] += StringHold[ tarPos - oppPos + 256 ];
                            }
                        }
                    }else if(GET_ROW(oppPos) == GET_ROW(pos))
                    {
                        int dir = oppPos > pos ? 0 : 1;
                        if( horiz->superCannonCap[dir] == GET_COL(pos) )
                        {
                            int tarPos = COORD_XY(GET_ROW(oppPos), horiz->cannonCap[dir]);
                            int tarPc = board[tarPos];
                            if( tarPc != 0 && !SAME_SIDE(tarPc, oppPc) && !protectedBy(i, tarPos) )
                            {
                                penalty[i] += StringHold[ tarPos - oppPos + 256 ];
                            }
                        }
                    }
                }
            }
        }
    }

    return SIDE_VALUE(side, penalty[1] - penalty[0]);


}

int Position::rookMobility()
{

    int bounce[2] = {0};
    int i;
    int sideTag;
    int pc;
    int pos;
    for(i = 0; i < 2; i++)
    {
        sideTag = SIDE_TAG(i);

        for(pc = sideTag + ROOK_FROM; pc <= sideTag + ROOK_TO; pc++)
        {
            pos = pieces[pc];
            if(pos == 0) continue;

            const auto& horiz = HorizonMovePtr(pos);
            const auto& vertic = VerticMovePtr(pos);

            bounce[i] += ( GET_COL(pos) - horiz->nonCap[0] ) + (horiz->nonCap[1] - GET_COL(pos));
            bounce[i] += (GET_ROW(pos) - vertic->nonCap[0]) + (vertic->nonCap[1] - GET_ROW(pos));
        }
    }

    return SIDE_VALUE(side, bounce[0] - bounce[1]);
}

int Position::evaluate(int alpha, int beta)
{
    int ret = material();
    if(ret + AS_GAP <= alpha)
    {
        return ret + AS_GAP;
    }
    else if( ret - AS_GAP >= beta )
    {
        return ret - AS_GAP;
    }

    ret += advisorShape();
    if(ret + SH_GAP <= alpha)
    {
        return ret + SH_GAP;
    }
    else if( ret - SH_GAP >= beta )
    {
        return ret - SH_GAP;
    }

    ret += stringHold();

    if(ret + RM_GAP <= alpha)
    {
        return ret + RM_GAP;
    }
    else if( ret - RM_GAP >= beta )
    {
        return ret - RM_GAP;
    }

    ret += rookMobility();

    if(ret + KT_GAP <= alpha)
    {
        return ret + KT_GAP;
    }
    else if( ret - KT_GAP >= beta )
    {
        return ret - KT_GAP;
    }

    ret += knightTrap();


    return ret;
}


