#include "userchatlist.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>

UserChatList::UserChatList(QWidget* parent)
    : QListWidget(parent)
{
    //Never show scrollbar
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Install event filter
    viewport()->installEventFilter(this);
}

bool UserChatList::eventFilter(QObject* obj, QEvent* event)
{
    //Show scrollbar when mouse entering user list
    if (obj == this->viewport())
    {
        if (event->type() == QEvent::Enter)
            setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        else if (event->type() == QEvent::Leave)
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    //Load more users when mouse wheel scrolling
    if (obj == this->viewport() && event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int nSteps = wheelEvent->angleDelta().y() / 100;
        QScrollBar* pScrollBar = this->verticalScrollBar();
        pScrollBar->setValue(pScrollBar->value() - nSteps);
        int nMaxValue = pScrollBar->maximum();
        int nCurValue = pScrollBar->value();
        if (nMaxValue <= nCurValue)
        {
            emit sig_load_more_users();
        }

        return true;
    }
    return QListWidget::eventFilter(obj, event);
}
