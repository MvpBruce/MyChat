#include "chatview.h"
#include <QVBoxLayout>
#include <QScrollBar>
#include <QTimer>

ChatView::ChatView(QWidget *parent)
    : QWidget{parent}, m_bAppended(false)
{
    QVBoxLayout * pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setObjectName("chat_area");
    pMainLayout->addWidget(m_pScrollArea);

    QWidget* w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);

    QVBoxLayout* pVLayout = new QVBoxLayout();
    pVLayout->addWidget(new QWidget(), 10000);
    w->setLayout(pVLayout);
    m_pScrollArea->setWidget(w);

    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *pVScrollBar = m_pScrollArea->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollChanged);

    QHBoxLayout* pHLayout = new QHBoxLayout();
    pHLayout->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout->setContentsMargins(0, 0, 0, 0);
    m_pScrollArea->setLayout(pHLayout);
    pVScrollBar->setHidden(true);

    m_pScrollArea->setWidgetResizable(true);
    m_pScrollArea->installEventFilter(this);
}

void ChatView::appendChild(QWidget *widget)
{
    QVBoxLayout* pV = qobject_cast<QVBoxLayout*>(m_pScrollArea->widget()->layout());
    pV->insertWidget(pV->count() - 1, widget);
    m_bAppended = true;
}

bool ChatView::eventFilter(QObject *o, QEvent *e)
{
    /*if(e->type() == QEvent::Resize && o == )
    {

    }
    else */if(e->type() == QEvent::Enter && o == m_pScrollArea)
    {
        m_pScrollArea->verticalScrollBar()->setHidden(m_pScrollArea->verticalScrollBar()->maximum() == 0);
    }
    else if(e->type() == QEvent::Leave && o == m_pScrollArea)
    {
        m_pScrollArea->verticalScrollBar()->setHidden(true);
    }

    return QWidget::eventFilter(o, e);
}

void ChatView::onVScrollChanged(int min, int max)
{
    if (m_bAppended)
    {
        QScrollBar* pScrollBar = m_pScrollArea->verticalScrollBar();
        pScrollBar->setSliderPosition(pScrollBar->maximum());
        QTimer::singleShot(500, [this](){
            m_bAppended = false;
        });
    }
}
