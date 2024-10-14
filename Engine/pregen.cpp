#include "pregen.h"
#include "qdebug.h"
#include "utils/randomgenerator.h"

const quint8 BoardMask[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const quint8 FortMask[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const quint8 LEGAL_SPAN_TAB[512] = {
                            0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
const qint8 KNIGHT_PIN_TAB[512] = {
    0, 0, 0, 0, 0, 0, 0,    0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    -0x10,0, -0x10,0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -0x01,0,    0, 0,    0x01,0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -0x01,0,    0, 0,    0x01,0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0x10, 0, 0x10, 0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,    0, 0,    0,   0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,    0,
};
const qint8 KnightDisplace[8] = {-0x21, -0x12, -0x1f, -0x0e, 0x21, 0x0e,  0x1f,  0x12};
// const qint8 KnightPin[8] = {     -0x10, -0x01, -0x10, 0x01,  0x10, -0x01, 0x10,  0x01};
                            //5     6       7     8      9    x       z      y
const qint8 BishopDisplace[8] = {-0x22, -0x1e, 0x1e, 0x22};
// const qint8 BishopPin[8] = {-0x11, -0xf, 0xf, 0x11};

const qint8 AdvisorDisplace[8] = {-0x11, -0xf, 0xf, 0x11};
const qint8 KingDisplace[8] = {-0x10, -0x01, 0x01, 0x10};

class PreGen PreGen;

PreGen::PreGen()
{
    init();
}

void PreGen::init()
{
    int i, j, k;
    RandomGenerator r;
    //  initialize zobrist random value table.
    for(i = 0; i < 14; i++)
    {
        for(j = 0; j < 256; j++)
        {
            zobriTable[i][j].setKey(r.rand(), r.rand(), r.rand());
        }
    }
    zobriPlayer.setKey(r.rand(), r.rand(), r.rand());
    qDebug() << "zobristkey initialize ok";

    for(i = 0; i < 256; i++)
    {
        if( IN_BOARD(i) )
        {
            PieceMask_row[i] = 1 << ( GET_REL_ROW(i) );
            PieceMask_col[i] = 1 << ( GET_REL_COL(i) );
        }
        else{
            PieceMask_row[i] = 0;
            PieceMask_col[i] = 0;
        }
    }

    QString str = "";
    for(i = 0; i < 16; i++)
    {
        for(j = 0; j < 16; j++)
        {
            str += QString("%1 ").arg(PieceMask_row[i * 16 + j], 5, 10, QLatin1Char('0'));
        }
        qDebug() << str;
        str = "";
    }
    qDebug() << "PieceMask initialize ok";

    // "row generation"
    // piece-bit corresponding relation:
    // bit: 8 7 6 5 4 3 2 1 0
    // col: 8 7 6 5 4 3 2 1 0
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 512; j++)
        {
            RookCannon_Move rcm = {{ TO_COL(i), TO_COL(i) }, {TO_COL(i), TO_COL(i)}, {TO_COL(i), TO_COL(i)}, {TO_COL(i), TO_COL(i)}};
            RookCannon_MoveMask rcmm = { 0};
            //collect left region
            for(k = i - 1; k >= 0; k-- )
            {
                if( ((1 << k) & j) != 0)
                {
                    rcm.rookCap[0] = TO_COL(k);
                    rcmm.rookCap |= 1 << k;
                    k--;
                    break;
                }
                rcm.nonCap[0] = TO_COL(k);
                rcmm.nonCap |= 1 << k;
            }

            for(; k >= 0; k--)
            {
                if( ( (1 << k) & j ) != 0 )
                {
                    rcm.cannonCap[0] = TO_COL(k);
                    rcmm.cannonCap |= 1 << k;
                    k--;
                    break;
                }
                rcmm.cannonCap |= 1 << k;
            }

            for(; k >= 0; k--)
            {
                if( ( (1 << k) & j ) != 0 )
                {
                    rcmm.superCannonCap |= 1 << k;
                    rcm.superCannonCap[0] = TO_COL(k);
                    k--;
                    break;
                }

            }

            //collect right region
            for(k = i + 1; k < 9; k++)
            {
                if( ((1 << k) & j) != 0)
                {
                    rcm.rookCap[1] = TO_COL(k);
                    rcmm.rookCap |= 1 << k;
                    k++;
                    break;
                }
                rcm.nonCap[1] = TO_COL(k);
                rcmm.nonCap |= 1 << k;
            }

            for(; k < 9; k++)
            {
                if( ( (1 << k) & j ) != 0 )
                {
                    rcm.cannonCap[1] = TO_COL(k);
                    rcmm.cannonCap |= 1 << k;
                    k++;
                    break;
                }
                rcmm.cannonCap |= 1 << k;
            }

            for(; k < 9; k++)
            {
                if( ( (1 << k) & j ) != 0 )
                {
                    rcmm.superCannonCap |= 1 << k;
                    rcm.superCannonCap[1] = TO_COL(k);
                    break;
                }

            }


            rookCannon_MoveTable_row[i][j] = rcm;
            rookCannon_MoveMaskTable_row[i][j] = rcmm;
        }
    }
    qDebug() << "row generation table initialize ok";
    // "col generation"
    // piece-bit corresponding relation:
    // bit: 9 8 7 6 5 4 3 2 1 0
    // col: 9 8 7 6 5 4 3 2 1 0
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 1024; j++)
        {
            RookCannon_Move rcm = {{TO_ROW(i), TO_ROW(i)}, {TO_ROW(i), TO_ROW(i)}, {TO_ROW(i), TO_ROW(i)}, {TO_ROW(i), TO_ROW(i)}};
            RookCannon_MoveMask rcmm = { 0};
            for(k = i - 1; k >= 0; k-- )
            {
                if( ((1 << k) & j) != 0)
                {
                    rcm.rookCap[0] = TO_ROW(k);
                    rcmm.rookCap |= 1 << k;
                    k--;
                    break;
                }
                rcm.nonCap[0] = TO_ROW(k);
                rcmm.nonCap |= 1 << k;
            }

            for(; k >= 0; k--)
            {
                if( ( (1 << k) & j ) != 0 )
                {
                    rcm.cannonCap[0] = TO_ROW(k);
                    rcmm.cannonCap |= 1 << k;
                    k--;
                    break;
                }
                rcmm.cannonCap |= 1 << k;
            }

            for(; k >= 0; k--)
            {
                if( ( (1 << k) & j ) != 0 )
                {
                    rcmm.superCannonCap |= 1 << k;
                    rcm.superCannonCap[0] = TO_ROW(k);
                    break;
                }
            }

            for(k = i + 1; k < 10; k++)
            {
                if( ((1 << k) & j) != 0)
                {
                    rcm.rookCap[1] = TO_ROW(k);
                    rcmm.rookCap |= 1 << k;
                    k++;
                    break;
                }
                rcm.nonCap[1] = TO_ROW(k);
                rcmm.nonCap |= 1 << k;
            }


            for(; k < 10; k++)
            {
                if( ( (1 << k) & j ) != 0 )
                {
                    rcm.cannonCap[1] = TO_ROW(k);
                    rcmm.cannonCap |= 1 << k;
                    k++;
                    break;
                }
                rcmm.cannonCap |= 1 << k;
            }

            for(; k < 10; k++)
            {
                if( ( (1 << k) & j ) != 0 )
                {
                    rcmm.superCannonCap |= 1 << k;
                    rcm.superCannonCap[1] = TO_ROW(k);
                    break;
                }
            }

            rookCannon_MoveTable_col[i][j] = rcm;
            rookCannon_MoveMaskTable_col[i][j] = rcmm;
        }
    }

    qDebug() << "col generation table initialize ok";

    for(i = 0; i < 256; i++)
    {
        if(IN_BOARD(i) == true)
        {
            int len = 0;
            Knight_Move km = {{0}, {0}};
            for(j = 0; j < 8; j++)
            {
                k = i + KnightDisplace[j];
                if( IN_BOARD(k) == true )
                {
                    km.tar[len] = k;
                    km.pin[len] = KNIGHT_PIN(i, k);
                    len++;
                }
            }
            km.tar[len] = 0;
            knight_MoveTable[i] = km;

            len = 0;
            Bishop_Move bm;
            for(j = 0; j < 4; j++)
            {
                k = i + BishopDisplace[j];
                if( IN_BOARD(k) == true && SAME_HALF(i, k) == true)
                {
                    bm.tar[len] = k;
                    bm.pin[len] = BISHOP_PIN(i, k);
                    len++;
                }
            }
            bm.tar[len] = 0;
            bishop_MoveTable[i] = bm;

            len = 0;
            for(j = 0; j < 4; j++)
            {
                k = i + AdvisorDisplace[j];
                if( IN_FORT(k) == true )
                {
                    advisor_MoveTable[i][len] = k;
                    len++;
                }
            }
            advisor_MoveTable[i][len] = 0;

            len = 0;
            for(j = 0; j < 4; j++)
            {
                k = i + KingDisplace[j];
                if( IN_FORT(k) == true )
                {
                    king_MoveTable[i][len] = k;
                    len++;
                }
            }
            king_MoveTable[i][len] = 0;

            for(j = 0; j < 2; j++)
            {
                len = 0;
                if( IN_BOARD( (k = SQUARE_FORWARD(i, j)) ) == true )
                {
                    pawn_MoveTable[i][j][len++] = k;
                }

                if( SAME_HALF(i, ( (1 - j) << 7) ) == false )
                {
                    k = i - 0x01;
                    if ( IN_BOARD( k ) == true )
                    {
                        pawn_MoveTable[i][j][len++] = k;
                    }
                    k = i + 0x01;
                    if ( IN_BOARD( k ) == true )
                    {
                        pawn_MoveTable[i][j][len++] = k;
                    }
                }
                pawn_MoveTable[i][j][len] = 0;
            }

        }
    }

    qDebug() << "all generation table initialize ok";


}
