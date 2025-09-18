#ifndef USERCHATITEM_H
#define USERCHATITEM_H

#include <QWidget>
#include "listitembase.h"
#include "core/userdata.h"

namespace Ui {
class UserChatItem;
}

class UserChatItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit UserChatItem(QWidget *parent = nullptr);
    ~UserChatItem();

    QSize sizeHint() const override;

    void SetInfo(QString name, QString head, QString msg);
    void SetInfo(std::shared_ptr<UserInfo> pInfo);
    void SetInfo(std::shared_ptr<FriendInfo> pInfo);
    void UpdateLasgMsg(const std::vector<std::shared_ptr<ChatTextData>>& vTextData);

private:
    Ui::UserChatItem *ui;
    QString m_Name;
    QString m_Head;
    QString m_Msg;
    std::shared_ptr<UserInfo> m_pInfo;
};

#endif // USERCHATITEM_H
