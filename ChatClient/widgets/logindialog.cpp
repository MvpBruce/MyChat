#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchToRegister);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_forget_btn_clicked()
{
    emit switchToResetPwd();
}

