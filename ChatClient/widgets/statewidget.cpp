#include "statewidget.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>

StateWidget::StateWidget(QWidget *parent)
    : QWidget{parent}, m_curState(WidgetState::Normal)
{
    setCursor(Qt::PointingHandCursor);
    AddRedPoint();
}

void StateWidget::SetState(QString normal, QString hover, QString press, QString select, QString selectHover, QString selectPress)
{
    m_strNormal = normal;
    m_strNormalHover = hover;
    m_strNormalPress = press;

    m_strSelected = select;
    m_strSelectedHover = selectHover;
    m_strSelectedPress = selectPress;

    setProperty("state", normal);
    resetStyle(this);
}

WidgetState StateWidget::GetState()
{
    return m_curState;
}

void StateWidget::ClearState()
{
    m_curState = WidgetState::Normal;
    setProperty("state", m_strNormal);
    resetStyle(this);
    update();
}

void StateWidget::SetSelected(bool bSelected)
{
    if (bSelected)
    {
        m_curState = WidgetState::Selected;
        setProperty("state", m_strSelected);
        resetStyle(this);
        update();
        return;
    }

    m_curState = WidgetState::Normal;
    setProperty("state", m_strNormal);
    resetStyle(this);
    update();
}

void StateWidget::AddRedPoint()
{
    m_pReadPopint = new QLabel();
    m_pReadPopint->setObjectName("redPoint");
    m_pReadPopint->setAlignment(Qt::AlignCenter);
    m_pReadPopint->setVisible(false);
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->addWidget(m_pReadPopint);
    pLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(pLayout);
}

void StateWidget::ShowRedPoint(bool bShow)
{
    m_pReadPopint->setVisible(bShow);
}

void StateWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStyleOption opt;
    opt.initFrom(this);
    QPainter pt(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &pt, this);;
}

void StateWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_curState == WidgetState::Selected)
        {
            QWidget::mousePressEvent(event);
            return;
        }

        if (m_curState == WidgetState::Normal)
        {
            m_curState = WidgetState::Selected;
            setProperty("state", m_strSelectedPress);
            resetStyle(this);
            update();
        }

        return;
    }

    QWidget::mousePressEvent(event);
}

void StateWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_curState == WidgetState::Normal)
        {
            setProperty("state", m_strNormalHover);
            resetStyle(this);
            update();
        }
        else
        {
            setProperty("state", m_strSelectedHover);
            resetStyle(this);
            update();
        }

        emit clicked();
        return;
    }

    QWidget::mouseReleaseEvent(event);
}

void StateWidget::enterEvent(QEnterEvent *event)
{
    if (m_curState == WidgetState::Normal)
    {
        setProperty("state", m_strNormalHover);
        resetStyle(this);
        update();
    }
    else
    {
        setProperty("state", m_strSelectedHover);
        resetStyle(this);
        update();
    }

    QWidget::enterEvent(event);
}

void StateWidget::leaveEvent(QEvent *event)
{
    if (m_curState == WidgetState::Normal)
    {
        setProperty("state", m_strNormal);
        resetStyle(this);
        update();
    }
    else
    {
        setProperty("state", m_strSelected);
        resetStyle(this);
        update();
    }

    QWidget::leaveEvent(event);
}
