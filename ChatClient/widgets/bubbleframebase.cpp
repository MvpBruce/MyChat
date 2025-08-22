#include "bubbleframebase.h"
#include <QPainter>

int BubbleFrameBase::s_nWidth = 8;
BubbleFrameBase::BubbleFrameBase(Role role, QWidget* parent)
    :QFrame(parent), m_Role(role), m_nMargin(3)
{
    m_pHLayout = new QHBoxLayout();
    // if (m_Role == Role::Self)
    //     m_pHLayout->setContentsMargins(m_nMargin, m_nMargin, m_nMargin, m_nMargin);
    // else
    //     m_pHLayout->setContentsMargins(m_nMargin, m_nMargin, m_nMargin, m_nMargin);

    this->setLayout(m_pHLayout);
}

void BubbleFrameBase::setMargin(int nMargin)
{
    m_nMargin = nMargin;
}

void BubbleFrameBase::setWidget(QWidget *widget)
{
    if (m_pHLayout->count() > 0)
        return;
    else
        m_pHLayout->addWidget(widget);
}

void BubbleFrameBase::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    if (m_Role == Role::Self)
    {
        QColor bgColor(149, 236, 105);
        painter.setBrush(QBrush(bgColor));
        //Draw rectangle
        QRect bgRect(0, 0, this->width(), this->height());
        painter.drawRoundedRect(bgRect, 5, 5);
        //Draw corner triangle
        //painter.drawPolygon();
    }
    else
    {

    }

    return QFrame::paintEvent(e);
}
