#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include "core/userdata.h"

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
    void SetUserInfo(std::shared_ptr<UserInfo> pUserInfo);
    void AppendChatMsg(std::shared_ptr<ChatTextData> chatMsg);

private slots:
    void on_send_btn_clicked();

signals:
    void sig_append_chat_msg(std::shared_ptr<ChatTextData>);

private:
    Ui::ChatPage *ui;
    std::shared_ptr<UserInfo> m_pPeerUserInfo;//peer user
};

#endif // CHATPAGE_H
