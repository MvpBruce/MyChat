#ifndef APPLYFRIENDITEM_H
#define APPLYFRIENDITEM_H

#include <QWidget>
#include "listitembase.h"
#include "core/userdata.h"

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
    std::shared_ptr<ApplyInfo> m_pApplyInfo;
};

#endif // APPLYFRIENDITEM_H
