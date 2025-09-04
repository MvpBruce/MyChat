#include "contactlist.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include "usercontactitem.h"
#include <QRandomGenerator>

const std::vector<QString>  strs ={"hello world !",
                                   "nice to meet u",
                                   "New year，new life",
                                   "You have to love yourself",
                                   "My love is written in the wind ever since the whole world is you"};

const std::vector<QString> names = {
    "HanMeiMei",
    "Lily",
    "Ben",
    "Androw",
    "Max",
    "Summer",
    "Candy",
    "Hunter"
};

const std::vector<QString> heads = {
    ":/assets/image/head_1.jpg",
    ":/assets/image/head_2.jpg",
    ":/assets/image/head_3.jpg",
    ":/assets/image/head_4.jpg",
    ":/assets/image/head_5.jpg"
};

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
    //add contact
    for(int i = 0; i < 13; i++)
    {
        int randomValue = QRandomGenerator::global()->bounded(100);
        int str_i = randomValue % strs.size();
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

    //todo
    //1.friend info page
    //2.other, likes apply page

}
