#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"

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
    void slot_SwitchRegister();
    void slot_SwitchLogin();
    void slot_SwitchReset();
    void slot_SwitchLogin1();

private:
    Ui::MainWindow *ui;
    LoginDialog* m_loginDlg;
    RegisterDialog* m_regDlg;
    ResetDialog* m_resetDlg;
};
#endif // MAINWINDOW_H
