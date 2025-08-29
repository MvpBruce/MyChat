#include "searchlist.h"
#include <core/TcpMgr.h>
#include "adduseritem.h"

SearchList::SearchList(QWidget *parent)
    : QListWidget(parent), m_pFindDlg(nullptr), m_pSearchEdit(nullptr)
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

}

void SearchList::SetSearchEdit(QWidget *edit)
{

}

bool SearchList::eventFilter(QObject *object, QEvent *event)
{
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

}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> pSearchInfo)
{

}
