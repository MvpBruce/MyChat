#include "bubbleframebase.h"
#include <QPainter>

int BubbleFrameBase::s_nWidth = 8;
BubbleFrameBase::BubbleFrameBase(Role role, QWidget* parent)
    :QFrame(parent), m_Role(role), m_nMargin(3)
{

}

void BubbleFrameBase::setMargin(int nMargin)
{
    m_nMargin = nMargin;
    m_pHLayout = new QHBoxLayout();
    if (m_Role == Role::Self)
        m_pHLayout->setContentsMargins(m_nMargin, m_nMargin, s_nWidth + m_nMargin, m_nMargin);
    else
        m_pHLayout->setContentsMargins(s_nWidth + m_nMargin, m_nMargin, m_nMargin, m_nMargin);

    this->setLayout(m_pHLayout);
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
    if (m_Role == Role::Self)
    {

    }
    else
    {

    }
    return QFrame::paintEvent(e);
}
