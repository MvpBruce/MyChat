#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

    void addChatUserList();

public slots:
    void slot_load_more_users();

private:
    Ui::ChatDialog *ui;
    bool m_bLoading;
};

#endif // CHATDIALOG_H
