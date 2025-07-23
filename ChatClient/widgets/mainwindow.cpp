#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_loginDlg(nullptr), m_regDlg(nullptr), m_resetDlg(nullptr)
{
    ui->setupUi(this);
    m_loginDlg = new LoginDialog(this);
    m_loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(m_loginDlg);
    m_loginDlg->show();

    connect(m_loginDlg, &LoginDialog::switchToRegister, this, &MainWindow::slot_SwitchRegister);
    connect(m_loginDlg, &LoginDialog::switchToResetPwd, this, &MainWindow::slot_SwitchReset);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_SwitchRegister()
{
    m_regDlg = new RegisterDialog(this);
    m_regDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    connect(m_regDlg, &RegisterDialog::switchToLogin, this, &MainWindow::slot_SwitchLogin);
    setCentralWidget(m_regDlg);
    m_loginDlg->hide();
    m_regDlg->show();
}

void MainWindow::slot_SwitchLogin()
{
    m_loginDlg = new LoginDialog(this);
    m_loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    connect(m_loginDlg, &LoginDialog::switchToRegister, this, &MainWindow::slot_SwitchRegister);
    connect(m_loginDlg, &LoginDialog::switchToResetPwd, this, &MainWindow::slot_SwitchReset);
    setCentralWidget(m_loginDlg);
    m_regDlg->hide();
    m_loginDlg->show();
}

void MainWindow::slot_SwitchLogin1()
{
    m_loginDlg = new LoginDialog(this);
    m_loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(m_loginDlg);

    m_resetDlg->hide();
    m_loginDlg->show();

    connect(m_loginDlg, &LoginDialog::switchToResetPwd, this, &MainWindow::slot_SwitchReset);
    connect(m_loginDlg, &LoginDialog::switchToRegister, this, &MainWindow::slot_SwitchRegister);
}

void MainWindow::slot_SwitchReset()
{
    m_resetDlg = new ResetDialog(this);
    m_resetDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(m_resetDlg);

    m_loginDlg->hide();
    m_resetDlg->show();

    connect(m_resetDlg, &ResetDialog::switchToLogin, this, &MainWindow::slot_SwitchLogin1);
}
