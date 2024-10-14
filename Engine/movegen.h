#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "position.h"

typedef enum
{
    PhaseHash = 0,
    PhaseGenCap = 1,
    PhaseCapture,
    PhaseKiller0,
    PhaseKiller1,
    PhaseGenNonCap,
    PhaseNonCap,
}Phase;

class MoveGenerator
{

public:
    MoveGenerator() {
        phase = PhaseCapture;

    }

    void initFull(int mvHash, int* pkiller,int* history);
    int nextFull(Position& pos);

    void getAllMove(Position &pos, int* history);
    void initQuiesc(Position& pos);
    int nextQuiesc();

private:
    void ShellSort();
    int nextMove();

    void setHistory();
public:
    Phase phase;

    int* phistory;
public:
    int mvHash;
    int killer0;
    int killer1;
public:
    Move mvs[120];
    int currentMoveIndex;
    int size;

public:
    int mvBest;
};

#endif // MOVEGEN_H
