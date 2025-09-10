#ifndef USERCONTACTITEM_H
#define USERCONTACTITEM_H

#include "listitembase.h"

namespace Ui {
class UserContactItem;
}

class UserInfo;
class UserContactItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit UserContactItem(QWidget *parent = nullptr);
    ~UserContactItem();
    QSize sizeHint() const override;
    void SetInfo(int nUid, QString strName, QString strIcon);
    void ShowRedPoint(bool bShow = false);
    std::shared_ptr<UserInfo> GetInfo();

private:
    Ui::UserContactItem *ui;
    std::shared_ptr<UserInfo> m_pUserInfo;
};

#endif // USERCONTACTITEM_H
