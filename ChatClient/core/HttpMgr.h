#ifndef HTTPMGR_H
#define HTTPMGR_H

#include "Singleton.h"
#include <QObject>
#include <QNetworkAccessManager>
#include "global/global.h"
#include <QObject>


class HttpMgr:public QObject, public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT

public:
    ~HttpMgr();
    void PostHttpRequst(QUrl url, QJsonObject json, RequstID id, Modules mod);

private:
    friend class Singleton<HttpMgr>;
    HttpMgr();
    void slot_http_finished(RequstID id, QString res, ErrorCodes ec, Modules mod);

private:
    QNetworkAccessManager m_NetworkMgr;

signals:
    void sig_http_finished(RequstID id, QString res, ErrorCodes ec, Modules mod);
    void sig_register_finished(RequstID id, QString res, ErrorCodes ec);
};

#endif // HTTPMGR_H
