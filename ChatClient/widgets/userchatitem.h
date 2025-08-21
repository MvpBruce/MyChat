#ifndef USERCHATITEM_H
#define USERCHATITEM_H

#include <QWidget>
#include "listitembase.h"

namespace Ui {
class UserChatItem;
}

class UserChatItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit UserChatItem(QWidget *parent = nullptr);
    ~UserChatItem();

    QSize sizeHint() const override;

    void SetInfo(QString name, QString head, QString msg);
private:
    Ui::UserChatItem *ui;
    QString m_Name;
    QString m_Head;
    QString m_Msg;
};

#endif // USERCHATITEM_H
