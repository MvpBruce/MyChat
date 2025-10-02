#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H

#include <QTextEdit>
#include <QWidget>
#include "global/global.h"
#include <QKeyEvent>

class MessageTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MessageTextEdit(QWidget *parent = nullptr);
    QVector<MsgInfo>& GetMessages();

signals:
    void send();

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    void InsertMsgList(QVector<MsgInfo> &list, QString type, QString text, QPixmap pix);

private:
    QVector<MsgInfo> m_msgList;
    QVector<MsgInfo> m_getMsgList;
};

#endif // MESSAGETEXTEDIT_H
