#include "contactlist.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include "listitembase.h"

ContactList::ContactList(QWidget* parent)
    : QListWidget(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->viewport()->installEventFilter(this);

    //todo, need to get data from server
    AddContactList();

    connect(this, &QListWidget::itemClicked, this, &ContactList::slot_item_clicked);
}

void ContactList::ShowRedPoint(bool bShow)
{

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

    //todo
    //1.friend info page
    //2.other, likes apply page

}
