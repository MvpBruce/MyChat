#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QListWidget>

class UserContactItem;
class UserInfo;
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

signals:
    void sig_load_contacts();
    void sig_switch_friend_info_page(std::shared_ptr<UserInfo> user_info);

private:
    UserContactItem* m_pContactItem;
};

#endif // CONTACTLIST_H
