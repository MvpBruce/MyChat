#include "registerdialog.h"
#include "ui_registerdialog.h"
//#include <QRegularExpression>
#include "core/HttpMgr.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->lineEdit_Pwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_Confirm->setEchoMode(QLineEdit::Password);

    ui->error_tip->setProperty("state", "normal");
    resetStyle(ui->error_tip);

    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_register_finished, this, &RegisterDialog::slot_register_finished);
    initHandlers();
    ui->error_tip->clear();

    connect(ui->lineEdit_User, &QLineEdit::editingFinished, this, &RegisterDialog::checkValidUser);
    connect(ui->lineEdit_Email, &QLineEdit::editingFinished, this, &RegisterDialog::checkValidEmail);
    connect(ui->lineEdit_Pwd, &QLineEdit::editingFinished, this, &RegisterDialog::checkValidPassWord);
    connect(ui->lineEdit_Confirm, &QLineEdit::editingFinished, this, &RegisterDialog::checkValidComfirPwd);
    connect(ui->lineEdit_Code, &QLineEdit::editingFinished, this, &RegisterDialog::checkValidVerifyCode);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_btn_GetCode_clicked()
{
    auto email = ui->lineEdit_Email->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool bMatched = regex.match(email).hasMatch();
    if (bMatched)
    {
        QJsonObject jObj;
        jObj["email"] = email;
        HttpMgr::GetInstance()->PostHttpRequst(QUrl(strGateServerURL + "/get_verifycode"), jObj, RequstID::GET_VERIFY_CODE, Modules::REGISTER);
    }
    else
    {
        showTip(tr("Invalid email"), false);
    }
}

void RegisterDialog::showTip(QString str, bool isErrorr)
{
    if (isErrorr)
        ui->error_tip->setProperty("state", "error");
    else
        ui->error_tip->setProperty("state", "normal");

    ui->error_tip->setText(str);
    resetStyle(ui->error_tip);
}

void RegisterDialog::slot_register_finished(RequstID id, QString res, ErrorCodes ec)
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

void RegisterDialog::initHandlers()
{
    m_handlers.insert(RequstID::GET_VERIFY_CODE, [this](const QJsonObject jObj){
        int nError = jObj["error"].toInt();
        if (nError != (int)ErrorCodes::SUCCESS)
        {
            this->showTip("Parameters error", false);
            return;
        }

        auto email = jObj["email"].toString();
        this->showTip("Code has been sent to email", true);
        qDebug() << "Email is: " << email;
    });

    m_handlers.insert(RequstID::REGISTER_USER, [this](const QJsonObject jObj) {
        int nError = jObj["error"].toInt();
        if (nError != (int)ErrorCodes::SUCCESS)
        {
            this->showTip("Parameters error", false);
            return;
        }

        auto email = jObj["email"].toString();
        this->showTip("Success to register", true);
        qDebug() << "Register email is: " << email;
        qDebug() << "User uuid is: " << jObj["verifycode"];
        emit switchToLogin();
    });
}

bool RegisterDialog::checkValidEmail()
{
    auto email = ui->lineEdit_Email->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool bMatched = regex.match(email).hasMatch();
    if (bMatched)
    {
        ui->error_tip->clear();
        return true;
    }
    else
    {
        showTip(tr("Invalid email"), false);
        return false;
    }
}

bool RegisterDialog::checkValidUser()
{
    if (ui->lineEdit_User->text() == "")
    {
        showTip(tr("User name can't be empty"), false);
        return false;
    }

    ui->error_tip->clear();
    return true;
}

bool RegisterDialog::checkValidPassWord()
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

bool RegisterDialog::checkValidComfirPwd()
{
    if (ui->lineEdit_Pwd->text() != ui->lineEdit_Confirm->text())
    {
        showTip(tr("Passwords were not matched"), false);
        return false;
    }

    ui->error_tip->clear();
    return true;
}

bool RegisterDialog::checkValidVerifyCode()
{
    auto pass = ui->lineEdit_Code->text();
    if(pass.isEmpty()){
        showTip(tr("Verification code can't be empty"), false);
        return false;
    }

    ui->error_tip->clear();
    return true;
}

void RegisterDialog::on_btn_ok_clicked()
{
    bool bRet = checkValidUser();
    if (!bRet)
        return;

    bRet = checkValidEmail();
    if (!bRet)
        return;

    bRet = checkValidPassWord();
    if (!bRet)
        return;

    bRet = checkValidVerifyCode();
    if (!bRet)
        return;

    //Send post request to gate server by json
    QJsonObject jsonObj;
    jsonObj["user"] = ui->lineEdit_User->text();
    jsonObj["email"] = ui->lineEdit_Email->text();
    jsonObj["password"] = ui->lineEdit_Pwd->text();
    jsonObj["verifycode"] = ui->lineEdit_Code->text();

    HttpMgr::GetInstance()->PostHttpRequst(QUrl(strGateServerURL + "/register"), jsonObj, RequstID::REGISTER_USER, Modules::REGISTER);
}

void RegisterDialog::on_btn_Cancel_clicked()
{
    emit switchToLogin();
}

