#include "userchatitem.h"
#include "ui_userchatitem.h"
#include <QPixmap>

UserChatItem::UserChatItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::UserChatItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
}

UserChatItem::~UserChatItem()
{
    delete ui;
}

QSize UserChatItem::sizeHint() const
{
    return QSize(250, 70);
}

void UserChatItem::SetInfo(QString name, QString head, QString msg)
{
    m_Name = name;
    m_Head = head;
    m_Msg = msg;

    QPixmap pixMap(m_Head);

    ui->icon_lb->setPixmap(pixMap.scaled(ui->icon_lb->size()));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(m_Name);
    ui->user_chat_lb->setText(m_Msg);
}
