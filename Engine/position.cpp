#include "position.h"
#include "entry/ucci.h"
#include "qdebug.h"
#include <iomanip>
#include <iostream>
using namespace std;

Position::Position() {
    init();
}

void Position::init()
{
    memset(board, 0, sizeof(board));
    memset(pieces, 0, sizeof(pieces));
    memset(bitBoard_row, 0, sizeof(bitBoard_row));
    memset(bitBoard_col, 0, sizeof(bitBoard_col));
    memset(repHash, 0, sizeof(repHash));

    zobri.setKey(0, 0, 0);
    bitPiece[0] = bitPiece[1] = 0;
    steps = 0;
    side = 0;
    repLenMask = REP_LEN - 1;

}

int Position::FromFen(const QString &fen)
{
    init();

    int i = 0, j = 0;
    int k = 0;
    int x;
    int pcWhite[7], pcBlack[7];
    pcWhite[0] = KING_FROM + SIDE_TAG(0);
    pcWhite[1] = ADVISOR_FROM+ SIDE_TAG(0);
    pcWhite[2] = BISHOP_FROM+ SIDE_TAG(0);
    pcWhite[3] = KNIGHT_FROM+ SIDE_TAG(0);
    pcWhite[4] = ROOK_FROM+ SIDE_TAG(0);
    pcWhite[5] = CANNON_FROM+ SIDE_TAG(0);
    pcWhite[6] = PAWN_FROM+ SIDE_TAG(0);
    for(k = 0; k < 7; k++)
    {
        pcBlack[k] = pcWhite[k] + 16;
    }
    for(k = 0; k < fen.length(); k++)
    {
        if(fen[k] >= 'A' && fen[k] <= 'Z')
        {
            x = FenPiece(fen[k]);
            addPiece( COORD_PC(i, j), pcWhite[x] );
            pcWhite[x] += 1;
            j += 1;
        }
        else if(fen[k] >= 'a' && fen[k] <= 'z')
        {
            x = FenPiece(fen[k]);
            addPiece( COORD_PC(i, j), pcBlack[x] );
            pcBlack[x] += 1;
            j += 1;
        }
        else if( fen[k] >= '0' && fen[k] <= '9')
        {
            j += fen[k].digitValue();
        }else if(fen[k] == '/')
        {
            i++;
            j = 0;
        }
    }

    return true;
}

QString Position::toFen()
{
    int nSpace;
    QString ret;
    int pos;
    int rSideTag = SIDE_TAG(0);
    ret = "";
    for(int i = ROW_TOP; i < ROW_TOP + 10; i++)
    {
        nSpace = 0;
        for(int j = COL_LEFT; j < COL_LEFT + 9; j++ )
        {
            pos = COORD_XY(i, j);
            if(board[ pos ] == 0)
            {
                nSpace += 1;
            }else
            {
                if(nSpace > 0)
                {
                    ret += QString::number(nSpace);
                    nSpace = 0;
                }

                if( board[ pos ] >= rSideTag && board[pos] < rSideTag + 16 )
                {
                    ret += ToFenPiece(board[pos]);
                }else
                {
                    ret += ToFenPiece(board[pos]).toLower();
                }
            }
        }
        if(nSpace > 0)
        {
            ret += QString::number(nSpace);
        }
        ret += "/";
    }
    ret.remove(ret.size() - 1, 1);
    return ret;
}

const RookCannon_MoveMask *Position::HorizonMaskPtr(int pos)
{
    int r = GET_ROW(pos) - ROW_TOP;
    int c = GET_COL(pos) - COL_LEFT;
    return &PreGen.rookCannon_MoveMaskTable_row[ c ][ bitBoard_row[r] ];
}

const RookCannon_MoveMask *Position::VerticMaskPtr(int pos)
{
    int r = GET_ROW(pos) - ROW_TOP;
    int c = GET_COL(pos) - COL_LEFT;
    return &PreGen.rookCannon_MoveMaskTable_col[r][ bitBoard_col[c] ];
}

const RookCannon_Move *Position::HorizonMovePtr(int pos)
{
    int r = GET_ROW(pos) - ROW_TOP;
    int c = GET_COL(pos) - COL_LEFT;
    return &PreGen.rookCannon_MoveTable_row[ c ][ bitBoard_row[r] ];

}

const RookCannon_Move *Position::VerticMovePtr(int pos)
{
    int r = GET_ROW(pos) - ROW_TOP;
    int c = GET_COL(pos) - COL_LEFT;
    return &PreGen.rookCannon_MoveTable_col[ r ][ bitBoard_col[c] ];
}


int Position::addPiece(int pos, int pc)
{
    int pt;

    board[pos] = pc;
    pieces[pc] = pos;

    bitBoard_row[GET_REL_ROW(pos)] ^= ( 1 << GET_REL_COL(pos) );
    bitBoard_col[GET_REL_COL(pos)] ^= ( 1 << GET_REL_ROW(pos) );
    bitPiece[PIECE_SIDE(pc)] ^= (1 << PIECE_INDEX(pc));

    pt = PIECE_TYPE_SIDE(pc);
    zobri.Xor(PreGen.zobriTable[pt][pos]);

    return 0;
}

int Position::delPiece(int pos)
{
    int pt;
    int pc = board[pos];
    pieces[pc] = 0;
    board[pos] = 0;

    bitBoard_row[GET_REL_ROW(pos)] ^= ( 1 << GET_REL_COL(pos) );
    bitBoard_col[GET_REL_COL(pos)] ^= ( 1 << GET_REL_ROW(pos) );
    bitPiece[PIECE_SIDE(pc)] ^= (1 << PIECE_INDEX(pc));

    pt = PIECE_TYPE_SIDE(pc);
    zobri.Xor(PreGen.zobriTable[pt][pos]);
    return 0;
}

int Position::makeMove(quint32 mv)
{
    Move* move = (Move*)&mv;

    saveStatus();
    quint32 oldPriKey = zobri._priKey;

    move->capture = movePiece(move);
    if(  (  checkedBy()) > 0)
    {
        undoMovePiece(move);
        rollback();
        return -1;
    }

    changeSide();
    move->chkchs = checkedBy();
    if(move->capture == 0)
    {
        if(move->chkchs == 0)
        {
            move->chkchs = -chasedBy(move->oe);
        }
        if(move->capture == -100)
        {
            move->capture = -100;
        }else
        {
            move->capture = qMin( (int)lastMove().capture, 0) -  (move->chkchs > 0 ? 0 : 1);
        }
    }

    trace[steps-1].mv.t = move->t;
    distance ++;
    if( repHash[oldPriKey & repLenMask ] == 0 )
    {
        repHash[oldPriKey & repLenMask] = distance;
    }

#ifdef BOARD_MOVE_DEBUG
    qDebug() << "makemv:" << ConvertToUcciCoord(trace[steps-1].mv.mv) << trace[steps-1].mv.capture;
#endif
    return 0;
}
int Position::undoMakeMove()
{
    if(steps == 0) return -1;
#ifdef BOARD_MOVE_DEBUG
    qDebug() << "undomakemv:" << ConvertToUcciCoord(trace[steps-1].mv.mv) << trace[steps-1].mv.capture;
#endif

    undoMovePiece(&(trace[steps-1].mv));
    side = OPP_SIDE(side);
    rollback();
    distance--;
    if( repHash[zobri._priKey & repLenMask ] == distance )
    {
        repHash[zobri._priKey & repLenMask] = 0;
    }

    return 0;
}


int Position::movePiece(Move *move)
{
    int src = move->o1;
    int dst = move->oe;
    int pcMoved = board[src];
    int pcCaptured = board[dst];


    Q_ASSERT( PIECE_SIDE(pcMoved) == side );

    board[src] = 0;
    board[dst] = pcMoved;
    pieces[pcMoved] = dst;

    bitBoard_row[GET_REL_ROW(src)] ^= ( 1 << GET_REL_COL(src) );
    bitBoard_col[GET_REL_COL(src)] ^= ( 1 << GET_REL_ROW(src) );

    if(pcCaptured != 0)
    {
        pieces[pcCaptured] = 0;
        bitPiece[PIECE_SIDE(pcCaptured)] ^= (1 << PIECE_INDEX(pcCaptured));
        zobri.Xor(PreGen.zobriTable[PIECE_TYPE_SIDE(pcCaptured)][dst]);

        if( pcCaptured < 32 )
        {
            vlRed -= preEval.valueRed[PIECE_TYPE(pcCaptured)][dst];
        }else
        {
            vlBlack -= preEval.valueBlack[PIECE_TYPE(pcCaptured)][dst];
        }

    }else
    {
        bitBoard_row[GET_REL_ROW(dst)] ^= ( 1 << GET_REL_COL(dst) );
        bitBoard_col[GET_REL_COL(dst)] ^= ( 1 << GET_REL_ROW(dst) );
    }

    int pt = PIECE_TYPE(pcMoved);
    if(side == 0)
    {
        vlRed += preEval.valueRed[pt][dst] - preEval.valueRed[pt][src];
    }else
    {
        vlBlack += preEval.valueBlack[pt][dst] - preEval.valueBlack[pt][src];
        pt += 7;
    }

    zobri.Xor(PreGen.zobriTable[pt][src]);
    zobri.Xor(PreGen.zobriTable[pt][dst]);

    return pcCaptured;
}

void Position::undoMovePiece(const Move *move)
{
    int src = move->o1;
    int dst = move->oe;
    int pcMoved = board[dst];
    int pcCaptured = move->capture;
    //int pt;
    board[src] = pcMoved;
    pieces[pcMoved] = src;

    Q_ASSERT(pcMoved != 0);


    bitBoard_row[GET_REL_ROW(src)] ^= ( 1 << GET_REL_COL(src) );
    bitBoard_col[GET_REL_COL(src)] ^= ( 1 << GET_REL_ROW(src) );

    //pt = PIECE_TYPE_SIDE(pcMoved);
    // zobristKey.Xor(PreGen.zobriTable[pt][dst]);
    // zobristKey.Xor(PreGen.zobriTable[pt][src]);

    if(pcCaptured > 0)
    {
        board[dst] = pcCaptured;
        pieces[pcCaptured] = dst;
        bitPiece[PIECE_SIDE(pcCaptured)] ^= (1 << PIECE_INDEX(pcCaptured));
        // zobristKey.Xor(PreGen.zobriTable[PIECE_TYPE_SIDE(pcCaptured)][dst]);
    }else
    {
        board[dst] = 0;
        bitBoard_row[GET_REL_ROW(dst)] ^= ( 1 << GET_REL_COL(dst) );
        bitBoard_col[GET_REL_COL(dst)] ^= ( 1 << GET_REL_ROW(dst) );
    }


}

void Position::nullMove()
{
    saveStatus();
    trace[steps - 1].mv.t = 0;
    distance++;
    changeSide();
}

void Position::undoNullMove()
{
    side = OPP_SIDE(side);
    rollback();
    distance--;
}

bool Position::legalMove(int mv)
{
    int src = SRC(mv);
    int dst = DST(mv);

    int pcMoved = board[src];
    int pcCaptured = board[dst];

    if(pcMoved == 0) return false;

    if(PIECE_SIDE(pcMoved) != side ) return false;

    if( IN_BOARD(dst) == 0 ) return false;

    // 不能吃己方棋子
    if(pcCaptured != 0 && SAME_SIDE(pcMoved, pcCaptured))
    {
        return false;
    }

    // 将
    switch( PIECE_INDEX(pcMoved) )
    {
    case KING_FROM:
        return IN_FORT(dst) && KING_SPAN( src, dst );
    case ADVISOR_FROM:
    case ADVISOR_TO:
        return IN_FORT(dst) && ADVISOR_SPAN( src, dst );
    case BISHOP_FROM:
    case BISHOP_TO:
        return SAME_HALF(src, dst) && BISHOP_SPAN(src, dst) && board[ BISHOP_PIN(src, dst)] == 0;
    case KNIGHT_FROM:
    case KNIGHT_TO:
    {
        int pinpos = KNIGHT_PIN(src, dst);
        return  pinpos != src && board[ pinpos ] == 0;
    }
    case ROOK_FROM:
    case ROOK_TO:
    {
        int sr = GET_ROW(src);
        int sc = GET_COL(src);
        int dr = GET_ROW(dst);
        int dc = GET_COL(dst);
        // 横向
        if(dr == sr)
        {
            const RookCannon_MoveMask& rc = *HorizonMaskPtr(src);
            if( pcCaptured == 0)
            {
                return ( rc.nonCap & PreGen.PieceMask_col[dst] ) != 0;
            }else
            {
                return ( rc.rookCap & PreGen.PieceMask_col[dst] ) != 0;
            }
        }else if(dc == sc)
        {
            const RookCannon_MoveMask& rc = *VerticMaskPtr(src);
            if( pcCaptured == 0)
            {
                return ( rc.nonCap & PreGen.PieceMask_row[dst] ) != 0;
            }else
            {
                return ( rc.rookCap & PreGen.PieceMask_row[dst] ) != 0;
            }
        }else return false;
    }

    case CANNON_FROM:
    case CANNON_TO:
    {
        int sr = GET_ROW(src);
        int sc = GET_COL(src);
        int dr = GET_ROW(dst);
        int dc = GET_COL(dst);
        // 横向
        if(dr == sr)
        {
            const RookCannon_MoveMask& rc = *HorizonMaskPtr(src);
            if( pcCaptured == 0)
            {
                return ( rc.nonCap & PreGen.PieceMask_col[dst] ) != 0;
            }else
            {
                return ( rc.cannonCap & PreGen.PieceMask_col[dst] ) != 0;
            }
        }else if(dc == sc)
        {
            const RookCannon_MoveMask& rc = *VerticMaskPtr(src);
            if( pcCaptured == 0)
            {
                return ( rc.nonCap & PreGen.PieceMask_row[dst] ) != 0;
            }else
            {
                return ( rc.cannonCap & PreGen.PieceMask_row[dst] ) != 0;
            }
        }else return false;
    }

    default:
    {

        if( AWAY_HALF(src, PIECE_SIDE(pcMoved)) && ( (src == (dst - 1) ) || ( src == (dst + 1)) ) )
        {
            return true;
        }
        else return SQUARE_FORWARD(src, PIECE_SIDE(pcMoved) ) == dst;
    }

    }
    return true;
}

int Position::genAllMoves(Move* mvs)
{
    int len = genCapMoves(mvs);
    len += genNonCapMoves(mvs + len);
    return len;
}

void Position::saveStatus()
{
    trace[steps] = (Trace(this->zobri, vlRed, vlBlack));
    steps++;
}

void Position::rollback()
{
    zobri = trace[steps-1].zobri;
    vlRed = trace[steps-1].vlRed;
    vlBlack = trace[steps-1].vlBlack;
    steps--;

}

void Position::changeSide()
{
    side = OPP_SIDE(side);
    zobri.Xor(PreGen.zobriPlayer);
}

int Position::checkedBy()
{
    int oppSide = OPP_SIDE(side);
    int oppSideTag = SIDE_TAG(oppSide);
    int kingPos = pieces[SIDE_TAG(side)];
    int oppPos;
    int oppPc;
    const RookCannon_MoveMask& rcmmHoriz = *HorizonMaskPtr(kingPos);
    const RookCannon_MoveMask& rcmmVer = *VerticMaskPtr(kingPos);

    //1. 检查对方的"车"是否能吃己方将
    for(oppPc = oppSideTag + ROOK_FROM; oppPc <= oppSideTag + ROOK_TO; oppPc++ )
    {
        oppPos = pieces[oppPc];
        // 横向
        if( GET_ROW(oppPos) == GET_ROW(kingPos) )
        {
            if( (rcmmHoriz.rookCap & PreGen.PieceMask_col[ oppPos  ] ) != 0 )
            {
                return oppPc;
            }
        }
        else if( GET_COL(oppPos) == GET_COL(kingPos))
        {
            if( (rcmmVer.rookCap & PreGen.PieceMask_row[ oppPos  ] ) != 0 )
            {
                return oppPc;
            }
        }
    }
    //2. 检查是否被对方"炮"将军.
    for(oppPc = oppSideTag + CANNON_FROM; oppPc <= oppSideTag + CANNON_TO; oppPc++ )
    {
        oppPos = pieces[oppPc];
        // 横向
        if( GET_ROW(oppPos) == GET_ROW(kingPos) )
        {
            if( (rcmmHoriz.cannonCap & PreGen.PieceMask_col[ oppPos  ] ) != 0 )
            {
                return oppPc;
            }
        }
        else if( GET_COL(oppPos) == GET_COL(kingPos))
        {
            if( (rcmmVer.cannonCap & PreGen.PieceMask_row[ oppPos  ] ) != 0 )
            {
                return oppPc;
            }
        }
    }
    //3. 检查是否被对方"马"将军.
    for(oppPc = oppSideTag + KNIGHT_FROM; oppPc <= oppSideTag + KNIGHT_TO; oppPc++ )
    {
        oppPos = pieces[oppPc];
        int pinpos = KNIGHT_PIN( oppPos, kingPos );
        if( pinpos != oppPos && board[pinpos] == 0 )
        {
            return oppPc;
        }
    }
    //4. 检查是否被对方"将"将军
    int oppKing = oppSideTag + KING_FROM;
    oppPos = pieces[oppKing];
    if( GET_COL(oppPos) == GET_COL(kingPos) )
    {
        if( ( rcmmVer.rookCap & PreGen.PieceMask_row[oppPos] ) != 0 )
        {
            return oppKing;
        }
    }

    //5. 检查是否被对方"兵"将军.
    for(oppPos = kingPos - 1; oppPos <= kingPos + 1; oppPos += 2)
    {
        oppPc = board[oppPos];
        if( SAME_SIDE(oppPc, oppSideTag + KING_FROM) && PIECE_INDEX(oppPc) >= PAWN_FROM )
        {
            return oppPc;
        }
    }
    oppPos = SQUARE_FORWARD(kingPos, this->side);
    oppPc = board[oppPos];
    if( SAME_SIDE(oppPc, oppSideTag + KING_FROM) && PIECE_INDEX(oppPc) >= PAWN_FROM)
        return oppPc;


    return 0;
}

/**
 * @brief 判断dst位置处棋子是否受side保护.
 * @param side
 * @param dst
 * @return 若dst位置不受保护,返回0
 */
int Position::protectedBy(int side, int dst)
{
    int sideTag = SIDE_TAG(side);
    int pc;
    int pos;
    const RookCannon_MoveMask& rcmmHoriz = *HorizonMaskPtr(dst);
    const RookCannon_MoveMask& rcmmVer = *VerticMaskPtr(dst);
    int kingPos = pieces[sideTag];
    if( SAME_HALF(dst, kingPos) )
    {
        if( IN_FORT(dst))
        {
            pc = sideTag + KING_FROM;
            if( KING_SPAN( pieces[pc], dst) )
            {
                return pc;
            }

            for(pc = sideTag + ADVISOR_FROM; pc <= sideTag + ADVISOR_TO; pc++)
            {
                pos = pieces[pc];
                if( ADVISOR_SPAN( pos, dst ) )
                {
                    return pc;
                }
            }
        }

        for(pc = sideTag + BISHOP_FROM; pc <= sideTag + BISHOP_TO; pc++)
        {
            pos = pieces[pc];
            if( BISHOP_SPAN( pos, dst ) && board[BISHOP_PIN(pos, dst)] == 0 )
            {
                return pc;
            }
        }
        pos = SQUARE_BACKWARD( dst, side );
        pc = board[pos];
        if( ( SAME_SIDE(pc, sideTag) ) != 0 && PIECE_INDEX(pc) >= PAWN_FROM )
        {
            return pc;
        }

    }else
    {
        for(pos = dst - 1; pos <= dst + 1; pos += 2)
        {
            pc = board[pos];
            if( SAME_SIDE(pc, sideTag + KING_FROM) && PIECE_INDEX(pc) >= PAWN_FROM )
            {
                return pc;
            }
        }

        pos = SQUARE_BACKWARD( dst, side );
        pc = board[pos];
        if( SAME_SIDE(pc, sideTag + KING_FROM) && PIECE_INDEX(pc) >= PAWN_FROM )
        {
            return pc;
        }
    }

    for(pc = sideTag + ROOK_FROM; pc <= sideTag + ROOK_TO; pc++)
    {
        pos = pieces[pc];

        if( GET_ROW(pos) == GET_ROW(dst) )
        {
            if ( ( rcmmHoriz.rookCap & PreGen.PieceMask_col[pos] ) != 0 )
            {
                return pc;
            }
        }else if( GET_COL(pos) == GET_COL(dst) )
        {
            if( (rcmmVer.rookCap & PreGen.PieceMask_row[pos]) != 0 )
            {
                return pc;
            }
        }
    }

    for(pc = sideTag + CANNON_FROM; pc <= sideTag + CANNON_TO; pc++)
    {
        pos = pieces[pc];
        if( GET_ROW(pos) == GET_ROW(dst) )
        {
            if ( ( rcmmHoriz.cannonCap & PreGen.PieceMask_col[pos] ) != 0 )
            {
                return pc;
            }
        }else if( GET_COL(pos) == GET_COL(dst) )
        {
            if( (rcmmVer.cannonCap & PreGen.PieceMask_row[pos]) != 0 )
            {
                return pc;
            }
        }
    }

    for(pc = sideTag + KNIGHT_FROM; pc <= sideTag + KNIGHT_TO; pc++)
    {
        pos = pieces[pc];
        int pinpos = KNIGHT_PIN(pos, dst);
        if(pinpos != pos && board[pinpos] == 0)
        {
            return pos;
        }
    }
    return 0;
}

// 检测对方在pos位置的棋子能捉己方的哪个子.
// 带有保护判断
int Position::chasedBy(int pos)
{

    int oppPc = board[pos];
    int pc;
    int src = pieces[oppPc];
    int dst;
    int len;
    int pcIndex;

    // if(SAME_SIDE(oppPc, SIDE_TAG(side)+KING_FROM))
    // {
    //     printBoard();
    // }

    Q_ASSERT(!SAME_SIDE(oppPc, SIDE_TAG(side)+KING_FROM));

    switch(PIECE_INDEX(oppPc))
    {
    case KNIGHT_FROM:
    case KNIGHT_TO:
    {
        len = 0;
        const Knight_Move& b = PreGen.knight_MoveTable[src];
        while( ( dst = b.tar[len] ) != 0 )
        {
            pc = board[dst];
            if( pc != 0 && SAME_SIDE(pc, oppPc) == false && board[b.pin[len]] == 0)
            {
                pcIndex = PIECE_INDEX(pc);
                // 对方马只捉车,炮
                if( pcIndex <= ROOK_TO )
                {
                    if(pcIndex >= ROOK_FROM)
                    {
                        return pc;
                    }
                }else
                {
                    if(pcIndex <= CANNON_TO)
                    {
                        // 无保护捉炮.
                        if( !protectedBy(side, dst) )
                        {
                            return pc;
                        }
                    }else
                    {
                        // 兵过河,无保护捉兵
                        if( AWAY_HALF(dst, side) && !protectedBy(side, dst))
                        {
                            return pc;
                        }
                    }
                }
            }
            len++;
        }
    }
        return 0;
    case ROOK_FROM:
    case ROOK_TO:
    {
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);

        for(len = 0; len <= 1; len++)
        {
            if(horinz.rookCap[len] == GET_COL(src)) continue;
            dst = COORD_XY ( GET_ROW(src), horinz.rookCap[len]);
            pc = board[dst];
            if( pc != 0 && SAME_SIDE(pc, oppPc) == false )
            {
                pcIndex = PIECE_INDEX(pc);
                if(pcIndex <= ROOK_TO )
                {
                    if(pcIndex >= KNIGHT_FROM && pcIndex <= KNIGHT_TO)
                    {
                        if(!protectedBy(side, dst))
                        {
                            return pc;
                        }
                    }

                }else
                {
                    if(pcIndex <= CANNON_TO)
                    {
                        if(!protectedBy(side, dst))
                        {
                            return pc;
                        }
                    }else
                    {
                        if(AWAY_HALF(dst, side) && !protectedBy(side, dst))
                        {
                            return pc;
                        }
                    }
                }
            }
        }

        for(len = 0; len <= 1; len++)
        {
            if(vertic.rookCap[len] == GET_ROW(src)) continue;
            dst = COORD_XY ( vertic.rookCap[len], GET_COL(src));
            pc = board[dst];
            if( pc != 0 && SAME_SIDE(pc, oppPc) == false )
            {
                pcIndex = PIECE_INDEX(pc);
                if(pcIndex <= ROOK_TO )
                {
                    if(pcIndex >= KNIGHT_FROM && pcIndex <= KNIGHT_TO)
                    {
                        if(!protectedBy(side, dst))
                        {
                            return pc;
                        }
                    }

                }else
                {
                    if(pcIndex <= CANNON_TO)
                    {
                        if(!protectedBy(side, dst))
                        {
                            return pc;
                        }
                    }else
                    {
                        if(AWAY_HALF(dst, side) && !protectedBy(side, dst))
                        {
                            return pc;
                        }
                    }
                }
            }
        }
    }
        return 0;

    case CANNON_FROM:
    case CANNON_TO:
    {
        const RookCannon_Move& horinz = *HorizonMovePtr(src);
        const RookCannon_Move& vertic = *VerticMovePtr(src);

        for(len = 0; len <= 1; len++)
        {
            if(horinz.cannonCap[len] == GET_COL(src)) continue;
            dst = COORD_XY ( GET_ROW(src), horinz.cannonCap[len]);
            pc = board[dst];
            if( oppPc != 0 && SAME_SIDE(pc, oppPc) == false )
            {
                pcIndex = PIECE_INDEX(pc);
                if(pcIndex <= ROOK_TO)
                {
                    if(pcIndex >= ROOK_FROM)
                    {
                        return pc;
                    }
                    else if(pcIndex >= KNIGHT_FROM)
                    {
                        if(!protectedBy(side, pc))
                        {
                            return pc;
                        }
                    }
                }else
                {
                    if(pcIndex >= PAWN_FROM)
                    {
                        if(AWAY_HALF(dst, side) && !protectedBy(side, pc))
                        {
                            return pc;
                        }
                    }
                }
            }
        }

        for(len = 0; len <= 1; len++)
        {
            if(vertic.cannonCap[len] == GET_ROW(src)) continue;
            dst = COORD_XY ( vertic.cannonCap[len], GET_COL(src));
            pc = board[dst];

            if( pc != 0 && SAME_SIDE(pc, oppPc) == false )
            {
                pcIndex = PIECE_INDEX(pc);
                if(pcIndex <= ROOK_TO)
                {
                    if(pcIndex >= ROOK_FROM)
                    {
                        return pc;
                    }
                    else if(pcIndex >= KNIGHT_FROM)
                    {
                        if(!protectedBy(side, pc))
                        {
                            return pc;
                        }
                    }
                }else
                {
                    if(pcIndex >= PAWN_FROM)
                    {
                        if(AWAY_HALF(dst, side) && !protectedBy(side, pc))
                        {
                            return pc;
                        }
                    }
                }
            }
        }

    }
    break;
    }

    return 0;
}

bool Position::bMate()
{
    Move mvs[120];
    int cnt;
    int i;

    cnt = genCapMoves(mvs);
    for(i = 0; i < cnt;i++)
    {
        if( makeMove( mvs[i].mv ) == 0 )
        {
            undoMakeMove();
            return false;
        }
    }

    cnt = genNonCapMoves(mvs);
    for(i = 0; i < cnt;i++)
    {
        if( makeMove(mvs[i].mv ) == 0 )
        {
            undoMakeMove();
            return false;
        }
    }

    return true;
}

int setPerpCheck(int chkchs, int mask)
{
    if(chkchs > 0)
    {
        mask = 0x10000;
    }else if(chkchs < 0)
    {
        mask = ( 1 << PIECE_INDEX( -chkchs ) ) & mask;
    }else
    {
        mask = 0;
    }

    return mask;
}

int Position::repStatus(int repOccur)
{
    int mask = 0x1ffff;
    int oppMask = 0x1ffff;
    int step = steps - 1;
    int _side = OPP_SIDE(side);
    int chkchs;

    if( repHash[zobri._priKey & repLenMask] == 0 )
    {
        return REP_NONE;
    }

    while(step >= 0 && trace[step].mv.mv != 0 && trace[step].mv.capture <= 0)
    {
        chkchs = trace[step].mv.chkchs;

        if( _side == side )
        {
            mask = setPerpCheck(chkchs, mask);

            if( trace[step].zobri == this->zobri )
            {
                repOccur--;
                if(repOccur == 0)
                {
                    return mask > oppMask ? REP_LOSE : mask < oppMask ? REP_WIN : REP_DRAW;
                }
            }
        }else
        {
            oppMask = setPerpCheck(chkchs, oppMask);
        }
        step -= 1;
        _side = OPP_SIDE(_side);
    }

    return REP_NONE;
}



void Position::printBoard()
{
    int i, j;
    QString str;
    for(i = 0; i < 16; i++ )
    {
        for(j = 0; j < 16; j++)
        {
            str += QString("%1 ").arg((int)board[i * 16 + j], 2, 10, QLatin1Char('0'));
        }
        qDebug() << str;
        str.clear();
    }

}

void Position::printPieces()
{
    int i;
    for(i = SIDE_TAG(0); i < SIDE_TAG(0) + 16; i++)
    {
        cout << (unsigned int)pieces[i] << ", ";
    }
    cout << endl;
    for(i = SIDE_TAG(1); i < SIDE_TAG(1) + 16; i++)
    {
        cout << (unsigned int)pieces[i] << ", ";
    }
    cout << endl;

    qDebug() << "current side: " << (side == 0 ? "red" : "black");
}

