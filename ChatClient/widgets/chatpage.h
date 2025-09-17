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

private slots:
    void on_send_btn_clicked();

private:
    Ui::ChatPage *ui;
    std::shared_ptr<UserInfo> m_pUserInfo;
};

#endif // CHATPAGE_H
