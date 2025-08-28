#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "core/TcpMgr.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_loginDlg(nullptr), m_regDlg(nullptr), m_resetDlg(nullptr), m_chatDlg(nullptr)
{
    ui->setupUi(this);
    m_loginDlg = new LoginDialog(this);
    m_loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(m_loginDlg);
    m_loginDlg->show();

    connect(m_loginDlg, &LoginDialog::switchToRegister, this, &MainWindow::slot_switchRegister);
    connect(m_loginDlg, &LoginDialog::switchToResetPwd, this, &MainWindow::slot_switchReset);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_switch_chatDialog, this, &MainWindow::slot_switchChat);

    slot_switchChat();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_switchReset()
{
    m_regDlg = new RegisterDialog(this);
    m_regDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    connect(m_regDlg, &RegisterDialog::switchToLogin, this, &MainWindow::slot_switchLogin);
    setCentralWidget(m_regDlg);
    m_loginDlg->hide();
    m_regDlg->show();
}

void MainWindow::slot_switchLogin()
{
    m_loginDlg = new LoginDialog(this);
    m_loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    connect(m_loginDlg, &LoginDialog::switchToRegister, this, &MainWindow::slot_switchRegister);
    connect(m_loginDlg, &LoginDialog::switchToResetPwd, this, &MainWindow::slot_switchReset);
    setCentralWidget(m_loginDlg);
    m_regDlg->hide();
    m_loginDlg->show();
}

void MainWindow::slot_switchLogin1()
{
    m_loginDlg = new LoginDialog(this);
    m_loginDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(m_loginDlg);

    m_resetDlg->hide();
    m_loginDlg->show();

    connect(m_loginDlg, &LoginDialog::switchToResetPwd, this, &MainWindow::slot_switchReset);
    connect(m_loginDlg, &LoginDialog::switchToRegister, this, &MainWindow::slot_switchRegister);
}

void MainWindow::slot_switchRegister()
{
    m_resetDlg = new ResetDialog(this);
    m_resetDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(m_resetDlg);

    m_loginDlg->hide();
    m_resetDlg->show();

    connect(m_resetDlg, &ResetDialog::switchToLogin, this, &MainWindow::slot_switchLogin1);
}

void MainWindow::slot_switchChat()
{
    m_chatDlg = new ChatDialog(this);
    m_chatDlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(m_chatDlg);
    m_chatDlg->show();
    m_loginDlg->hide();
    this->setMinimumSize(QSize(1050,900));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}
