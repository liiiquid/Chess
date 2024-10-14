#ifndef ZOBRISTKEY_H
#define ZOBRISTKEY_H
#include <QObject>

class ZobristKey
{
public:
    ZobristKey();
    ZobristKey(quint32 priKey, quint32 secKey_0, quint32 secKey_1);
    void setKey(quint32 priKey, quint32 secKey_0, quint32 secKey_1);

public:
    void Xor(const ZobristKey& key);

private:
    quint32 _priKey;
    quint32 _secKey_0, _secKey_1;
};

#endif // ZOBRISTKEY_H
