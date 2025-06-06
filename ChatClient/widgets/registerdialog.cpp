#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global/global.h"
#include <QRegularExpression>
#include "core/HttpMgr.h"
#include <QJsonDocument>

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

    }
    else
    {
        showTip(tr("Invalid email"), true);
    }
}

void RegisterDialog::showTip(QString str, bool isError)
{
    if (isError)
        ui->error_tip->setProperty("state", "error");
    else
        ui->error_tip->setProperty("state", "normal");

    ui->error_tip->setText(str);
    resetStyle(ui->error_tip);
}

void RegisterDialog::slot_register_finished(RequstID id, QString res, ErrorCodes ec)
{
    //Error case
    if (ec != ErrorCodes::SUCCESS)
    {
        showTip(tr("Network request failed"), false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull())
    {
        showTip(tr("Failed to parse Json"), false);
        return;
    }

    //Register callback function by request id
    m_handlers[id](jsonDoc.object());
}

void RegisterDialog::initHandlers()
{
    m_handlers.insert(RequstID::GET_VERIFY_CODE, [this](QJsonObject jsonObj){
        int ec = jsonObj["error"].toInt();
        if (ec != (int)ErrorCodes::SUCCESS)
        {
            showTip(tr("Error params"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("Verification code has been sent to your email"), false);
        qDebug() << "Email is: "<< email;
    });
}

