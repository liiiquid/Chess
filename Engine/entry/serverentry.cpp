#include "serverentry.h"
#include <QDataStream>
ServerEntry::ServerEntry(QObject *parent)
    : QObject{parent}
{

    pos.init();
    search = new Search(&pos);
    server = new QTcpServer(this);
    ip = "Any";
    port = "9999";
    start = false;
}


void ServerEntry::init()
{

    if(server->isListening()){
        //server->close();
        close();

    }else{
        const QString address_text= ip;
        const QHostAddress address=(address_text=="Any")
                                         ?QHostAddress::Any
                                         :QHostAddress(address_text);
        if(server->listen(address,port.toUShort())){

            emit messageReceived("Server started");
            setStart(true);
        }
    }

    connect(server,&QTcpServer::newConnection,this,[&]{
        while(server->hasPendingConnections())
        {
            QTcpSocket* socket = server->nextPendingConnection();
            TcpSocket *client = new TcpSocket(socket);
            clientList.append(client);
            emit messageReceived(QString("[%1:%2] Soket Connected")
                             .arg(socket->peerAddress().toString())
                             .arg(socket->peerPort()));

            connect(client, &TcpSocket::receivePacket, this, &ServerEntry::handleCmd);
            connect(client, &TcpSocket::disconnected, this, &ServerEntry::onDisonnected);
        }
        updateState();
    });
    connect(server,&QTcpServer::acceptError,[&](QAbstractSocket::SocketError){
        emit messageReceived("Server Error:"+server->errorString());
    });
}

void ServerEntry::close()
{
    server->close();
    for(const TcpSocket* socket:qAsConst(clientList))
    {
        socket->disconnectFromHost();
    }
    setStart(false);
    emit messageReceived("Server stoped");

}

void ServerEntry::print()
{
    qDebug() << "printing...";
    pos.printBoard();
    pos.printPieces();
}

void ServerEntry::printBitBoardRow()
{
    for(int i = 0; i < 10; i++)
    {
        pos.printBitRow(i);
    }
}

void ServerEntry::printBitBoardCol()
{
    for(int i = 0; i < 9; i++)
    {
        pos.printBitCol(i);
    }
}

void ServerEntry::updateState()
{
    if(server->isListening()){
        emit messageReceived(QString("Server[%1:%2] connections:%3")
                         .arg(server->serverAddress().toString())
                         .arg(server->serverPort())
                         .arg(clientList.count()));
    }
}

void ServerEntry::handleCmd(const QString &cmd)
{
    UcciType type = ucciProcess.processCommand(cmd, comm);
    res = "";
    emit messageReceived(cmd);
    switch( type )
    {
    case ucci:
        res = "ucci fighting!!!";
        break;
    case isready:
        res = "isready ready";
        break;
    case position:
        buildPos(comm);
        res = "position ok";
        break;
    case go:
    {
        search->type = comm.gomode;
        search->_depth = comm.depth;
        int mv = search->go();
        if(mv == 0)
        {
            res = "go " + QString("win ") + (pos.side == 0 ? "red" : "black");
        }
        else if(mv == 0xffff)
        {
            res = "go " + QString("resign ") + + (pos.side == 0 ? "red" : "black");
        }
        else
        {
            res = "go " + ConvertToUcciCoord(mv);
        }

        qDebug() << "nodes:"<<search->allNode << res;
        // 作为先手时, steps = 0, 2, 4, ..., 2n
        // 作为后手时, steps = 1, 3, 5, ..., 2n + 1.
        // n从0开始,当n=0时,对应steps=0,此时为AI执行该的第一步, 所以为 (2n / 2) + 1 = (2n + 1 ) / 2 + 1, 整数之除法
        qDebug() << "AverageNode:" << (search->totalNode / (pos.steps / 2 + 1))<<"TotalNode:"<<search->totalNode<<"CurrentCount:"<<(pos.steps / 2 + 1);
        break;
    }
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
        res = "getmv ";
        int pc = pos.board[comm.coordList->at(0)];
        if(pc != 0 )
        {
            res += genPieceMove(pc);
        }else{
            res += "illegal";
        }
        break;
    }
    case makemv:
    {
        res = "makemv ";
        if( pos.makeMove(comm.coordList->at(0)) == 0 )
        {
            res += ConvertToUcciCoord(comm.coordList->at(0));
        }else
        {
            res += "illegal";
        }

    }
    break;
    case selpiece:
    {
        res = "selpiece ";
        int pc = comm.pc;
        if( PIECE_SIDE(pc) == pos.side )
        {
            res += QString::number(pc) + " ";
            res += genPieceMove(pc);

        }else {
            res += "illegal";
        }
    }
        break;
    case score:
    {
        res = "score ";
        res += QString::number(pos.vlRed) + " ";
        res += QString::number(pos.vlBlack);
    }
        break;
    case protect:
    {
        res = "protect ";
        res += pos.protectedBy(comm.checkProtectTurn, ConvertToArrayPoint(*comm.checkProtectPos)) > 0 ? "true" : "false";
    }
        break;
    case undomakemv:
    {
        res = "undomakemv ";
        Move t = pos.lastMove();

        if(pos.undoMakeMove() == 0)
        {

            res += ConvertToUcciCoord(t.mv) + " ";
            if(t.capture > 0)
            {
                res += QString::number(t.capture);
            }else
            {
                res += "0";
            }
            qDebug() << res;
        }else
        {
            res += "illegal";
        }


    }
    case POS_ERROR:
        break;
    }

    TcpSocket* client = dynamic_cast<TcpSocket*>(sender());
    client->sendPacket(res);

}

bool ServerEntry::buildPos(const UcciComm &comm)
{
    bool ret = pos.FromFen(*comm.fen);
    int i;
    pos.side = comm.side;

    if(comm.coordList != nullptr )
    {
        const QVector<quint32>& coord = *comm.coordList;

        for(i = 0; i < coord.size(); i++)
        {
            pos.makeMove(coord[i]);
        }

    }
    return ret;
}

QString ServerEntry::genPieceMove(int pc)
{
    QString ret = "";
    coordList.resize(0);
    this->pos.genPieceMoves( coordList, pos.pieces[pc] );

    for(auto& mv : coordList)
    {
        ret += ConvertToUcciCoord(mv.mv) + " ";
    }
    ret.remove(ret.size() - 1, 1);
    return ret;
}

void ServerEntry::onDisonnected()
{
    TcpSocket* c = dynamic_cast<TcpSocket*>(sender());

    qDebug() << "socket disconnected";
    emit messageReceived(QString("[%1:%2] Soket Disconnected")
                             .arg(c->peerAddress().toString())
                             .arg(c->peerPort()));

    if( clientList.removeOne(c) == false )
    {
        emit messageReceived("clientList remove failed");
    }
    c->deleteLater();
    emit messageReceived(QString("Current Connected Number:%1")
                             .arg(clientList.size()));

}
