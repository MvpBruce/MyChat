#include "chatpage.h"
#include "ui_chatpage.h"
#include "global/global.h"
#include "chatmsgbaseitem.h"
#include "textbubble.h"
#include "imagebubble.h"
#include "core/usermgr.h"

ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage), m_pUserInfo(nullptr)
{
    ui->setupUi(this);
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::on_send_btn_clicked()
{
    if (m_pUserInfo == nullptr)
    {
        qDebug() << "Friend list is empty";
        return;
    }

    auto pUserInfo = UserMgr::GetInstance()->GetUserInfo();
    auto pTextEdit = ui->chat_text_edit;
    Role role = Role::Self;
    QString userName = pUserInfo->m_strName;
    QString userIcon = pUserInfo->m_strIcon;
    ChatMsgBaseItem* pChatItem = new ChatMsgBaseItem(role);
    pChatItem->setIcon(QPixmap(userIcon));
    pChatItem->setNmae(userName);
    TextBubble* pTextBubble = new TextBubble(role, "Hello world");
    pChatItem->setWidget(pTextBubble);
    ui->chat_history_list->appendChild(pChatItem);

    ChatMsgBaseItem* pOther = new ChatMsgBaseItem(Role::Other);
    pOther->setIcon(QPixmap(":/assets/image/head_2.jpg"));
    pOther->setNmae(QString("Other"));
    pTextBubble = new TextBubble(Role::Other, "Received hello world");
    pOther->setWidget(pTextBubble);
    ui->chat_history_list->appendChild(pOther);


    pChatItem = new ChatMsgBaseItem(role);
    pChatItem->setIcon(QPixmap(userIcon));
    pChatItem->setNmae(userName);
    ImageBubble* pBubble = new ImageBubble(QPixmap(":/assets/image/head_3.jpg"), role);
    pChatItem->setWidget(pBubble);
    ui->chat_history_list->appendChild(pChatItem);
}

