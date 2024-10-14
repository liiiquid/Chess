#ifndef SERVERENTRY_H
#define SERVERENTRY_H

#include <QObject>
#include "ucci.h"
#include "position.h"
#include "search.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

class ServerEntry : public QObject
{
    Q_OBJECT
public:
    explicit ServerEntry(QObject *parent = nullptr);
public:
    void initServer();
    void closeServer();
    void updateState();

public:
    void sendToClient(const QString& text, QTcpSocket* socket);
    void readData(QTcpSocket* socket);
public:
    void handleCmd(const QString& cmd);

private:
    bool buildPos(const UcciComm &comm);

private:
    Ucci ucciProcess;
    UcciComm comm;

    Position pos;
    Search search;
    QString res;

    QVector<quint32> coordList;

    QTcpServer* server;
    QVector<QTcpSocket*> clientList;
    QString ip;
    QString port;
signals:
};

#endif // SERVERENTRY_H
