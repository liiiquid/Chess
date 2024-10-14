#include "serverentry.h"
#include <QDataStream>
ServerEntry::ServerEntry(QObject *parent)
    : QObject{parent}
{

    pos.init();
    server = new QTcpServer(this);
}


void ServerEntry::initServer()
{

    if(server->isListening()){
        //server->close();
        closeServer();
        qDebug() <<("Server stoped");

    }else{
        const QString address_text= ip;
        const QHostAddress address=(address_text=="Any")
                                         ?QHostAddress::Any
                                         :QHostAddress(address_text);
        if(server->listen(address,port.toUShort())){

            qDebug() << ("Server started");
        }
    }

    connect(server,&QTcpServer::newConnection,this,[this]{
        while(server->hasPendingConnections())
        {
            QTcpSocket *socket=server->nextPendingConnection();
            clientList.append(socket);
            qDebug() << (QString("[%1:%2] Soket Connected")
                             .arg(socket->peerAddress().toString())
                             .arg(socket->peerPort()));

            connect(socket,&QTcpSocket::readyRead,[&]{
                QTcpSocket* client = dynamic_cast<QTcpSocket*>(sender());
                readData(client);
            });

            connect(socket,&QAbstractSocket::errorOccurred,[socket](QAbstractSocket::SocketError){
                qDebug () << (QString("[%1:%2] Soket Error:%3")
                                 .arg(socket->peerAddress().toString())
                                 .arg(socket->peerPort())
                                 .arg(socket->errorString()));
            });

            connect(socket,&QTcpSocket::disconnected,[this,socket]{
                socket->deleteLater();
                clientList.removeOne(socket);
                qDebug () << (QString("[%1:%2] Soket Disonnected")
                                 .arg(socket->peerAddress().toString())
                                 .arg(socket->peerPort()));
                updateState();
            });
        }
        updateState();
    });
    connect(server,&QTcpServer::acceptError,[&](QAbstractSocket::SocketError){
        qDebug() << ("Server Error:"+server->errorString());
    });
}

void ServerEntry::closeServer()
{
    server->close();
    for(QTcpSocket * socket:qAsConst(clientList))
    {
        socket->disconnectFromHost();
        if(socket->state()!=QAbstractSocket::UnconnectedState){
            socket->abort();
        }
    }
}

void ServerEntry::updateState()
{
    if(server->isListening()){
        qDebug() << (QString("Server[%1:%2] connections:%3")
                         .arg(server->serverAddress().toString())
                         .arg(server->serverPort())
                         .arg(clientList.count()));
    }
}

void ServerEntry::sendToClient(const QString& text, QTcpSocket* socket)
{
    if(!server->isListening())
        return;
    const QByteArray& send_data=text.toUtf8();
    qint64 packetlen = send_data.length();
    if(send_data.isEmpty())
        return;
    socket->write((const char*)&packetlen);
    socket->write(send_data);
}

void ServerEntry::readData(QTcpSocket* socket)
{
    static QByteArray dataBuffer = "";
    if(socket->bytesAvailable() <=0)
        return;
    QByteArray buffer = socket->readAll();
    dataBuffer.append(buffer);

    qint64 total_bytes;

    int total_length = dataBuffer.size();

    while(total_length)
    {
        if(total_length < sizeof(qint64))
            break;
        total_bytes = *(const qint64*)dataBuffer.left(sizeof(qint64)).data();
        if(total_length < total_bytes + sizeof(qint64))
            break;
        QString instruct = QString::fromUtf8(dataBuffer.remove( 0, total_length + sizeof(qint64)));
        qDebug() << "recv: " << instruct;
        handleCmd(instruct);

        total_length = buffer.size();
    }
}

void ServerEntry::handleCmd(const QString &cmd)
{
    UcciType type = ucciProcess.processCommand(cmd, comm);
    res = "";
    switch( type )
    {
    case ucci:
        res = "ucci ";
        res += "fighting!!!";
        break;
    case isready:
        res = "isready ";
        res += "ready";
        break;
    case position:
        buildPos(comm);
        break;
    case go:
        break;
    case stop:
        break;
    case quit:
        res = "quit";
        break;
    case getpos:
        res = "getpos ";
        res += pos.toFen();
        break;
    case getmv:
    {
        coordList.resize(0);
        this->pos.genPieceMoves( coordList, comm.coordList->at(0) );
        res = "getmv ";
        for(auto mv : qAsConst(coordList))
        {
            res += ConvertToUcciCoord(mv) + ' ';
        }
        break;
    }
    case makemv:
    {
        pos.makeMove(comm.coordList->at(0));
        res = "makemv ";
        res += "ok";
    }
    break;
    case POS_ERROR:
        break;
    }
}

bool ServerEntry::buildPos(const UcciComm &comm)
{
    bool ret = pos.FromFen(*comm.fen);
    const QVector<quint32>& coord = *comm.coordList;
    int i = 0;
    for(i = 0; i < coord.size(); i++)
    {
        pos.makeMove(coord[i]);
    }
    return ret;
}
