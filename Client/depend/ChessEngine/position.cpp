#include "position.h"

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
static inline int FenPiece(const QChar& c)
{
    switch(c.unicode())
    {
    case 'K':
        return 0;
    case 'A':
        return 1;
    case 'B':
        return 2;
    case 'N':
        return 3;
    case 'R':
        return 4;
    case 'C':
        return 5;
    case 'P':
        return 6;
    }
    return -1;
}
const int PieceType[50] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
};

inline int OPP_SIDE(int side){return 1-side;}
inline int SIDE_TAG(int side){return side * 16 + 16;}
inline int PIECE_TYPE(int pc){ return PieceType[pc]; }
inline int PIECE_TYPE_SIDE(int pc) {int pt = PIECE_TYPE(pc); if( pc > 31 ) pt += 7; return pt;}
inline bool AWAY_HALF(int pos, int side) { return  ( pos & 0x10 ) == (side << 7);}
inline int PIECE_SIDE(int pc) {return (pc & 0x10) != 0 ? 0 : 1;}
inline bool SAME_SIDE(int pc1, int pc2){ return ((pc1 ^ pc2) & 0x10) == 0 ? true : false; }
inline int PIECE_INDEX(int pc) {return pc & 0x0f;}

const char FenPieceTable[] = {'K', 'A', 'B', 'N', 'R', 'C', 'P'};

inline QChar ToFenPiece(int pc) {return FenPieceTable[ PIECE_TYPE(pc) ];}



Position::Position() {
    init();
}

void Position::init()
{
    memset(board, 0, sizeof(board));
    memset(pieces, 0, sizeof(pieces));
    memset(bitBoard_row, 0, sizeof(bitBoard_row));
    memset(bitBoard_col, 0, sizeof(bitBoard_col));

    zobri.setKey(0, 0, 0);
    side = 0;
}

int Position::FromFen(const QString &fen)
{
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
            pos = COORD_PC(i, j);
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
    return &PreGen.rookCannon_MoveTable_row[ r ][ bitBoard_row[c] ];
}


int Position::addPiece(int pos, int pc)
{
    int pt;

    board[pos] = pc;
    pieces[pc] = pos;
    bitBoard_row[GET_ROW(pos)] ^= ( 1 << GET_COL(pos) );
    bitBoard_col[GET_COL(pos)] ^= ( 1 << GET_ROW(pos) );

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

    bitBoard_row[GET_ROW(pos)] ^= ( 1 << GET_COL(pos) );
    bitBoard_col[GET_COL(pos)] ^= ( 1 << GET_ROW(pos) );

    pt = PIECE_TYPE_SIDE(pc);
    zobri.Xor(PreGen.zobriTable[pt][pos]);
    return 0;
}

int Position::makeMove(quint32 mv)
{
    Move* move = (Move*)&mv;
    saveStatus();

    movePiece(move);

    if(checkedBy() > 0)
    {
        undoMovePiece(move);
        rollback();
        return -1;
    }

    changeSide();
    trace.last().mv = *move;

    return 0;
}
int Position::undoMakeMove()
{
    if(trace.size() <= 0) return -1;

    const Trace& t = trace.last();

    undoMovePiece(&t.mv);

    side = OPP_SIDE(side);
    rollback();
    return 0;
}


void Position::movePiece(Move *move)
{
    int src = move->o1;
    int dst = move->oe;
    int pcMoved = board[src];
    int pcCaptured = board[dst];

    board[src] = 0;
    board[dst] = pcMoved;
    pieces[pcMoved] = dst;

    bitBoard_row[GET_ROW(src)] ^= ( 1 << GET_COL(src) );
    bitBoard_col[GET_COL(src)] ^= ( 1 << GET_ROW(src) );

    if(pcCaptured != 0)
    {
        pieces[pcCaptured] = 0;
        move->capture = pcCaptured;
        zobri.Xor(PreGen.zobriTable[PIECE_TYPE_SIDE(pcCaptured)][dst]);
    }

    int pt = PIECE_TYPE_SIDE(pcMoved);
    zobri.Xor(PreGen.zobriTable[pt][src]);
    zobri.Xor(PreGen.zobriTable[pt][dst]);
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

    bitBoard_row[GET_ROW(src)] ^= ( 1 << GET_COL(src) );
    bitBoard_col[GET_COL(src)] ^= ( 1 << GET_ROW(src) );

    //pt = PIECE_TYPE_SIDE(pcMoved);
    // zobristKey.Xor(PreGen.zobriTable[pt][dst]);
    // zobristKey.Xor(PreGen.zobriTable[pt][src]);

    if(pcCaptured != 0)
    {
        board[dst] = pcCaptured;
        pieces[pcCaptured] = dst;
        // zobristKey.Xor(PreGen.zobriTable[PIECE_TYPE_SIDE(pcCaptured)][dst]);
    }else
    {
        board[dst] = 0;
        bitBoard_row[GET_ROW(dst)] ^= ( 1 << GET_COL(dst) );
        bitBoard_col[GET_COL(dst)] ^= ( 1 << GET_ROW(dst) );

    }
}

bool Position::legalMove(const Move *mv)
{
    int src = mv->o1;
    int dst = mv->oe;

    int pcMoved = board[src];
    int pcCaptured = board[dst];

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

        if( AWAY_HALF(dst, PIECE_SIDE(pcMoved) ) && ((src == dst - 1) || (src == dst + 1)) )
        {
            return true;
        }
        else return SQUARE_FORWARD(src, PIECE_SIDE(pcMoved) ) == dst;
    }

    }
    return true;
}

void Position::genAllMoves(QVector<quint32> &mvs)
{
    genCapMoves(mvs);
    genNonCapMoves(mvs);
}

void Position::genPieceMoves(QVector<quint32> &mvs, int pos)
{
    switch(PIECE_INDEX(board[pos]) )
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

void Position::saveStatus()
{
    trace.push_back(Trace(this->zobri));
}

void Position::rollback()
{
    zobri = trace.last().zobri;
    trace.removeLast();

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
    if( ( rcmmVer.rookCap & PreGen.PieceMask_row[oppKing] ) != 0 )
    {
        return oppKing;
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

    if( SAME_HALF(dst, sideTag) )
    {
        if( IN_FORT(dst))
        {
            pc = sideTag + KING_FROM;
            if( KING_SPAN( board[pc], dst) )
            {
                return pc;
            }

            for(pc = sideTag + ADVISOR_FROM; pc <= sideTag + ADVISOR_TO; pc++)
            {
                pos = board[pc];
                if( ADVISOR_SPAN( pos, dst ) )
                {
                    return pc;
                }
            }
        }

        for(pc = sideTag + BISHOP_FROM; pc <= sideTag + BISHOP_TO; pc++)
        {
            pos = board[pc];
            if( BISHOP_SPAN( pos, dst ) && board[BISHOP_PIN(pos, dst)] == 0 )
            {
                return pc;
            }
        }
        pos = SQUARE_BACKWARD( dst, side );
        pc = board[pos];
        if( (pc & sideTag ) != 0 && PIECE_INDEX(pc) >= PAWN_FROM )
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
        pos = board[pc];
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
        pos = board[pc];
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
        pos = board[pc];
        int pinpos = KNIGHT_PIN(pos, dst);
        if(pinpos != pos && board[pinpos] == 0)
        {
            return pos;
        }
    }
    return 0;
}


