#include "applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent) {}

ApplyFriendItem::~ApplyFriendItem()
{

}

void ApplyFriendItem::SetInfo(std::shared_ptr<ApplyInfo> pInfo)
{

}

void ApplyFriendItem::ShowAddBtn(bool bShow)
{

}

QSize ApplyFriendItem::sizeHint() const
{
    return QSize(250, 80);
}

int ApplyFriendItem::GetUid()
{
    return m_pApplyInfo->m_nUID;
}
