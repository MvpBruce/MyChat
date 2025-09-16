#ifndef APPLYFRIENDITEM_H
#define APPLYFRIENDITEM_H

#include <QWidget>
#include "listitembase.h"
#include "core/userdata.h"

namespace Ui {
class ApplyFriendItem;
}


class ApplyFriendItem : public ListItemBase
{
    Q_OBJECT
public:
    ApplyFriendItem(QWidget *parent = nullptr);
    ~ApplyFriendItem();
    void SetInfo(std::shared_ptr<ApplyInfo> pInfo);
    void ShowAddBtn(bool bShow);
    QSize sizeHint() const override;
    int GetUid();

private:
    Ui::ApplyFriendItem* ui;
    std::shared_ptr<ApplyInfo> m_pApplyInfo;
    bool m_bAdded;

signals:
    void sig_auth_friend(std::shared_ptr<ApplyInfo> pInfo);
};

#endif // APPLYFRIENDITEM_H
