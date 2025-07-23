#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global/global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

signals:
    void switchToLogin();

private slots:
    void on_btn_GetCode_clicked();
    void slot_register_finished(RequstID id, QString res, ErrorCodes ec);
    void on_btn_ok_clicked();
    void on_btn_Cancel_clicked();

private:
    void initHandlers();

    bool checkValidEmail();
    bool checkValidUser();
    bool checkValidPassWord();
    bool checkValidComfirPwd();
    bool checkValidVerifyCode();

private:
    void showTip(QString str, bool isError);

private:
    Ui::RegisterDialog *ui;
    QMap<RequstID, std::function<void(const QJsonObject&)>> m_handlers;
};

#endif // REGISTERDIALOG_H
