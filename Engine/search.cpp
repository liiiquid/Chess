#include "search.h"
int Search::go()
{

    QString str = "";

    reset();

    allNode = 0;
    maxDepth = 0;
    pvline[0] = 0;
    pos->distance = 0;
    tt->collideCount = 0;
    tt->recordCount = 0;
    tt->probeCount = 0;

    pos->preEvaluate();

    if(type == Depth)
    {


        qDebug () << "mode: depth, depth:" << _depth;
        qDebug() << "current side:" << ( pos->side == 0 ? "red" : "black");


        int val = 0;

        //val = AlphaBeta(_depth, -MATE_VALUE, MATE_VALUE, pvline);

        for(int i = 1; i <= _depth; i++)
        {
            val = AlphaBeta(i, -MATE_VALUE, MATE_VALUE, pvline);

            // if(val >= WIN_VALUE || val <= -WIN_VALUE)
            // {
            //     qDebug() << "aborting depth:" << i << "val:"<<val;
            //     break;
            // }
        }

        if(val == MATE_VALUE)
        {
            qDebug() << ( pos->side == 0 ? "red win" :"black win");
            printNodeInfor();
            return 0;
        }else if(val == -MATE_VALUE)
        {
            qDebug() << ( pos->side == 0 ? "red resign" :"black resign");
            printNodeInfor();
            return 0x00ffff;
        }

        str = "";
        for(int i = 0; i < _depth && pvline[i] != 0; i++)
        {
            str += ConvertToUcciCoord(pvline[i]) + " ";
        }
        qDebug() << str;
        qDebug() << "collide/total:" << QString("%1/%2").arg(tt->collideCount).arg(tt->recordCount + tt->probeCount);
        qDebug() << "maxDepth:" << maxDepth;
        totalNode += allNode;
        averageNode = totalNode / ( pos->steps / 2 + 1 );
    }
    return pvline[0];
}

void Search::reset()
{
    clearHistory();
    clearKiller();
    tt->clear();

}

void Search::printNodeInfor()
{
    qDebug() << QString("AverageNode: %1, TotalNode:%2, TotalSteps:%3").arg(totalNode / pos->steps).arg(totalNode).arg(pos->steps);
}


void AppendPVLine(int* dst, int mv, int*src)
{
    *dst = mv;
    dst++;
    while( *src != 0 )
    {
        *dst = *src;
        dst++;src++;
    }
    *dst = 0;
}


int Search::AlphaBeta(int depth, int alpha, int beta,int* wpvline)
{

    int val;
    //int vlRep;
    int mv;
    int vlBest;
    int mvBest;
    int mvHash;
    MoveGenerator _mvgen;

    int pvline[MAX_DEPTH];
    pvline[0] = 0;

    // if( ( vlRep = pos->repStatus()) > 0 )
    // {
    //     return pos->drawValue(vlRep);
    // }

    if( depth <= 0 )
    {
        return Quiescence(alpha, beta);
    }

    vlBest = tt->probe(*pos, alpha, beta, depth, mvHash);

    if(vlBest > -MATE_VALUE)
    {
        return vlBest;
    }

    if( mvHash == 0 && depth > 2 )
    {
        val = AlphaBeta( depth / 2, alpha, beta, pvline );
        if(val <= alpha )
        {
            AlphaBeta( depth / 2, -MATE_VALUE, beta, pvline );
        }
        mvHash = pvline[0];
    }

    allNode++;
    vlBest = -MATE_VALUE;
    mvBest = 0;

    _mvgen.initFull(mvHash, &killer[pos->distance][0], history);

    while( (mv = _mvgen.nextFull(*pos)) != 0)
    {
        if( pos->makeMove(mv) == 0)
        {
            if(vlBest == -MATE_VALUE)
            {
                val = -AlphaBeta(depth - 1, -beta, -alpha, pvline);
            }else
            {
                val = -NullWindow(depth - 1, -alpha);
                if(val > alpha && val < beta)
                {
                    val = -AlphaBeta(depth - 1, -beta, -alpha, pvline);    // (1) 较稳定
                    //val = -AlphaBeta(depth - 1, -beta, -val, pvline);      // (2) 不稳定
                }
            }
            pos->undoMakeMove();

            if( val > vlBest )
            {
                vlBest = val;
                if(val >= beta)
                {
                    mvBest = mv;
                    break;
                }
                if(val > alpha)
                {
                    alpha = val;
                    mvBest = mv;
                }
            }
        }
    }

    // 被绝杀
    if(vlBest == -MATE_VALUE)
    {
        Q_ASSERT(pos->bMate());
        return pos->distance - MATE_VALUE;
    }else
    {
        tt->record(*pos, vlBest, mvBest, depth);
        if(mvBest != 0 )
        {
            setHeuristic(mvBest, depth);
        }

        AppendPVLine(wpvline, mvBest, pvline);
        return vlBest;
    }

}

const bool NO_NULL = true;
const int NULL_DEPTH = 2;
int Search::NullWindow(int depth, int beta, bool bNoNull)
{
    int val;
    int vlRep;
    int mv;
    int vlBest;
    int mvHash;
    int mvBest;
    MoveGenerator _mvgen;

    if( ( vlRep = pos->repStatus()) > 0 )
    {
        return pos->drawValue(vlRep);
    }

    if( depth <= 0 )
    {
        Q_ASSERT( depth >= -NULL_DEPTH );
        return Quiescence(beta - 1, beta);
    }

    vlBest = tt->probe(*pos, beta - 1, beta, depth, mvHash);
    // 只检查是否符合剪枝条件, 为保证pvline完整
    if(vlBest > -MATE_VALUE)
    {
        return vlBest;
    }

    if( !bNoNull && pos->nullOkay())
    {
        pos->nullMove();
        val = -NullWindow(depth - 1 - NULL_DEPTH, -beta + 1, NO_NULL);
        pos->undoNullMove();

        if(val >= beta)
        {
            if( pos->nullSafe() )
            {
                tt->record(*pos, val, 0, qMax(depth, NULL_DEPTH));
                return val;
            }else if( NullWindow(depth - NULL_DEPTH, beta, NO_NULL) >= beta )
            {
                tt->record(*pos, val, 0, qMax(depth, NULL_DEPTH));
                return val;
            }
        }
    }

    allNode++;

    vlBest = -MATE_VALUE;
    mvBest = 0;
    _mvgen.initFull(mvHash, &killer[pos->distance][0], history);
    while( (mv = _mvgen.nextFull(*pos)) != 0)
    {
        if( pos->makeMove(mv) == 0 )
        {
            val = -NullWindow(depth - 1, -beta + 1);
            pos->undoMakeMove();

            if( val > vlBest )
            {
                vlBest = val;
                if(val >= beta)
                {
                    mvBest = mv;
                    break;
                }
            }
        }
    }

    // 绝杀
    if(vlBest == -MATE_VALUE)
    {
        Q_ASSERT(pos->bMate());
        return pos->distance - MATE_VALUE;
    }else
    {
        tt->record(*pos, vlBest, mvBest, depth);
        if(mvBest != 0)
        {
            setHeuristic(mvBest, depth);
        }
        return vlBest;
    }
}

int Search::Quiescence(int alpha, int beta)
{
    int val;
    int vlBest;
    int mv;
    bool bCheck;
    int vlRep;
    MoveGenerator _mvgen;

    if( ( vlRep = pos->repStatus()) > 0 )
    {
        return pos->drawValue(vlRep);
    }

    Q_ASSERT(pos->distance <= MAX_DEPTH);
    if(pos->distance > maxDepth)
    {
        maxDepth = pos->distance;
    }
    if(pos->distance == MAX_DEPTH)
    {
        return pos->evaluate(alpha, beta);
    }

    allNode++;

    vlBest = -MATE_VALUE;

    bCheck = pos->lastMove().chkchs > 0 ? true : false;

    if(bCheck)
    {
        _mvgen.getAllMove(*pos, history);
    }else
    {
        val = pos->evaluate(alpha, beta);
        if(val >= beta)
        {
            return val;
        }
        _mvgen.initQuiesc(*pos);
    }

    while( ( mv = _mvgen.nextQuiesc() ) != 0 )
    {
        if( pos->makeMove(mv) == 0 )
        {
            val = -Quiescence( -beta, -alpha);
            pos->undoMakeMove();
            if(val > vlBest )
            {
                vlBest = val;
                if( val >= beta)
                {
                    break;
                }

                if(val > alpha)
                {
                    alpha = val;
                }
            }
        }
    }

    if( bCheck  )
    {
        if( vlBest == -MATE_VALUE)
        {
            Q_ASSERT(pos->bMate());
            return pos->distance - MATE_VALUE;
        }
    }else
    {
        if( vlBest == -MATE_VALUE)
        {
            if( pos->bMate() )
            {
                return pos->distance - MATE_VALUE;
            }
            else
            {
                return pos->evaluate(alpha, beta);
            }
        }
        return vlBest;
    }
    return vlBest;
}

void Search::setHeuristic(int mv, int depth)
{
    int di = pos->distance;
    history[mv] += depth * depth;
    if( mv != killer[di][0] )
    {
        killer[di][1] = killer[di][0];
        killer[di][0] = mv;
    }
}


/**
 *
 *     if( checkBoardRecover(board, pos->board) == false)
    {
        qDebug() << "board unrecover," << QString::number(mv, 16) << ConvertToUcciCoord(mv);
        qDebug() << "current side: " << pos->side << "current depth:" << depth;
        qDebug() << "makecnt:"<<pos->makeCnt << "unmakecnt:"<<pos->unmakeCnt;
        qDebug() << "origin board:";
        printBoard(board);
        qDebug() << "real board:";
        printBoard(pos->board);
        qDebug() << "o";
    }
 */
