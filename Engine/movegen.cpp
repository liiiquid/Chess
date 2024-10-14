#include "movegen.h"
#include "entry/ucci.h"
#include "x86asm.h"
int MoveGenerator::nextMove()
{
    if( currentMoveIndex < size )
    {
        return mvs[currentMoveIndex++].mv;
    }else
    {
        return 0;
    }
}

void MoveGenerator::initFull(int hashMv, int* pkiller, int* history)
{
    mvHash = hashMv;
    phase = PhaseHash;
    killer0 = pkiller[0];
    killer1 = pkiller[1];
    phistory = history;
}

int MoveGenerator::nextFull(Position& pos)
{
    int mv;
    switch(phase)
    {
    case PhaseHash:
    {
        phase = PhaseGenCap;

        if(mvHash != 0)
        {
            return mvHash;
        }
    }
    case PhaseGenCap:
    {
        currentMoveIndex = 0;
        size = pos.genCapMoves(mvs);
        ShellSort();
        phase = PhaseCapture;
    }
    case PhaseCapture:
    {
        mv = nextMove();
        if(mv != 0)
        {
            return mv;
        }else
        {
            phase = PhaseKiller0;
        }
    }
    case PhaseKiller0:
    {
        phase = PhaseKiller1;
        if( killer0 != 0 && pos.legalMove(killer0))
        {
            return killer0;
        }
    }
    case PhaseKiller1:
    {
        phase = PhaseGenNonCap;
        if( killer1 != 0 && pos.legalMove(killer1))
        {
            return killer1;
        }
    }
    case PhaseGenNonCap:
    {
        currentMoveIndex = 0;
        size = pos.genNonCapMoves(mvs);
        setHistory();
        ShellSort();
        phase = PhaseNonCap;
    }
    case PhaseNonCap:
    {
        return nextMove();
    }
    }
    return 0;
}

void MoveGenerator::initQuiesc(Position &pos)
{
    currentMoveIndex = 0;
    size = pos.genCapMoves(mvs);
    ShellSort();
}

int MoveGenerator::nextQuiesc()
{
    return nextMove();
}

void MoveGenerator::ShellSort()
{
    int steps[] = {1, 5, 19, 41, 109};
    int stepLevel = sizeof(steps) / sizeof(int) - 1;
    int i, j;
    int k;
    int step;
    Move t;
    for( k = stepLevel; k >= 0; k-- )
    {
        step = steps[k];
        for( i = step; i < size; i += step )
        {
            t = mvs[i];
            for(j = i - step; j > -1 && t.mvlva > mvs[j].mvlva; j -= step )
            {
                mvs[j + step] = mvs[j];
            }
            mvs[j + step] = t;
        }
    }
}

void MoveGenerator::setHistory()
{
    int i;
    int j;
    int shift = 0;
    int newshift;
    int vl;
    for(i = 0; i < size; i++)
    {
        vl = phistory[mvs[i].mv] >> shift;
        if( vl > 65535 )
        {
            newshift = Bsr(vl) - 15;
            for(j = 0; j < i; j++)
            {
                mvs[j].val >>= newshift;
            }
            vl >>= newshift;
            Q_ASSERT( 32767 < vl && vl < 65536 );
            shift += newshift;
        }
        mvs[i].val = vl;
    }
}

void MoveGenerator::getAllMove(Position &pos, int* history)
{
    currentMoveIndex = 0;
    size = pos.genAllMoves(mvs);
    phistory = history;
    setHistory();
    ShellSort();
    // for(int i = 0; i < size; i++)
    // {
    //     Move mv = mvs[i];
    //     qDebug() << ConvertToUcciCoord(mv.t) << mv.mvlva;
    // }
}
