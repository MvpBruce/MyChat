#include "statebtn.h"
#include "global/global.h"

StateBtn::StateBtn(QWidget* parent): QPushButton(parent)
{

}

StateBtn::~StateBtn()
{

}

void StateBtn::SetState(QString normal, QString hover, QString press)
{
    m_strNormal = normal;
    m_strHover = hover;
    m_strPress = press;
    setProperty("state", normal);
    resetStyle(this);
    update();
}

void StateBtn::enterEvent(QEnterEvent *event)
{
    setProperty("state",m_strHover);
    resetStyle(this);
    update();
    QPushButton::enterEvent(event);
}

void StateBtn::mousePressEvent(QMouseEvent *event)
{
    setProperty("state",m_strPress);
    resetStyle(this);
    update();
    QPushButton::mousePressEvent(event);
}

void StateBtn::mouseReleaseEvent(QMouseEvent *event)
{
    setProperty("state",m_strHover);
    resetStyle(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}

void StateBtn::leaveEvent(QEvent *event)
{
    setProperty("state",m_strNormal);
    resetStyle(this);
    update();
    QPushButton::leaveEvent(event);
}
