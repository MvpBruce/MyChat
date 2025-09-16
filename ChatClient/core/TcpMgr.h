#pragma once
#include "global/global.h"
#include "Singleton.h"
#include <QTcpSocket>
#include <QObject>

class SearchInfo;
class AddFriendInfo;
class AuthInfo;
class TcpMgr : public QObject, public Singleton<TcpMgr>, public std::enable_shared_from_this<TcpMgr>
{
Q_OBJECT
	friend class Singleton<TcpMgr>;
public:
	~TcpMgr();

public slots:
    void slot_tcp_connect(ServerInfo info);
    void slot_send_data(RequstID reqId, QByteArray data);

signals:
    void sig_connect_success(bool bSuccess);
    void sig_send_data(RequstID reqId, QByteArray data);
    void sig_login_failed(int nError);
    void sig_switch_chatDialog();
    void sig_user_search(std::shared_ptr<SearchInfo>);
    void sig_friend_apply(std::shared_ptr<AddFriendInfo>);
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);

private:
    TcpMgr();
    void initHandlers();
    void processMsg(RequstID id, int len, QByteArray data);

private:
    QTcpSocket m_socket;
    QString m_host;
    quint16 m_port;
    bool m_bPending; //Need to continue to parse msg thats means it has processed head, but still need to process body
    QByteArray m_buffer;
    quint16 m_messageId;
    quint16 m_messageLen;
    QMap<RequstID, std::function<void(RequstID id, int len, QByteArray data)>> m_handlers;
};

