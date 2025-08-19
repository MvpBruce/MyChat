#include "userchatlist.h"

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


    //Load more users when mouse wheel scrolling
    return QListWidget::eventFilter(obj, event);
}
