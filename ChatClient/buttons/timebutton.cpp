#include "timebutton.h"
#include <QMouseEvent>

TimeButton::TimeButton(QWidget *parent)
    : m_nCount(0)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, [this]() {
        m_nCount++;
        if (m_nCount >= 10)
        {
            m_pTimer->stop();
            m_nCount = 0;
            this->setText(tr("Get code"));
            this->setEnabled(true);
            return;
        }

        this->setText(QString::number(m_nCount));
    });
}

TimeButton::~TimeButton()
{
    m_pTimer->stop();
}

void TimeButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        setEnabled(false);
        setText(QString::number(m_nCount));
        m_pTimer->start(1000);
        emit clicked();
    }

    QPushButton::mouseReleaseEvent(e);
}
