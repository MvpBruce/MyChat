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
    //todo
    m_pUserInfo = std::make_shared<UserInfo>();
}

void UserContactItem::ShowRedPoint(bool bShow)
{
    if (bShow)
        ui->red_piont->show();
    else
        ui->red_piont->hide();
}
