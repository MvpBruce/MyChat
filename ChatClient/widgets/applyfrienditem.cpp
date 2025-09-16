#include "applyfrienditem.h"
#include "ui_applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent)
    :ListItemBase(parent), ui(new Ui::ApplyFriendItem), m_bAdded(false)
{
    ui->setupUi(this);
    SetItemType(ListItemType::APPLY_FRIEND_ITEM);
    ui->addBtn->SetState("normal","hover", "press");
    ui->addBtn->hide();
    connect(ui->addBtn, &StateBtn::clicked, [this]() { emit this->sig_auth_friend(m_pApplyInfo);});
}

ApplyFriendItem::~ApplyFriendItem()
{
    delete ui;
}

void ApplyFriendItem::SetInfo(std::shared_ptr<ApplyInfo> pInfo)
{
    m_pApplyInfo = pInfo;
    QPixmap pixMap(m_pApplyInfo->m_strIcon);
    ui->iconLabel->setPixmap(pixMap.scaled(ui->iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->iconLabel->setScaledContents(true);

    ui->userNameLabel->setText(m_pApplyInfo->m_strName);
    ui->userChatLabel->setText(m_pApplyInfo->m_strDesc);
}

void ApplyFriendItem::ShowAddBtn(bool bShow)
{
    if (bShow)
    {
        ui->addBtn->show();
        ui->alreadyAddLabel->hide();
        m_bAdded = true;
    }
    else
    {
        ui->addBtn->hide();
        ui->alreadyAddLabel->show();
        m_bAdded = false;
    }
}

QSize ApplyFriendItem::sizeHint() const
{
    return QSize(250, 80);
}

int ApplyFriendItem::GetUid()
{
    return m_pApplyInfo->m_nUID;
}
