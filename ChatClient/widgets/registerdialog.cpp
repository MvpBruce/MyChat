#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global/global.h"
#include <QRegularExpression>

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->lineEdit_Pwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_Confirm->setEchoMode(QLineEdit::Password);

    ui->error_tip->setProperty("state", "normal");
    resetStyle(ui->error_tip);
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
        showTip(tr("Invalid email"));
    }
}

void RegisterDialog::showTip(QString str)
{
    ui->error_tip->setText(str);
    ui->error_tip->setProperty("state", "error");
    resetStyle(ui->error_tip);
}

