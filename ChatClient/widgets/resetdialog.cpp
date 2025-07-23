#include "resetdialog.h"
#include "ui_resetdialog.h"
#include <QRegularExpression>
#include <QJsonObject>
#include "core/HttpMgr.h"

ResetDialog::ResetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResetDialog)
{
    ui->setupUi(this);

    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_resetPwd_finished, this, &ResetDialog::slot_resetPwd_finished);
    connect(ui->lineEdit_User, &QLineEdit::editingFinished, this, &ResetDialog::checkValidUser);
    connect(ui->lineEdit_Email, &QLineEdit::editingFinished, this, &ResetDialog::checkValidEmail);
    connect(ui->lineEdit_Pwd, &QLineEdit::editingFinished, this, &ResetDialog::checkValidPassWord);
    connect(ui->lineEdit_Code, &QLineEdit::editingFinished, this, &ResetDialog::checkValidVerifyCode);

    ui->error_tip->clear();
    initHandlers();
}

ResetDialog::~ResetDialog()
{
    delete ui;
}

bool ResetDialog::checkValidEmail()
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

bool ResetDialog::checkValidUser()
{
    if (ui->lineEdit_User->text() == "")
    {
        showTip(tr("User name can't be empty"), false);
        return false;
    }

    ui->error_tip->clear();
    return true;
}

bool ResetDialog::checkValidPassWord()
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

bool ResetDialog::checkValidVerifyCode()
{
    auto pass = ui->lineEdit_Code->text();
    if(pass.isEmpty()){
        showTip(tr("Verification code can't be empty"), false);
        return false;
    }

    ui->error_tip->clear();
    return true;
}

void ResetDialog::initHandlers()
{
    m_handlers.insert(RequstID::GET_VERIFY_CODE, [this](const QJsonObject jObj) {
        int nError = jObj["error"].toInt();
        if (nError != (int)ErrorCodes::SUCCESS)
        {
            showTip("Parameters error", false);
            return;
        }

        auto email = jObj["email"].toString();
        this->showTip("Code has been sent to email", true);
        qDebug() << "Email is: " << email;

    });

    m_handlers.insert(RequstID::RESET_PWD, [this](const QJsonObject jObj) {
        int nError = jObj["error"].toInt();
        if (nError != (int)ErrorCodes::SUCCESS)
        {
            showTip("Parameters error", false);
            return;
        }

        auto email = jObj["email"].toString();
        this->showTip("Password has been reset, please return to login", true);
        qDebug() << "Email is: " << email;
        qDebug() << "User uuid is: " << jObj["uuid"].toString();
        emit switchToLogin();
    });
}

void ResetDialog::showTip(QString str, bool isErrorr)
{
    if (isErrorr)
        ui->error_tip->setProperty("state", "error");
    else
        ui->error_tip->setProperty("state", "normal");

    ui->error_tip->setText(str);
    resetStyle(ui->error_tip);
}

void ResetDialog::on_reset_pwd_btn_clicked()
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

    //Send http to reset password
    QJsonObject jsonObj;
    jsonObj["email"] = ui->lineEdit_Email->text();
    jsonObj["user"] = ui->lineEdit_User->text();
    jsonObj["password"] = ui->lineEdit_Pwd->text();
    jsonObj["verifycode"] = ui->lineEdit_Code->text();

    HttpMgr::GetInstance()->PostHttpRequst(QUrl(strGateServerURL + "/reset_pwd"), jsonObj, RequstID::RESET_PWD, Modules::RESET);
}


void ResetDialog::on_cancel_btn_clicked()
{
    emit switchToLogin();
}


void ResetDialog::on_get_code_btn_clicked()
{
    auto email = ui->lineEdit_Email->text();
    auto bRet = checkValidEmail();
    if (!bRet)
        return;

    //Send http to get verification code
    QJsonObject jsonObj;
    jsonObj["email"] = email;
    HttpMgr::GetInstance()->PostHttpRequst(QUrl(strGateServerURL + "/get_verifycode"), jsonObj, RequstID::GET_VERIFY_CODE, Modules::RESET);
}

void ResetDialog::slot_resetPwd_finished(RequstID id, QString res, ErrorCodes ec)
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

