#include "messagetextedit.h"

MessageTextEdit::MessageTextEdit(QWidget *parent)
    :QTextEdit(parent)
{
    setMaximumHeight(50);
}

QVector<MsgInfo> MessageTextEdit::GetMessages()
{
    return QVector<MsgInfo>();
}
