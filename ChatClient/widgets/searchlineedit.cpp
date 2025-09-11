#include "searchlineedit.h"

SearchLineEdit::SearchLineEdit(QWidget* parent)
    :QLineEdit(parent), m_MaxLen(256)
{
    connect(this, &QLineEdit::textChanged, this, &SearchLineEdit::limitTextLength);
}

void SearchLineEdit::setMaxLength(int nMaxLen)
{
    m_MaxLen = nMaxLen;
}

void SearchLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    emit sig_foucus_out();
}

void SearchLineEdit::limitTextLength(QString text)
{
    if (text.length() <= 0)
        return;

    QByteArray byteArr = text.toUtf8();
    if (byteArr.size() > m_MaxLen)
    {
        byteArr = byteArr.left(m_MaxLen);
        this->setText(QString::fromUtf8(byteArr));
    }
}
