#include "authenfrienddlg.h"
#include "ui_authenfrienddlg.h"

AuthenFriendDlg::AuthenFriendDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthenFriendDlg)
{
    ui->setupUi(this);
}

AuthenFriendDlg::~AuthenFriendDlg()
{
    delete ui;
}
