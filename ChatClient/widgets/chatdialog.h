#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global/global.h"

namespace Ui {
class ChatDialog;
}

class StateWidget;

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

    void AddChatUserList();
    void ClearSideBarState(StateWidget* pWidget);

public slots:
    void slot_load_more_users();
    void slot_clicked_chat();
    void slot_clicked_contact();
    void slot_text_changed(const QString& text);

private:
    void SwitchMode(bool bSearch = false);

private:
    Ui::ChatDialog *ui;
    bool m_bLoading;
    QList<StateWidget*> m_listWidget;
    SideBarMode m_Mode;
    SideBarMode m_State;
};

#endif // CHATDIALOG_H
