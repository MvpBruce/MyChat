#ifndef HTTPMGR_H
#define HTTPMGR_H

#include "Singleton.h"
#include <QObject>
#include <QNetworkAccessManager>
#include "global/global.h"
#include <QObject>
#include <QJsonObject>


class HttpMgr:public QObject, public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT

public:
    ~HttpMgr();

private:
    friend class Singleton<HttpMgr>;
    HttpMgr();

    void PostHttpRequst(QUrl url, QJsonObject json, RequstID id, Modules mod);

    void slot_http_finished(RequstID id, QString res, ErrorCodes ec, Modules md);

private:
    QNetworkAccessManager m_NetworkMgr;

signals:
    void sig_http_finished(RequstID id, QString res, ErrorCodes ec, Modules md);
    void sig_register_finished(RequstID id, QString res, ErrorCodes ec);
};

#endif // HTTPMGR_H
