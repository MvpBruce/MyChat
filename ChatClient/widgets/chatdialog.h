#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>

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

private:
    Ui::ChatDialog *ui;
    bool m_bLoading;
    QList<StateWidget*> m_listWidget;
};

#endif // CHATDIALOG_H
