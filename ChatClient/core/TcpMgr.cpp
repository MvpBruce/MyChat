#include "TcpMgr.h"

TcpMgr::~TcpMgr()
{
}

TcpMgr::TcpMgr(): m_host(""), m_port(0), m_bPending(false), m_messageId(0), m_messageLen(0)
{
    //Connected
    QObject::connect(&m_socket, &QTcpSocket::connected, [&]() {
        qDebug() << "Connected to chat server!";
        emit sig_connect_success(true);
    });

    //After finished tcp connection, will trigger in login dialog
    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);

    //When data is ready
    QObject::connect(&m_socket, &QTcpSocket::readyRead, [&] () {

    });

    //Exceptions
    QObject::connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
        qDebug() << "Error:" << m_socket.errorString();
    });

    //Socket closed
    QObject::connect(&m_socket, &QTcpSocket::disconnected, [&]{
        qDebug() << "Disconnected from server.";
    });

    initHandlers();
}

void TcpMgr::slot_tcp_connect(ServerInfo info)
{
    qDebug() << "Connecting to server...";
    m_host = info.host;
    m_port = info.port.toUInt();
    //Will trigger QTcpSocket::connected
    m_socket.connectToHost(m_host, m_port);
}

void TcpMgr::slot_send_data(RequstID reqId, QByteArray data)
{
    uint16_t id = static_cast<uint16_t>(reqId);
    uint16_t dataLen = data.length();
    QByteArray dataBlock;
    QDataStream stream(&dataBlock, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << id << dataLen;
    dataBlock.append(data);

    //Send data to chat server
    m_socket.write(dataBlock);
    qDebug() << "Tcp server sent date";
}

void TcpMgr::initHandlers()
{
    m_handlers.insert(RequstID::CHAT_LOGIN, [](RequstID id, int len, QByteArray data){

    });
}
