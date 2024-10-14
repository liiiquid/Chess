#ifndef SERVEENTRY_H
#define SERVEENTRY_H
#include "ucci.h"
#include "position.h"
#include "search.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

class ServeEntry : public QObject
{
    Q_OBJECT
public:
    ServeEntry();

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
};

#endif // SERVEENTRY_H
