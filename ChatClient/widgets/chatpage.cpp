#include "chatpage.h"
#include "ui_chatpage.h"
#include "global/global.h"
#include "chatmsgbaseitem.h"
#include "textbubble.h"

ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
{
    ui->setupUi(this);
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::on_send_btn_clicked()
{
    //auto textEdit = ui->chat_text_edit;
    Role role = Role::Self;
    QString userName = "test user";
    QString userIcon = ":/assets/image/head_1.jpg";
    ChatMsgBaseItem* pChatItem = new ChatMsgBaseItem(role);
    pChatItem->setIcon(QPixmap(userIcon));
    pChatItem->setNmae(userName);
    TextBubble* pTextBubble = new TextBubble(role, "Hello world");
    pChatItem->setWidget(pTextBubble);
    ui->chat_history_list->appendChild(pChatItem);
}

