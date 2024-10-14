#include "hash.h"
#include "search.h"
Hash::Hash(int size) {

    Q_ASSERT(size < 32 );
    lengthMask = 0;
    _nodes = nullptr;
    create(size);
    clear();

}

void Hash::create(int size)
{
    if(lengthMask != 0){
        return;
    }
    if(size < 32 )
    {
        lengthMask = ( 1 << size ) - 1;
        _nodes = new HashNode[lengthMask + 1];
        clear();
    }

}

void Hash::clear()
{
    if(_nodes != nullptr)
    {
        memset(_nodes, 0, (lengthMask + 1) * sizeof(HashNode));
    }else
    {
        qDebug() << "clear(): _nodes == nullptr";
    }
    collideCount = 0;
}

void Hash::record( Position &pos, int val, int mv, int depth)
{
    HashNode& node = _nodes[pos.zobri._priKey & lengthMask];
    recordCount += 1;
    if(node._secKey_0 == pos.zobri._secKey_0 && node._secKey_1 == pos.zobri._secKey_1)
    {
        // 替换搜索深度少的结点
        if( node.depth < depth )
        {
            node.mv = mv;
            node.val = val;
            node.depth = depth;
        }

    }else
    {
        if(node.depth == 0)
        {
            node._secKey_0 = pos.zobri._secKey_0;
            node._secKey_1 = pos.zobri._secKey_1;
            node.depth = depth;
            node.val = val;
            node.mv = mv;
        }else
        {
            node._secKey_0 = pos.zobri._secKey_0;
            node._secKey_1 = pos.zobri._secKey_1;
            node.depth = depth;
            node.val = val;
            node.mv = mv;
            collideCount += 1;
        }
    }

}

int Hash::probe( Position &pos, int alpha, int beta, int depth, int& mv)
{
    const HashNode& node = _nodes[pos.zobri._priKey & lengthMask];
    probeCount += 1;
    mv = 0;

    if(node._secKey_0 == pos.zobri._secKey_0 && node._secKey_1 == pos.zobri._secKey_1)
    {
        mv = node.mv;
        Q_ASSERT(mv == 0 || pos.legalMove(mv));
        // 只有当哈希表结点深度大于等于被探测结点时才有效.
        if( node.depth >= depth )
        {
            if( node.val <= alpha || node.val >= beta)
            {
                Q_ASSERT(-MATE_VALUE <= node.val && node.val <= MATE_VALUE);
                return node.val;
            }
        }

    }else
    {
        if(node.depth != 0)
        {
            collideCount += 1;
        }
    }

    return -MATE_VALUE;
}
