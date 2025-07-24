#include "logindialog.h"
#include "ui_logindialog.h"
#include <QJsonObject>
#include "core/HttpMgr.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchToRegister);
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_userLogin_finished, this, &LoginDialog::slot_user_login_finish);
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

void LoginDialog::on_pushButton_clicked()
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
    jsonObj["passowrd"] = pwd;

    HttpMgr::GetInstance()->PostHttpRequst(QUrl(strGateServerURL + "/login_user"), jsonObj, RequstID::LOGIN_USER, Modules::LOGIN);
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
        qDebug()<< "User is: " << user ;
    });
}

