#include "HttpMgr.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonObject>


HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{
    connect(this, &HttpMgr::sig_http_finished, this, &HttpMgr::slot_http_finished);
}

void HttpMgr::PostHttpRequst(QUrl url, QJsonObject json, RequstID id, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    //content
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.length());
    QNetworkReply* reply = m_NetworkMgr.post(request, data);
    auto self = shared_from_this();
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, id, mod](){
        QString res = "";
        ErrorCodes ec = ErrorCodes::NEWTORK;
        if (reply->error() != QNetworkReply::NoError)
        {
            qDebug() << reply->errorString();
        }
        else
        {
            res = reply->readAll();
            ec = ErrorCodes::SUCCESS;
        }

        emit self->sig_http_finished(id, res, ec, mod);
        reply->deleteLater();
        return;
    });
}

void HttpMgr::slot_http_finished(RequstID id, QString res, ErrorCodes ec, Modules mod)
{
    if (mod == Modules::REGISTER)
    {
        //send register signal
        emit sig_register_finished(id, res, ec);
    }
}
