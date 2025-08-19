#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
#include "chatdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slot_switchRegister();
    void slot_switchLogin();
    void slot_switchReset();
    void slot_switchLogin1();
    void slot_switchChat();

private:
    Ui::MainWindow *ui;
    LoginDialog* m_loginDlg;
    RegisterDialog* m_regDlg;
    ResetDialog* m_resetDlg;
    ChatDialog* m_chatDlg;
};
#endif // MAINWINDOW_H
