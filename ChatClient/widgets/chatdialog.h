#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global/global.h"

namespace Ui {
class ChatDialog;
}

class StateWidget;
class UserInfo;
class AddFriendInfo;
class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

    void AddChatUserList();
    void ClearSideBarState(StateWidget* pWidget);

public slots:
    void slot_load_more_users();
    void slot_clicked_chat();
    void slot_clicked_contact();
    void slot_text_changed(const QString& text);
    void slot_friend_info_page(std::shared_ptr<UserInfo> pUserInfo);
    void slot_apply_friend(std::shared_ptr<AddFriendInfo> pInfo);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void SwitchMode(bool bSearch = false);
    void HandleMousePress(QMouseEvent* event);

private:
    Ui::ChatDialog *ui;
    bool m_bLoading;
    QList<StateWidget*> m_listWidget;
    SideBarMode m_Mode;
    SideBarMode m_State;
};

#endif // CHATDIALOG_H
