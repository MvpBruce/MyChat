#include "userfounddlg.h"
#include "ui_userfounddlg.h"
#include "core/userdata.h"
#include "applyfriend.h"

UserFoundDlg::UserFoundDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserFoundDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("Add");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    ui->add_to_contact->SetState("normal", "hover", "press");
    this->setModal(true);

}

UserFoundDlg::~UserFoundDlg()
{
    delete ui;
}

void UserFoundDlg::SetSearchInfo(std::shared_ptr<SearchInfo> pInfo)
{
    ui->name_lb->setText(pInfo->m_strName);
    QPixmap headPixmap(pInfo->m_strIcon);
    headPixmap = headPixmap.scaled(ui->head_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lb->setPixmap(headPixmap);
    m_pSearchInfo = pInfo;
}

void UserFoundDlg::on_add_to_contact_clicked()
{
    this->hide();
    auto pApplyFriend = new ApplyFriend(this);
    pApplyFriend->SetSearchInfo(m_pSearchInfo);
    pApplyFriend->setModal(true);
    pApplyFriend->show();
}

