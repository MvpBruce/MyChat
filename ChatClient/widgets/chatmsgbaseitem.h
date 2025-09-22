#ifndef CHATMSGBASEITEM_H
#define CHATMSGBASEITEM_H

#include <QWidget>
#include "global/global.h"
#include <QLabel>

class ChatMsgBaseItem : public QWidget
{
    Q_OBJECT
public:
    explicit ChatMsgBaseItem(Role role, QWidget *parent = nullptr);
    void setName(const QString &name);
    void setIcon(const QPixmap &icon);
    void setWidget(QWidget* widget);

private:
    QLabel* m_pName;
    QLabel* m_pIcon;
    QWidget* m_pBubble;
};

#endif // CHATMSGBASEITEM_H
