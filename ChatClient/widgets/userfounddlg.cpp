#include "userfounddlg.h"
#include "ui_userfounddlg.h"
#include "core/userdata.h"

UserFoundDlg::UserFoundDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserFoundDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("Add");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    //todo, need to use real image
    QPixmap headPixmap(":/assets/image/head_1.jpg");
    headPixmap = headPixmap.scaled(ui->head_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lb->setPixmap(headPixmap);
    ui->add_to_contact->SetState("normal", "hover", "press");
    this->setModal(true);

}

UserFoundDlg::~UserFoundDlg()
{
    delete ui;
}

void UserFoundDlg::SetSearchInfo(std::shared_ptr<SearchInfo> info)
{
    ui->name_lb->setText(info->m_strName);
    m_pSearchInfo = info;
}

void UserFoundDlg::on_add_to_contact_clicked()
{
    this->hide();
    //todo, pop up apply friend ui and show
}

