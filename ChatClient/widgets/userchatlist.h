#ifndef USERCHATLIST_H
#define USERCHATLIST_H

#include <QListWidget>

class UserChatList : public QListWidget
{
    Q_OBJECT
public:
    UserChatList(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

signals:
    void sig_load_users();
};

#endif // USERCHATLIST_H
