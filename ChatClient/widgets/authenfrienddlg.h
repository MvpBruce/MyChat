#ifndef AUTHENFRIENDDLG_H
#define AUTHENFRIENDDLG_H

#include <QDialog>

namespace Ui {
class AuthenFriendDlg;
}

class AuthenFriendDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AuthenFriendDlg(QWidget *parent = nullptr);
    ~AuthenFriendDlg();

private:
    Ui::AuthenFriendDlg *ui;
};

#endif // AUTHENFRIENDDLG_H
