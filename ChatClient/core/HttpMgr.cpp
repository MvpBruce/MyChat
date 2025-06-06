#include "HttpMgr.h"
#include <QJsonDocument>
#include <QNetworkReply>


HttpMgr::~HttpMgr()
{
    connect(this, &HttpMgr::sig_http_finished, this, &HttpMgr::slot_http_finished);
}

HttpMgr::HttpMgr()
{

}

void HttpMgr::PostHttpRequst(QUrl url, QJsonObject json, RequstID id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

    QNetworkReply* reply = m_NetworkMgr.post(request, data);
    auto self = shared_from_this();
    QObject::connect(reply, &QNetworkReply::finished, [reply, self, id, mod](){
        //Error case
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << reply->errorString();
            emit self->sig_http_finished(id, "", ErrorCodes::NEWTORK, mod);
            reply->deleteLater();
            return;
        }

        //Sucess
        QString res = reply->readAll();
        emit self->sig_http_finished(id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    });
}

void HttpMgr::slot_http_finished(RequstID id, QString res, ErrorCodes ec, Modules md)
{
    if (md == Modules::REGISTER)
    {
        emit sig_register_finished(id, res, ec);
    }
}
