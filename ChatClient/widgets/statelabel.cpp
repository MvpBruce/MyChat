#include "statelabel.h"
#include <QMouseEvent>

StateLabel::StateLabel(QWidget* parent)
    :QLabel(parent), m_curState(ClickState::Normal)
{
    setCursor(Qt::PointingHandCursor);
}

void StateLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        if(m_curState == ClickState::Normal)
        {
            m_curState = ClickState::Selected;
            setProperty("state", m_strSelectedPress);
            resetStyle(this);
            update();
        }
        else
        {
            m_curState = ClickState::Normal;
            setProperty("state", m_strNormalPress);
            resetStyle(this);
            update();
        }

        return;
    }

    QLabel::mousePressEvent(ev);
}

void StateLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        if(m_curState == ClickState::Normal)
        {
            setProperty("state",m_strNormalHover);
            resetStyle(this);
            update();
        }
        else
        {
            setProperty("state",m_strSelectedHover);
            resetStyle(this);
            update();
        }

        emit clicked(this->text(), m_curState);
        return;
    }

    QLabel::mouseReleaseEvent(ev);
}

void StateLabel::enterEvent(QEnterEvent *event)
{
    if(m_curState == ClickState::Normal)
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

    QLabel::enterEvent(event);
}

void StateLabel::leaveEvent(QEvent *event)
{
    if(m_curState == ClickState::Normal)
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

    QLabel::leaveEvent(event);
}

void StateLabel::SetState(QString normal, QString hover, QString press, QString select, QString selectHover, QString selectPress)
{
    m_strNormal = normal;
    m_strNormalHover = hover;
    m_strNormalPress = press;

    m_strSelected = select;
    m_strSelectedHover = selectHover;
    m_strSelectedPress = selectPress;

    setProperty("state", m_strNormal);
    resetStyle(this);
}

ClickState StateLabel::GetCurState()
{
    return m_curState;
}

void StateLabel::SetCurState(ClickState state)
{
    m_curState = state;
    if (m_curState == ClickState::Normal)
        setProperty("state", m_strNormal);
    else if (m_curState == ClickState::Selected)
        setProperty("state", m_strSelected);

    resetStyle(this);
}

void StateLabel::ResetNormalState()
{
    m_curState = ClickState::Normal;
    setProperty("state", m_strNormal);
    resetStyle(this);
}
