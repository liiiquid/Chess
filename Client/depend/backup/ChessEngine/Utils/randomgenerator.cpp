#include "randomgenerator.h"
#include "qdebug.h"
#include <QRandomGenerator64>
RandomGenerator::RandomGenerator(QObject *parent)
    : QObject{parent}
{


}

quint32 RandomGenerator::rand()
{
    return QRandomGenerator64::global()->bounded((quint32)1 << 31);
}

void RandomGenerator::test1()
{
    qDebug() << "starting";
    quint64 sum = 0;
    int totalcnt = 100;
    int cnt = totalcnt;
    while( cnt-- > 0 )
    {
        sum += rand();
    }

    qDebug() << sum / totalcnt;
    qDebug() << "over";
}
