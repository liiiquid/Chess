#ifndef UCCI_H
#define UCCI_H

#include <QString>
#include <QStringList>
#include "pregen.h"
#include "qdebug.h"
typedef enum{
    ucci = 0,
    isready,
    position,
    go,
    stop,
    quit,
    getpos,
    getmv,
    makemv,
    selpiece,
    score,
    protect,
    undomakemv,
    POS_ERROR,
}UcciType;

typedef enum{
    Depth,
    Node,
    Time,
}GoMode;

typedef union
{
    struct{
        const QString* fen;
        const QVector<quint32>* coordList;
        int side;
    };

    struct{
        int pc;
    };

    struct{
        GoMode gomode;
        union{
            int depth; int maxNode; int limitTime;
        };
    };

    struct{
        int checkProtectTurn;
        const QString* checkProtectPos;
    };

}UcciComm;
inline const quint32 ConvertToArrayCoord(const QString &mv)
{
    quint16 st = 0, ed = 0;
    st = COORD_PC( 9 - mv[1].toLower().unicode() + '0', mv[0].toLower().unicode() - 'a');
    ed = COORD_PC( 9 - mv[3].toLower().unicode() + '0', mv[2].toLower().unicode() - 'a');
    return (ed << 8) + st;
}

inline const QString ConvertToUcciCoord(int mv)
{
    QString ret = "";
    int src = mv & 0xff;
    int dst = (mv & 0xff00) >> 8;

    ret += QChar(GET_REL_COL(src) + 'a');
    ret += QChar( 9 - GET_REL_ROW(src) + '0' );
    ret += QChar(GET_REL_COL(dst) + 'a');
    ret += QChar( 9 - GET_REL_ROW(dst) + '0');
    return ret;
}
inline const QString ConvertToUcciPoint(int cr)
{
    QString ret = "";
    int src = cr & 0xff;
    ret += QChar(GET_REL_COL(src) + 'a');
    ret += QChar( 9 - GET_REL_ROW(src) + '0' );
    return ret;
}

inline const int ConvertToArrayPoint(const QString& cr)
{
    quint16 st = 0;
    st = COORD_PC( 9 - cr[1].toLower().unicode() + '0', cr[0].toLower().unicode() - 'a');
    return st;
}

class Ucci
{

public:

public:
    Ucci();

public:
    UcciType processCommand(const QString& command, UcciComm& comm);

private:

    QString charBuffer;
    QVector<quint32> coordList;

};
#endif // UCCI_H
