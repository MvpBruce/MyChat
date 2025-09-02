#ifndef USERFOUNDDLG_H
#define USERFOUNDDLG_H

#include <QDialog>

namespace Ui {
class UserFoundDlg;
}

class UserFoundDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UserFoundDlg(QWidget *parent = nullptr);
    ~UserFoundDlg();

private:
    Ui::UserFoundDlg *ui;
};

#endif // USERFOUNDDLG_H
