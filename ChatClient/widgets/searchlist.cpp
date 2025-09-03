#include "searchlist.h"
#include <core/TcpMgr.h>
#include "adduseritem.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include "userfounddlg.h"
#include "core/userdata.h"

SearchList::SearchList(QWidget *parent)
    : QListWidget(parent), m_pFindDlg(nullptr), m_pSearchEdit(nullptr), m_bPending(false)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);
    connect(this, &QListWidget::itemClicked, this, &SearchList::slot_item_clicked);
    AddTipItem();
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_user_search, this, &SearchList::slot_user_search);
}

void SearchList::CloseFindDlg()
{
    if (m_pFindDlg)
    {
        m_pFindDlg->hide();
        m_pFindDlg = nullptr;
    }
}

void SearchList::SetSearchEdit(QWidget *edit)
{

}

bool SearchList::eventFilter(QObject *object, QEvent *event)
{
    if (object == this->viewport())
    {
        if (event->type() == QEvent::Enter)
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        else if (event->type() ==QEvent::Leave)
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    if (object == this->viewport() && event->type() == QEvent::Wheel)
    {
        QWheelEvent* pWheel =  static_cast<QWheelEvent*>(event);
        int nSteps = pWheel->angleDelta().y() / 90;
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - nSteps);
        return true;
    }

    return QListWidget::eventFilter(object, event);
}

void SearchList::AddTipItem()
{
    auto* pInvalidItem = new QWidget();
    pInvalidItem->setObjectName("invalid_item");
    QListWidgetItem* pTmp = new QListWidgetItem();
    pTmp->setSizeHint(QSize(250, 10));
    this->addItem(pTmp);
    this->setItemWidget(pTmp, pInvalidItem);
    pTmp->setFlags(pTmp->flags() &~ Qt::ItemIsSelectable);

    auto* pUserItem = new AddUserItem();
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(pUserItem->sizeHint());
    this->addItem(pItem);
    this->setItemWidget(pItem, pUserItem);
}

void SearchList::WaitPending(bool bPending)
{

}

void SearchList::slot_item_clicked(QListWidgetItem *pItem)
{
    QWidget* pWidget = this->itemWidget(pItem);
    if (!pWidget)
        return;

    ListItemBase* pBaseItem = qobject_cast<ListItemBase*>(pWidget);
    if (!pBaseItem)
        return;

    auto nType = pBaseItem->GetItemType();
    if (nType == ListItemType::INVALID_ITEM)
        return;

    if (nType == ListItemType::ADD_USER_TIP_ITEM)
    {
        m_pFindDlg = std::make_shared<UserFoundDlg>(this);
        auto info = std::make_shared<SearchInfo>(0, "test", "test", "hello world", 0);
        (dynamic_cast<UserFoundDlg*>(m_pFindDlg.get()))->SetSearchInfo(info);
        m_pFindDlg->show();
    }

    //CloseFindDlg();
}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> pSearchInfo)
{

}
