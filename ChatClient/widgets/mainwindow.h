#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"

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

private:
    void slot_SwitchRegister();

private:
    Ui::MainWindow *ui;
    LoginDialog* m_loginDlg;
    RegisterDialog* m_regDlg;
};
#endif // MAINWINDOW_H
