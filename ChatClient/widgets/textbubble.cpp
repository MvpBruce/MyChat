#include "textbubble.h"
#include <QTextBlock>

TextBubble::TextBubble(Role role, const QString& text, QWidget* parent)
    : BubbleFrameBase(role, parent)
{
    m_pTextEdit = new QTextEdit();
    m_pTextEdit->setReadOnly(true);
    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->installEventFilter(this);
    QFont font("Microsoft YaHei");
    font.setPointSize(12);
    m_pTextEdit->setFont(font);
    setChatText(text);
    setWidget(m_pTextEdit);
    m_pTextEdit->setStyleSheet("QTextEdit{background:transparent;border:none}");
}

bool TextBubble::eventFilter(QObject *o, QEvent *e)
{
    if(m_pTextEdit == o && e->type() == QEvent::Paint)
        adjustTextHeight();

    return BubbleFrameBase::eventFilter(o, e);
}

void TextBubble::setChatText(const QString &text)
{
    m_pTextEdit->setPlainText(text);
    QFontMetricsF fontM(m_pTextEdit->font());
    QTextDocument* pDoc = m_pTextEdit->document();
    int nMaxWidth = 0;
    for (auto it = pDoc->begin(); it != pDoc->end(); it = it.next())
    {
        QString txt = it.text();

        //int nTextWidth = (int)fontM.boundingRect(txt).width();
        QSizeF fSize = fontM.size(Qt::TextFlag::TextSingleLine, txt);
        int nTextWidth = fSize.width();
        nMaxWidth = std::max(nMaxWidth, nTextWidth);
    }

    qreal docMargin = m_pTextEdit->document()->documentMargin();
    int nLeft = layout()->contentsMargins().left();
    int nRight = layout()->contentsMargins().right();
    setMaximumWidth(nMaxWidth + docMargin * 2 + (nLeft + nRight));
}

void TextBubble::adjustTextHeight()
{
    qreal doc_margin = m_pTextEdit->document()->documentMargin();
    QTextDocument *doc = m_pTextEdit->document();
    qreal text_height = 0;
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
        QTextLayout *pLayout = it.layout();
        QRectF text_rect = pLayout->boundingRect();
        text_height += text_rect.height();
    }

    int vMargin = this->layout()->contentsMargins().top();
    setFixedHeight(text_height + doc_margin *2 + vMargin*2 );
}
