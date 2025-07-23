#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include "global/global.h"

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();

signals:
    void switchToLogin();

private slots:
    void on_reset_pwd_btn_clicked();
    void on_cancel_btn_clicked();
    void on_get_code_btn_clicked();

private:
    bool checkValidEmail();
    bool checkValidUser();
    bool checkValidPassWord();
    bool checkValidVerifyCode();

    void initHandlers();
    void showTip(QString str, bool isErrorr);

private:
    Ui::ResetDialog *ui;
    QMap<RequstID, std::function<void(const QJsonObject&)>> m_handlers;
};

#endif // RESETDIALOG_H
