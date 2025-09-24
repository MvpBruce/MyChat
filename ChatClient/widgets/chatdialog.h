#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global/global.h"
#include "core/userdata.h"

namespace Ui {
class ChatDialog;
}

class StateWidget;
class QListWidgetItem;
class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

public slots:
    void slot_load_more_users();
    void slot_clicked_chat();
    void slot_clicked_contact();
    void slot_text_changed(const QString& text);
    void slot_friend_info_page(std::shared_ptr<UserInfo> pUserInfo);
    void slot_switch_apply_friend_page();
    void slot_apply_friend(std::shared_ptr<AddFriendInfo> pInfo);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> pInfo);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> pInfo);
    void slot_append_chat_msg(std::shared_ptr<ChatTextData> pData);
    void slot_chat_text_msg(std::shared_ptr<ChatTextMsg> pTextMsg);
    void slot_item_clicked(QListWidgetItem *pItem);
    void slot_current_Item_Changed(QListWidgetItem *current, QListWidgetItem *previous);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void UpdateChatMsg(const std::vector<std::shared_ptr<ChatTextData>>& vChatText);

private:
    void SwitchMode(bool bSearch = false);
    void HandleMousePress(QMouseEvent* event);
    void AddChatUserList();
    void ClearSideBarState(StateWidget* pWidget);
    void SetCurChatItem(int nUid);

private:
    Ui::ChatDialog *ui;
    bool m_bLoading;
    QList<StateWidget*> m_listWidget;
    SideBarMode m_Mode;
    SideBarMode m_State;
    QMap<int, QListWidgetItem*> m_mapUidToItem;
    int m_nCurChatUId;
};

#endif // CHATDIALOG_H
