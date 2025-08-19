#include "logindialog.h"
#include "ui_logindialog.h"
#include <QJsonObject>
#include "core/HttpMgr.h"
#include "core/TcpMgr.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchToRegister);
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_userLogin_finished, this, &LoginDialog::slot_user_login_finish);

    connect(this, &LoginDialog::sig_connect_tcp, TcpMgr::GetInstance().get(), &TcpMgr::slot_tcp_connect);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_connect_success, this, &LoginDialog::slot_tcp_connect_finished);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_login_failed, this, &LoginDialog::slot_login_failed);
    initHandlers();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_forget_btn_clicked()
{
    emit switchToResetPwd();
}

bool LoginDialog::checkValidUser()
{
    if (ui->lineEdit_User->text() == "")
    {
        showTip(tr("User name can't be empty"), false);
        return false;
    }

    ui->error_tip->clear();
    return true;
}

bool LoginDialog::checkValidPassWord()
{
    auto password = ui->lineEdit_Pwd->text();
    if (password.length() < 6 || password.length() > 18)
    {
        showTip(tr("Passowrd length must be 6 ~ 18"), false);
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool bMatch = regExp.match(password).hasMatch();
    if(!bMatch){
        showTip(tr("Can't contain illegal charactors"), false);
        return false;
    }

    ui->error_tip->clear();
    return true;
}

void LoginDialog::showTip(QString str, bool isErrorr)
{
    if (isErrorr)
        ui->error_tip->setProperty("state", "error");
    else
        ui->error_tip->setProperty("state", "normal");

    ui->error_tip->setText(str);
    resetStyle(ui->error_tip);
}

void LoginDialog::slot_user_login_finish(RequstID id, QString res, ErrorCodes ec)
{
    if (ec == ErrorCodes::NEWTORK)
    {
        showTip(tr("Network request error"), false);
        return;
    }

    QJsonDocument jDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jDoc.isEmpty())
    {
        showTip(tr("Parse json error"), false);
        return;
    }

    if (!jDoc.isObject())
    {
        showTip(tr("Parse json error"), false);
        return;
    }

    QJsonObject jObj = jDoc.object();

    m_handlers[id](jObj);
}

void LoginDialog::initHandlers()
{
    m_handlers.insert(RequstID::LOGIN_USER, [this](const QJsonObject jObj){
        int nError = jObj["error"].toInt();
        if (nError != (int)ErrorCodes::SUCCESS)
        {
            showTip("Parameters error", false);
            return;
        }

        auto user = jObj["user"].toString();
        showTip(tr("Login successful"), true);

        ServerInfo info;
        info.host = jObj["host"].toString();
        info.port = jObj["port"].toString();
        info.uId = jObj["uid"].toInt();
        info.token = jObj["token"].toString();

        m_uid = info.uId;
        m_token = info.token;

        qDebug()<< "User: " << user << ", uid: " << info.uId << ", host: " << info.host
                 << ", port: " << info.port << ", token: " << info.token;

        //Notify tcpmgr to send tcp request to chat server
        emit sig_connect_tcp(info);
    });
}

void LoginDialog::slot_tcp_connect_finished(bool bSuccess)
{
    if (bSuccess)
    {
        showTip(tr("Connected to chat server, logining..."), true);
        QJsonObject jObj;
        jObj["uid"] = m_uid;
        jObj["token"] = m_token;

        QJsonDocument doc(jObj);
        QByteArray jsonData = doc.toJson();
        //Send tcp request to chat server
        emit TcpMgr::GetInstance()->sig_send_data(RequstID::CHAT_LOGIN, jsonData);
    }
    else
    {
        showTip(tr("Newtork exception"), false);
    }
}

void LoginDialog::slot_login_failed(int error)
{
    QString strTips = QString("Login failed, error: %1").arg(error);
    showTip(strTips, false);
    //enableButtons(false);
}

void LoginDialog::enableButtons(bool enabled)
{
    ui->login_btn->setEnabled(enabled);
    ui->reg_btn->setEnabled(enabled);
}


void LoginDialog::on_login_btn_clicked()
{
    qDebug()<<"login btn clicked";
    if (!checkValidUser())
        return;

    if (!checkValidPassWord())
        return;

    auto user = ui->lineEdit_User->text();
    auto pwd = ui->lineEdit_Pwd->text();
    //Send http request
    QJsonObject jsonObj;
    jsonObj["user"] = user;
    jsonObj["password"] = pwd;

    HttpMgr::GetInstance()->PostHttpRequst(QUrl(strGateServerURL + "/login_user"), jsonObj, RequstID::LOGIN_USER, Modules::LOGIN);
}

