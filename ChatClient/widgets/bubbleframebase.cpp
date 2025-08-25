#include "bubbleframebase.h"
#include <QPainter>

int BubbleFrameBase::s_nWidth = 8;
BubbleFrameBase::BubbleFrameBase(Role role, QWidget* parent)
    :QFrame(parent), m_Role(role), m_nMargin(3)
{
    m_pHLayout = new QHBoxLayout();
    // if (m_Role == Role::Self)
    //     m_pHLayout->setContentsMargins(0, m_nMargin, 0, m_nMargin);
    // else
    //     m_pHLayout->setContentsMargins(0, m_nMargin, 0, m_nMargin);

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
        QRect bgRect(0, 0, this->width() - s_nWidth, this->height());
        painter.drawRoundedRect(bgRect, 5, 5);
        //Draw right triangle
        QPointF points[3] = {
            QPointF(bgRect.x() + bgRect.width(), 12),
            QPointF(bgRect.x() + bgRect.width(), s_nWidth + 12),
            QPointF(bgRect.x() + bgRect.width() + s_nWidth, 12)
        };
        painter.drawPolygon(points, 3);
    }
    else
    {
        QColor bgColor(255, 255, 255);
        painter.setBrush(QBrush(bgColor));
        QRect bgRect(s_nWidth, 0, this->width(), this->height());
        painter.drawRoundedRect(bgRect, 5, 5);
        //Draw left triangle later
        QPointF points[3] = {
            QPointF(bgRect.x(), 12),
            QPointF(bgRect.x(), 12 + s_nWidth),
            QPointF(bgRect.x() - s_nWidth, 12)
        };
        painter.drawPolygon(points, 3);
    }

    return QFrame::paintEvent(e);
}
