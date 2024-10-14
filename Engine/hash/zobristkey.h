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

    bool operator==(const ZobristKey& key)
    {
        return key._priKey == _priKey && key._secKey_0 == _secKey_0 && key._secKey_1 == _secKey_1;
    }
public:
    QString toString()
    {
        return QString("priKey:%1,_secKey_0:%2,_secKey_1:%3")
            .arg(_priKey, 8,16, QChar('0'))
            .arg(_secKey_0, 8, 16, QChar('0'))
            .arg(_secKey_1, 8, 16, QChar('0'));
    }
public:
    quint32 _priKey;
    quint32 _secKey_0, _secKey_1;
};

#endif // ZOBRISTKEY_H
