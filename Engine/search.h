#ifndef SEARCH_H
#define SEARCH_H
#include "position.h"
#include "movegen.h"
#include "entry/ucci.h"
#include "hash/hash.h"
const int MATE_VALUE = 10000;
const int WIN_VALUE = 9900;
const int MAX_DEPTH = 64;
class Search
{

public:
    Search(Position* p) {
        this->pos = p;
        tt = new Hash(27);
        reset();
        totalNode = 0;
        averageNode = 0;
    }

public:
    int go();
    void reset();

private:

    int AlphaBeta(int depth, int alpha, int beta, int* wpvline);
    int NullWindow(int depth, int beta, bool bNoNull = false);
    int Quiescence(int alpha, int beta);


private:
    void setHeuristic(int mv, int depth);
    void clearKiller(){
        memset(killer, 0, MAX_DEPTH * sizeof(int[2]));
    }
    void clearHistory(){
        memset(history, 0, sizeof(history));
    }

private:
    void CheckTimeOut();

public:
    Position* pos;

public:
    Hash* tt;
    int killer[MAX_DEPTH][2];
    int history[65536];

public:
    int allNode;
    int pvline[MAX_DEPTH];
    int maxDepth;

public:
    int totalNode;
    int averageNode;
public:
    GoMode type;
    union{
        int _depth; int _limitTime; int _maxNode;
    };

private:
    qint64 _maxLimitTime;
    qint64 _beginTime;
};

#endif // SEARCH_H
