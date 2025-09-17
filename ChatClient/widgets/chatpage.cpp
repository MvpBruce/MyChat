#include "chatpage.h"
#include "ui_chatpage.h"
#include "global/global.h"
#include "chatmsgbaseitem.h"
#include "textbubble.h"
#include "imagebubble.h"
#include "core/usermgr.h"
#include <QJsonObject>
#include <QJsonArray>
#include "core/TcpMgr.h"

ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage), m_pPeerUserInfo(nullptr)
{
    ui->setupUi(this);
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::on_send_btn_clicked()
{
    if (m_pPeerUserInfo == nullptr)
    {
        qDebug() << "Friend list is empty";
        return;
    }

    auto pUserInfo = UserMgr::GetInstance()->GetUserInfo();
    Role role = Role::Self;
    QString userName = pUserInfo->m_strName;
    QString userIcon = pUserInfo->m_strIcon;

    QJsonObject textObj;
    QJsonArray textArray;
    auto pTextEdit = ui->chat_text_edit;
    const QVector<MsgInfo>& msgList = pTextEdit->GetMessages();
    int nTextSize = 0;
    for (int i = 0; i < msgList.size(); i++)
    {
        //message len is over 1024, not processed
        if (msgList[i].content.length() > 1024)
            continue;

        QString strType = msgList[i].msgType;
        ChatMsgBaseItem* pChatItem = new ChatMsgBaseItem(role);
        pChatItem->setIcon(QPixmap(userIcon));
        pChatItem->setNmae(userName);
        QWidget* pBubble = nullptr;
        if (strType == "text")
        {
            pBubble = new TextBubble(role, msgList[i].content);
            if (nTextSize + msgList[i].content.length() >1024)
            {
                textObj["fromuid"] = pUserInfo->m_nUID;
                textObj["touid"] = m_pPeerUserInfo->m_nUID;
                textObj["text_array"] = textArray;
                QJsonDocument jDoc(textObj);
                QByteArray jsonData = jDoc.toJson(QJsonDocument::Compact);
                nTextSize = 0;
                textArray = QJsonArray();
                textObj = QJsonObject();
                emit TcpMgr::GetInstance()->sig_send_data(RequstID::TEXT_CHAT_MSG_REQ, jsonData);
            }

            nTextSize += msgList[i].content.length();
            QUuid uuid = QUuid::createUuid();
            QString strUuid = uuid.toString();
            QJsonObject obj;
            QByteArray byteMessage = msgList[i].content.toUtf8();
            obj["content"] = QString::fromUtf8(byteMessage);
            obj["msgid"] = strUuid;
            textArray.append(obj);
            auto pChatData = std::make_shared<ChatTextData>(strUuid, obj["content"].toString(), pUserInfo->m_nUID, m_pPeerUserInfo->m_nUID);
            emit sig_append_chat_msg(pChatData);
        }
        else if (strType == "image")
        {

        }
        else if (strType == "file")
        {

        }

        if (pBubble)
        {
            pChatItem->setWidget(pBubble);
            ui->chat_history_list->appendChild(pChatItem);
        }
    }

    qDebug() << "TextArray is: " << textArray;
    //send text info to server
    textObj["text_array"] = textArray;
    textObj["fromuid"] = pUserInfo->m_nUID;
    textObj["touid"] = m_pPeerUserInfo->m_nUID;
    QJsonDocument doc(textObj);
    QByteArray JsonData = doc.toJson(QJsonDocument::Compact);
    textArray = QJsonArray();
    textObj = QJsonObject();
    emit TcpMgr::GetInstance()->sig_send_data(RequstID::TEXT_CHAT_MSG_REQ, JsonData);
}

