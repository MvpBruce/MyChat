#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_loginDlg = new LoginDialog();
    setCentralWidget(m_loginDlg);
    m_loginDlg->show();

    connect(m_loginDlg, &LoginDialog::switchToRegister, this, &MainWindow::slot_SwitchRegister);
    m_regDlg = new RegisterDialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_SwitchRegister()
{
    setCentralWidget(m_regDlg);
    m_loginDlg->hide();
    m_regDlg->show();
}
