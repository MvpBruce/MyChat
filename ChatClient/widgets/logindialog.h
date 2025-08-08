#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global/global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    //UI swtich
    void switchToRegister();
    void switchToResetPwd();

    //TcpMgr
    void sig_connect_tcp(ServerInfo info);

private slots:
    void on_forget_btn_clicked();
    void on_pushButton_clicked();
    void slot_user_login_finish(RequstID id, QString res, ErrorCodes ec);
    void slot_tcp_connect_finished(bool bSuccess);

private:
    bool checkValidUser();
    bool checkValidPassWord();
    void showTip(QString str, bool isErrorr);

    void initHandlers();

private:
    Ui::LoginDialog *ui;
    QMap<RequstID, std::function<void(const QJsonObject&)>> m_handlers;
    int m_uid;
    QString m_token;
};

#endif // LOGINDIALOG_H
