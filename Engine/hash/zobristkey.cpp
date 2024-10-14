#include "zobristkey.h"

ZobristKey::ZobristKey()
{
    setKey(0, 0, 0);
}

ZobristKey::ZobristKey(quint32 priKey, quint32 secKey_0, quint32 secKey_1) {
    setKey(priKey, secKey_0, secKey_1);
}

void ZobristKey::setKey(quint32 priKey, quint32 secKey_0, quint32 secKey_1)
{
    _priKey = priKey;
    _secKey_0 =secKey_0;
    _secKey_1 = secKey_1;
}

void ZobristKey::Xor(const ZobristKey &key)
{
    _priKey ^= key._priKey;
    _secKey_0 ^= key._secKey_0;
    _secKey_1 ^= key._secKey_1;
}
