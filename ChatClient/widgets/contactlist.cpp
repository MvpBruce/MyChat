#include "contactlist.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include "usercontactitem.h"
#include <QRandomGenerator>
#include "core/TcpMgr.h"
#include "groupitem.h"

ContactList::ContactList(QWidget* parent)
    : QListWidget(parent), m_pAddFriendItem(nullptr), m_pGroupItem(nullptr)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->viewport()->installEventFilter(this);

    //todo, need to get data from server
    AddContactList();

    connect(this, &QListWidget::itemClicked, this, &ContactList::slot_item_clicked);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_add_auth_friend, this, &ContactList::slot_add_auth_friend);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ContactList::slot_auth_rsp);
}

void ContactList::ShowRedPoint(bool bShow)
{
    m_pAddFriendItem->ShowRedPoint(bShow);
}

bool ContactList::eventFilter(QObject *object, QEvent *event)
{
    if (object == this->viewport())
    {
        if (event->type() == QEvent::Enter)
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        else if (event->type() == QEvent::Leave)
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    if (object == this->viewport() && event->type() == QEvent::Wheel)
    {
        QWheelEvent* pWheel = static_cast<QWheelEvent*>(event);
        int nSteps = pWheel->angleDelta().y() / 90;
        QScrollBar* pScrollBar = this->verticalScrollBar();
        pScrollBar->setValue(pScrollBar->value() - nSteps);
        int nMax = pScrollBar->maximum();
        int nCurrent = pScrollBar->value();
        if (nCurrent >= nMax)
        {
            emit sig_load_contacts();
        }

        return true;
    }

    return QListWidget::eventFilter(object, event);
}

void ContactList::AddContactList()
{
    //add addfriend item
    m_pAddFriendItem = new UserContactItem();
    m_pAddFriendItem->setObjectName("addFriendItem");
    m_pAddFriendItem->SetInfo(0, "New friend", ":/assets/image/add_friend.png");
    m_pAddFriendItem->SetItemType(ListItemType::APPLY_FRIEND_ITEM);
    auto pAddItem = new QListWidgetItem();
    pAddItem->setSizeHint(m_pAddFriendItem->sizeHint());
    this->addItem(pAddItem);
    this->setItemWidget(pAddItem, m_pAddFriendItem);
    this->setCurrentItem(pAddItem);

    //add group item
    m_pGroupItem = new QListWidgetItem();
    auto pGroupItem = new GroupItem();
    m_pGroupItem->setSizeHint(pGroupItem->sizeHint());
    this->addItem(m_pGroupItem);
    this->setItemWidget(m_pGroupItem, pGroupItem);
    m_pGroupItem->setFlags(m_pGroupItem->flags() &~ Qt::ItemIsSelectable);

    //add contacts, todo, need to get data from server
    for(int i = 0; i < 13; i++)
    {
        int randomValue = QRandomGenerator::global()->bounded(100);
        //int str_i = randomValue % strs.size();
        int head_i = randomValue % heads.size();
        int name_i = randomValue % names.size();
        auto* pContactItem = new UserContactItem();
        pContactItem->SetInfo(0, names[name_i], heads[head_i]);
        QListWidgetItem* pItem = new QListWidgetItem();
        pItem->setSizeHint(pContactItem->sizeHint());
        this->addItem(pItem);
        this->setItemWidget(pItem, pContactItem);
    }
}

void ContactList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget* pWidget = this->itemWidget(item);
    if (!pWidget)
    {
        qDebug() << "Item is null";
        return;
    }

    ListItemBase* pBase = static_cast<ListItemBase*>(pWidget);
    if (!pBase)
    {
        qDebug() << "BaseItem is null";
        return;
    }

    auto nType = pBase->GetItemType();
    if (nType == ListItemType::INVALID_ITEM)
    {
        qDebug() << "Invalid Item";
        return;
    }

    if (nType == ListItemType::APPLY_FRIEND_ITEM)
    {
        qDebug() << "Invalid Item";
        return;
    }

    if (nType == ListItemType::CONTACT_USER_ITEM)
    {
        auto contactItem = qobject_cast<UserContactItem*>(pBase);
        auto pUserInfo = contactItem->GetInfo();
        emit sig_switch_friend_info_page(pUserInfo);
        return;
    }

    //todo
    //1.friend info page
    //2.other, likes apply page
}

void ContactList::slot_add_auth_friend(std::shared_ptr<AuthInfo> pInfo)
{
    //todo, check if they are aleady friend

    //insert after group item
    auto pUserItem = new UserContactItem();
    pUserItem->SetInfo(pInfo);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(pUserItem->sizeHint());

    int nIndex = this->row(m_pGroupItem);
    this->insertItem(nIndex + 1, pItem);
    this->setItemWidget(pItem, pUserItem);

}

void ContactList::slot_auth_rsp(std::shared_ptr<AuthRsp> pInfo)
{
    //todo, check if they are aleady friend
    auto pUserItem = new UserContactItem();
    pUserItem->SetInfo(pInfo->m_nUID, pInfo->m_strName, pInfo->m_strIcon);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(pUserItem->sizeHint());

    int nIndex = this->row(m_pGroupItem);
    this->insertItem(nIndex + 1, pItem);
    this->setItemWidget(pItem, pUserItem);
}
