#ifndef HASH_H
#define HASH_H
#include "zobristkey.h"
#include "position.h"

typedef struct
{
    quint32 _secKey_0;
    quint32 _secKey_1;
    qint16 val;
    quint16 mv;
    qint16 depth;
}HashNode;

class Hash
{
public:
    Hash(int size);

public:
    void create(int size);
    void clear();

public:
    void record(Position& pos, int val, int mv, int depth);
    int probe( Position& pos, int alpha, int beta, int depth, int &mv);
public:
    int lengthMask;
    HashNode* _nodes;
public:
    int collideCount;
    int recordCount;
    int probeCount;
};

#endif // HASH_H
