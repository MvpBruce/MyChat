#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QListWidget>
#include "core/userdata.h"

class UserContactItem;
class ContactList : public QListWidget
{
    Q_OBJECT
public:
    ContactList(QWidget* parent = nullptr);
    void ShowRedPoint(bool bShow = true);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    void AddContactList();

public slots:
    void slot_item_clicked(QListWidgetItem* item);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> pInfo);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> pInfo);

signals:
    void sig_load_contacts();
    void sig_switch_friend_info_page(std::shared_ptr<UserInfo> user_info);


private:
    UserContactItem* m_pAddFriendItem;
    QListWidgetItem* m_pGroupItem;
};

#endif // CONTACTLIST_H
