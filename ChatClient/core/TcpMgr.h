#pragma once
#include "global/global.h"
#include "Singleton.h"
#include <QTcpSocket>
#include <QObject>

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

private:
    TcpMgr();
    void initHandlers();

private:
    QTcpSocket m_socket;
    QString m_host;
    quint16 m_port;
    bool m_bPending;
    QByteArray m_buffer;
    quint16 m_messageId;
    quint16 m_messageLen;
    QMap<RequstID, std::function<void(RequstID id, int len, QByteArray data)>> m_handlers;
};

