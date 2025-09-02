#include "userfounddlg.h"
#include "ui_userfounddlg.h"

UserFoundDlg::UserFoundDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserFoundDlg)
{
    ui->setupUi(this);
}

UserFoundDlg::~UserFoundDlg()
{
    delete ui;
}
