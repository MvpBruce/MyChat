#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global/global.h"
#include <QRegularExpression>
#include "core/HttpMgr.h"
#include <QJsonDocument>
#include <QJsonObject>

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
}

