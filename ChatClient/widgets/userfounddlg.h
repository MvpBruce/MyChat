#ifndef USERFOUNDDLG_H
#define USERFOUNDDLG_H

#include <QDialog>

namespace Ui {
class UserFoundDlg;
}

class SearchInfo;
class UserFoundDlg : public QDialog
{
    Q_OBJECT

public:
    explicit UserFoundDlg(QWidget *parent = nullptr);
    ~UserFoundDlg();
    void SetSearchInfo(std::shared_ptr<SearchInfo> pInfo);

private slots:
    void on_add_to_contact_clicked();

private:
    Ui::UserFoundDlg *ui;
    QWidget* m_pParent;
    std::shared_ptr<SearchInfo> m_pSearchInfo;
};

#endif // USERFOUNDDLG_H
