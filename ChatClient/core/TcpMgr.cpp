#include "TcpMgr.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "core/usermgr.h"
#include "core/userdata.h"

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
    QObject::connect(&m_socket, &QTcpSocket::readyRead, [this] () {
        m_buffer.append(m_socket.readAll());
        QDataStream stream(&m_buffer, QIODevice::ReadOnly);
        //todo
        while(true)
        {
            //Need to process msg head
            if (!m_bPending)
            {
                //buffer size less than head length, return and continue to read, id+msgLen
                if (m_buffer.size() < sizeof(uint16_t) * 2)
                    return;

                //Get id and msgLen
                stream >> m_messageId >> m_messageLen;
                m_buffer = m_buffer.mid(sizeof(uint16_t) * 2);
                qDebug() << "Msg id: " << m_messageId << ", msg length: " << m_messageLen;
            }

            //Rest of buffer length is less than msg len, continue to read data for msg body
            if (m_buffer.size() < m_messageLen)
            {
                m_bPending = true;
                return;
            }

            //Got enough msg body length and read it
            m_bPending = false;
            QByteArray msgBody = m_buffer.mid(0, m_messageLen);
            qDebug() << "Msg body: " << msgBody;
            //Move forward to new head for next msg
            m_buffer = m_buffer.mid(m_messageLen);
            processMsg(RequstID(m_messageId), m_messageLen, msgBody);
        }
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
    qDebug() << "Connecting to hsot: " << m_host << ":" << m_port;
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
    qDebug() << "Tcp server sent data: " << dataBlock;
}

void TcpMgr::initHandlers()
{
    m_handlers.insert(RequstID::CHAT_LOGIN_RSP, [this](RequstID id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug() << "Request id: " << static_cast<int>(id) <<", data: " << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (jsonDoc.isNull())
        {
            qDebug() << "Failed to create Json document.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error"))
        {
            qDebug() << "No error key in Json";
            emit sig_login_failed(static_cast<int>(ErrorCodes::JSON));
            return;
        }

        int nError = jsonObj["error"].toInt();
        if (nError != static_cast<int>(ErrorCodes::SUCCESS))
        {
            qDebug() << "Login failed: " << nError;
            emit sig_login_failed(nError);
            return;
        }

        auto uid = jsonObj["uid"].toInt();
        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto gender = jsonObj["gender"].toInt();
        auto pUserInfo = std::make_shared<UserInfo>(uid, name, nick, icon, gender);
        UserMgr::GetInstance()->SetUserInfo(pUserInfo);
        UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());
        //todo,
        //1.load apply list

        //2.get friend list from json
        emit sig_switch_chatDialog();
    });

    m_handlers.insert(RequstID::SEARCH_USER_RSP, [this](RequstID id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug() << "id: " << id << "data: " << data;
        QJsonDocument jDoc = QJsonDocument::fromJson(data);
        if (jDoc.isNull())
        {
            qDebug() << "Failed to create json document";
            return;
        }

        QJsonObject jObj = jDoc.object();
        if (!jObj.contains("error"))
        {
            qDebug() << "Failed to parse json, no error in json";
            emit sig_login_failed(ErrorCodes::JSON);
            return;
        }

        int nError = jObj["error"].toInt();
        if (nError != ErrorCodes::SUCCESS)
        {
            qDebug() << "Failed to search, error: " << nError;
            emit sig_login_failed(nError);
            return;
        }

        auto pSearchInfo = std::make_shared<SearchInfo>(jObj["uid"].toInt(), jObj["name"].toString(), jObj["nick"].toString(), jObj["desc"].toString(), jObj["gender"].toInt(), jObj["icon"].toString());
        emit sig_user_search(pSearchInfo);
    });

    m_handlers.insert(RequstID::ADD_FRIEND_RSP, [this](RequstID id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "Requst id: " << id << " data: " << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (jsonDoc.isNull())
        {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error"))
        {
            qDebug() << "Failed to parse json, no error in json";
            return;
        }

        int nError = jsonObj["error"].toInt();
        if (nError != ErrorCodes::SUCCESS)
        {
            qDebug() << "Failed to search, error: " << nError;
            return;
        }

        qDebug() << "Success to add friend" ;
    });

    m_handlers.insert(RequstID::ADD_FRIEND_NOTIFY, [this](RequstID id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "id: " << id << "data: " << data;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error")) {
            qDebug() << "Failed to parse json, no error in json";
            emit sig_user_search(nullptr);
            return;
        }

        int nError = jsonObj["error"].toInt();
        if (nError != ErrorCodes::SUCCESS)
        {
            qDebug() << "Failed to notify add friend, error: " << nError;
            emit sig_user_search(nullptr);
            return;
        }

        int nFromUId = jsonObj["applyuid"].toInt();
        QString strName = jsonObj["name"].toString();
        QString strDesc = jsonObj["desc"].toString();
        QString strIcon = jsonObj["icon"].toString();
        QString strNick = jsonObj["nick"].toString();
        int nGender = jsonObj["gender"].toInt();

        auto addFriendInfo = std::make_shared<AddFriendInfo>(nFromUId, strName, strDesc, strIcon, strNick, nGender);
        emit sig_friend_apply(addFriendInfo);
    });

    m_handlers.insert(RequstID::AUTH_FRIEND_NOTIFY, [this](RequstID id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "id: " << id << "data: " << data;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error")) {
            qDebug() << "Failed to parse json, no error in json";
            return;
        }

        int nError = jsonObj["error"].toInt();
        if (nError != ErrorCodes::SUCCESS)
        {
            qDebug() << "Failed to notify auth friend, error: " << nError;
            return;
        }

        int nFromUId = jsonObj["fromuid"].toInt();
        QString strName = jsonObj["name"].toString();
        QString strIcon = jsonObj["icon"].toString();
        QString strNick = jsonObj["nick"].toString();
        int nGender = jsonObj["gender"].toInt();

        auto pInfo = std::make_shared<AuthInfo>(nFromUId, strName, strNick, strIcon, nGender);
        emit sig_add_auth_friend(pInfo);
    });
}

void TcpMgr::processMsg(RequstID id, int len, QByteArray data)
{
    auto it = m_handlers.find(id);
    if (it == m_handlers.end())
    {
        qDebug() << "Not found handler id: " << static_cast<int>(id);
        return;
    }

    it.value()(id, len, data);
}
