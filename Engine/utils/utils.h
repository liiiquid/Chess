#ifndef UTILS_H
#define UTILS_H
#include <QObject>

inline const quint32 PopCnt(quint32 i)
{
    i = (i & 0x55555555) + ((i >> 1) & 0x55555555) ;
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333) ;
    i = (i & 0x0F0F0F0F) + ((i >> 4) & 0x0F0F0F0F) ;
    i = (i*(0x01010101) >> 24) ;
    return i;
}


#endif // UTILS_H
