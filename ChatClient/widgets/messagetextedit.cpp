#include "messagetextedit.h"

MessageTextEdit::MessageTextEdit(QWidget *parent)
    :QTextEdit(parent)
{
    setMaximumHeight(50);
}

QVector<MsgInfo>& MessageTextEdit::GetMessages()
{
    m_getMsgList.clear();
    QString doc = this->document()->toPlainText();
    QString text = "";
    int nIndexUrl = 0;
    int nCount = m_msgList.size();
    for (int i = 0; i < doc.size(); i++)
    {
        if (doc[i] == QChar::ObjectReplacementCharacter)
        {
            if (!text.isEmpty())
            {
                QPixmap pix;
                InsertMsgList(m_getMsgList, "text", text, pix);
                text.clear();
            }

            while (nIndexUrl < nCount)
            {
                MsgInfo msgInfo = m_msgList[nIndexUrl];
                if (this->document()->toHtml().contains(msgInfo.content, Qt::CaseSensitive))
                {
                    nIndexUrl++;
                    m_getMsgList.append(msgInfo);
                    break;
                }

                nIndexUrl++;
            }
        }
        else
        {
            text.append(doc[i]);
        }
    }

    if (!text.isEmpty())
    {
        QPixmap pix;
        InsertMsgList(m_getMsgList, "text", text, pix);
        text.clear();
    }
    m_msgList.clear();
    this->clear();
    return m_getMsgList;
}

void MessageTextEdit::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) && (e->modifiers() & Qt::ShiftModifier))
    {
        emit send();
        return;
    }

    QTextEdit::keyPressEvent(e);
}

void MessageTextEdit::InsertMsgList(QVector<MsgInfo> &list, QString type, QString text, QPixmap pix)
{
    MsgInfo msg;
    msg.msgType = type;
    msg.content = text;
    msg.pixMap = pix;
    list.append(msg);
}
