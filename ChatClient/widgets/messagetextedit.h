#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H

#include <QTextEdit>
#include <QWidget>
#include "global/global.h"

class MessageTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MessageTextEdit(QWidget *parent = nullptr);
    QVector<MsgInfo> GetMessages();
};

#endif // MESSAGETEXTEDIT_H
