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
    //创建Server对象

    if(server->isListening()){
        //server->close();
        closeServer();
        //关闭server后恢复界面状态
        qDebug() <<("Server stoped");

    }else{
        //从界面上读取ip和端口
        //可以使用 QHostAddress::Any 监听所有地址的对应端口
        const QString address_text= ip;
        const QHostAddress address=(address_text=="Any")
                                         ?QHostAddress::Any
                                         :QHostAddress(address_text);
        //开始监听，并判断是否成功
        if(server->listen(address,port.toUShort())){
            //连接成功就修改界面按钮提示，以及地址栏不可编辑
            qDebug() << ("Server started");
        }
    }

    //监听到新的客户端连接请求
    connect(server,&QTcpServer::newConnection,this,[this]{
        //如果有新的连接就取出
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

            //连接断开，销毁socket对象，这是为了开关server时socket正确释放
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
    //停止服务
    server->close();
    for(QTcpSocket * socket:qAsConst(clientList))
    {
        //断开与客户端的连接
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
    //判断是否开启了server
    if(!server->isListening())
        return;
    //将发送区文本发送给客户端
    const QByteArray& send_data=text.toUtf8();
    qint64 packetlen = send_data.length();
    //数据为空就返回
    if(send_data.isEmpty())
        return;
    socket->write((const char*)&packetlen);
    socket->write(send_data);
}

void ServerEntry::readData(QTcpSocket* socket)
{
    static QByteArray dataBuffer = "";
    //socket缓冲区中没有数据直接返回
    if(socket->bytesAvailable() <=0)
        return;
    QByteArray buffer = socket->readAll();
    dataBuffer.append(buffer);   //读取数据放入缓冲区

    qint64 total_bytes;

    int total_length = dataBuffer.size();

    while(total_length)
    {
        if(total_length < sizeof(qint64))     //不够包头数据长度，等下次解析
            break;
        total_bytes = *(const qint64*)dataBuffer.left(sizeof(qint64)).data();
        if(total_length < total_bytes + sizeof(qint64))   //不够数据长度
            break;
        QString instruct = QString::fromUtf8(dataBuffer.remove( 0, total_length + sizeof(qint64)));
        qDebug() << "recv: " << instruct;
        handleCmd(instruct);

        //缓存多余的数据
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
