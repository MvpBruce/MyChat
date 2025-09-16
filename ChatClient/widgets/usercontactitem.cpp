#include "usercontactitem.h"
#include "ui_usercontactitem.h"
#include "core/userdata.h"

UserContactItem::UserContactItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::UserContactItem)
{
    ui->setupUi(this);
    this->SetItemType(ListItemType::CONTACT_USER_ITEM);
    ShowRedPoint(true);
}

UserContactItem::~UserContactItem()
{
    delete ui;
}

QSize UserContactItem::sizeHint() const
{
    return QSize(250, 70);
}

void UserContactItem::SetInfo(int nUid, QString strName, QString strIcon)
{
    m_pUserInfo = std::make_shared<UserInfo>(nUid, strName, strName, strIcon, 0);
    QPixmap pixMap(strIcon);
    ui->icon_lb->setPixmap(pixMap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);
    ui->contact_name_lb->setText(strName);
}

void UserContactItem::SetInfo(std::shared_ptr<AuthInfo> pInfo)
{
    m_pUserInfo = std::make_shared<UserInfo>(pInfo);
    QPixmap pixMap(m_pUserInfo->m_strIcon);
    ui->icon_lb->setPixmap(pixMap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);
    ui->contact_name_lb->setText(m_pUserInfo->m_strName);
}

void UserContactItem::ShowRedPoint(bool bShow)
{
    if (bShow)
        ui->red_piont->show();
    else
        ui->red_piont->hide();
}

std::shared_ptr<UserInfo> UserContactItem::GetInfo()
{
    return m_pUserInfo;
}
