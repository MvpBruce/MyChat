#ifndef AUTHENFRIENDDLG_H
#define AUTHENFRIENDDLG_H

#include <QDialog>
#include "core/userdata.h"

namespace Ui {
class AuthenFriendDlg;
}

class AuthenFriendDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AuthenFriendDlg(QWidget *parent = nullptr);
    ~AuthenFriendDlg();

    void SetApplyInfo(std::shared_ptr<ApplyInfo> pApplyInfo);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void slot_apply_ok();
    void slot_apply_cancel();

private:
    Ui::AuthenFriendDlg *ui;
    std::shared_ptr<ApplyInfo> m_pApplyInfo;
};

#endif // AUTHENFRIENDDLG_H
