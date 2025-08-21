#include "chatmsgbaseitem.h"
#include <QGridLayout>
#include <QSpacerItem>

ChatMsgBaseItem::ChatMsgBaseItem(Role role, QWidget *parent)
    : QWidget{parent}
{
    m_pName = new QLabel();
    m_pName->setObjectName("msg_user_name");
    m_pName->setFixedHeight(20);
    m_pIcon = new QLabel();
    m_pIcon->setScaledContents(true);
    m_pIcon->setFixedSize(40, 40);
    m_pBubble = new QWidget();

    QGridLayout* pGridLayout = new QGridLayout();
    pGridLayout->setSpacing(3);
    pGridLayout->setContentsMargins(3, 3, 3, 3);
    QSpacerItem* pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if (role == Role::Self)
    {
        m_pName->setContentsMargins(0, 0, 8, 0);
        m_pName->setAlignment(Qt::AlignRight);
        pGridLayout->addWidget(m_pName, 0, 1, 1, 1);
        pGridLayout->addWidget(m_pIcon, 0, 2, 2, 1, Qt::AlignTop);
        pGridLayout->addWidget(m_pBubble, 1, 1, 1, 1);
        pGridLayout->setColumnStretch(0, 2);
        pGridLayout->setColumnStretch(1, 3);
    }
    else
    {
        m_pName->setContentsMargins(8, 0, 0, 0);
        m_pName->setAlignment(Qt::AlignLeft);
        pGridLayout->addWidget(m_pIcon, 0, 0, 1, 1);
        pGridLayout->addWidget(m_pName, 0, 1, 1, 1);
        pGridLayout->addWidget(m_pBubble, 1, 1, 1, 1);
        pGridLayout->addItem(pSpacer, 2, 2, 1, 1);
        pGridLayout->setColumnStretch(1, 3);
        pGridLayout->setColumnStretch(2, 2);
    }

    this->setLayout(pGridLayout);
}

void ChatMsgBaseItem::setNmae(const QString &name)
{
    m_pName->setText(name);
}

void ChatMsgBaseItem::setIcon(const QPixmap &icon)
{
    m_pIcon->setPixmap(icon);
}

void ChatMsgBaseItem::setWidget(QWidget* widget)
{
    QGridLayout* pGLayout = (qobject_cast<QGridLayout *>)(this->layout());
    pGLayout->replaceWidget(m_pBubble, widget);
    delete m_pBubble;
    m_pBubble = widget;
}
