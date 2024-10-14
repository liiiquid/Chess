#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <QObject>

class RandomGenerator : public QObject
{
    Q_OBJECT
public:
    explicit RandomGenerator(QObject *parent = nullptr);

    static quint32 rand();
    static void test1();
signals:
};

#endif // RANDOMGENERATOR_H
