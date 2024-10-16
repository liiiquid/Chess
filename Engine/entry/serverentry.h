#ifndef SERVERENTRY_H
#define SERVERENTRY_H

#include <QObject>
#include "ucci.h"
#include "position.h"
#include "search.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QQmlEngine>
#include "../Client/tcpsocket.h"

class ServerEntry : public QObject
{
    Q_OBJECT
    Q_PROPERTY (bool start READ getStart WRITE setStart NOTIFY startChanged)
public:
    explicit ServerEntry(QObject *parent = nullptr);

public:
    static ServerEntry* instance()
    {
        static bool _init = false;
        static ServerEntry * ret = nullptr;
        if(_init == false )
        {
            ret = new ServerEntry();
            _init = true;
        }
        return ret;
    }
public:
    Q_INVOKABLE void init();
    Q_INVOKABLE void close();
    Q_INVOKABLE void print();
    Q_INVOKABLE void printBitBoardRow();
    Q_INVOKABLE void printBitBoardCol();
    Q_INVOKABLE void printBitPiece() {pos.printBitPiece();}
    Q_INVOKABLE void printMaterial() {pos.preEvaluate();qDebug() << pos.material();}
    Q_INVOKABLE void printAp(){pos.preEvaluate();qDebug() << pos.advisorShape();}
    Q_INVOKABLE void printSh(){pos.preEvaluate();qDebug() << pos.stringHold();}
    Q_INVOKABLE void printKT(){pos.preEvaluate();qDebug() << pos.knightTrap();}
    Q_INVOKABLE void printRm(){pos.preEvaluate();qDebug() << pos.rookMobility();}
    void updateState();
public slots:
    void handleCmd(const QString& cmd);

private:
    bool buildPos(const UcciComm &comm);

    QString genPieceMove(int pc);

private:
    Ucci ucciProcess;
    UcciComm comm;

    Position pos;
    Search* search;
    QString res;

    QVector<Move> coordList;

    QTcpServer* server;
    QVector<TcpSocket*> clientList;
    QString ip;
    QString port;

public:
    bool getStart() {return start;}
    void setStart(bool state) { start = state; emit startChanged(start); }
public slots:
    void onDisonnected();

private:
    bool start;

signals:
    void startChanged(bool _start);
    void messageReceived(const QString& str);
};

#endif // SERVERENTRY_H
