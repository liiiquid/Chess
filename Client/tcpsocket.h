#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

typedef enum{
    Connected = 0,
    Unconnected,
    Reconnecting,
}SocketState;
inline const QString socketStateToString(SocketState state) {
    switch (state) {
    case Connected:   return QStringLiteral("Connected");
    case Unconnected: return QStringLiteral("Unconnected");
    case Reconnecting:  return QStringLiteral("Reconnecting");
    default:    return QStringLiteral("Unknown");
    }
}
class TcpSocket : public QObject{
    Q_OBJECT

    SocketState _state;
public:

    SocketState state(){return _state;}
    void setState(SocketState state) {

        _state = state;
        if(_state == Connected)
        {
            emit connected();
        }else if(_state == Unconnected)
        {
            emit disconnected();
        }else if(_state == Reconnecting)
        {
            emit reconnecting();
        }
    }
    TcpSocket(const QString& ip, const QString& port, QObject* parent = nullptr)
        : QObject(parent)
    {
        _ip = ip;
        _port = port;
        _socket = new QTcpSocket(this);
        _socket->connectToHost(QHostAddress(_ip), _port.toUShort());
        setState(Unconnected);
        init();
    }

    TcpSocket(QTcpSocket* socket, QObject* parent = nullptr)
        : QObject(parent)
    {
        _ip = _port = "";
        if( socket->state() != QAbstractSocket::ConnectedState )
        {
            qDebug() << "TcpSocket(QTcpSocket* socket):socket is not in connected state.";
            _socket = nullptr;
            setState(Unconnected);
            return;
        }
        _socket = socket;
        _socket->setParent(this);
        setState(Connected);
        init();

    }
public:

    QTimer* _reconnectTimer;
    QString errorString = "";
    void init()
    {

        qDebug() << "TcpSocket is initializing";
        _reconnectTimer = new QTimer(this);
        connect(_reconnectTimer, &QTimer::timeout, [&](){
            if(_ip != "" && _port != "")
                _socket->connectToHost(QHostAddress(_ip), _port.toUShort());
        });

        connect(_socket, &QTcpSocket::connected, this, [&](){
            qDebug () << (QString("[%1:%2] Soket Connected...")
                             .arg(_socket->peerAddress().toString())
                             .arg(_socket->peerPort()));
            if(_reconnectTimer->isActive() )
            {
                _reconnectTimer->stop();
            }
            setState(Connected);
        });

        connect(_socket,&QTcpSocket::readyRead, [this](){
            this->readData();
        });

        connect(_socket,&QAbstractSocket::errorOccurred,[&](QAbstractSocket::SocketError){

            errorString = _socket->errorString();
            if(_ip != "" && _port != "")
            {
                _reconnectTimer->setSingleShot(true);
                _reconnectTimer->setInterval(1000);
                _reconnectTimer->start();
                setState(Reconnecting);
            }

        });

        connect(_socket,&QTcpSocket::disconnected,this, &TcpSocket::onDisconnected);
    }

public slots:
    void onDisconnected()
    {
        setState(Unconnected);
    }

    QHostAddress peerAddress()
    {
        return _socket->peerAddress();
    }

    quint16 peerPort()
    {
        return _socket->peerPort();
    }

    void disconnectFromHost() const
    {
        _socket->close();
        if(_socket->state()!=QAbstractSocket::UnconnectedState){
            _socket->abort();
        }
    }
public:
    void sendPacket(const QString& text)
    {
        if( _state != Connected )
        {
            qDebug() << "socket not connected, currentState: " << socketStateToString(_state);
            return;
        }
        const QByteArray& send_data=text.toUtf8();
        qint64 packetlen = send_data.length();
        if(send_data.isEmpty())
            return;
        _socket->write((const char*)&packetlen, sizeof(packetlen));
        _socket->write(send_data);
    }

private slots:
    void readData()
    {
        if(_socket->bytesAvailable() <=0)
            return;

        dataBuffer.append(_socket->readAll());

        qint64 total_bytes;

        int total_length = dataBuffer.size();

        while(total_length)
        {
            if(total_length < sizeof(qint64))
                break;
            total_bytes = *(const qint64*)dataBuffer.left(sizeof(qint64)).data();
            if(total_length < total_bytes + sizeof(qint64))
                break;

            dataBuffer.remove(0, sizeof(qint64));
            const QString& instruct = QString::fromUtf8(dataBuffer.left(total_bytes));
            emit receivePacket(instruct);

            dataBuffer.remove(0, total_bytes);
            total_length = dataBuffer.size();
        }
    }
private:
    QByteArray dataBuffer;
private:
    QTcpSocket* _socket;

private:
    QString _ip;
    QString _port;
signals:
    void receivePacket(const QString& instruct);
    void disconnected() const;
    void connected() const;
    void reconnecting() const;
};
#endif // TCPSOCKET_H
