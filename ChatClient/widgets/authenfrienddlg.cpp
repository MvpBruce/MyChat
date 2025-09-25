#include "authenfrienddlg.h"
#include "ui_authenfrienddlg.h"
#include <QScrollBar>
#include <QJsonObject>
#include "core/usermgr.h"
#include "core/TcpMgr.h"

AuthenFriendDlg::AuthenFriendDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthenFriendDlg), m_pApplyInfo(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setObjectName("authenFriendDlg");
    setModal(true);
    ui->labelEdit->setPlaceholderText("Search and add labels");
    ui->backEdit->setPlaceholderText("New friend");

    ui->labelEdit->move(2, 2);
    ui->labelEdit->setFixedHeight(20);
    ui->labelEdit->setMaxLength(10);
    ui->inputTipWid->hide();

    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);
    ui->scrollArea->installEventFilter(this);
    ui->okBtn->SetState("normal","hover","press");
    ui->cancelBtn->SetState("normal","hover","press");

    connect(ui->cancelBtn, &QPushButton::clicked, this, &AuthenFriendDlg::slot_apply_cancel);
    connect(ui->okBtn, &QPushButton::clicked, this, &AuthenFriendDlg::slot_apply_ok);
}

AuthenFriendDlg::~AuthenFriendDlg()
{
    delete ui;
}

void AuthenFriendDlg::SetApplyInfo(std::shared_ptr<ApplyInfo> pApplyInfo)
{
    m_pApplyInfo = pApplyInfo;
    ui->backEdit->setPlaceholderText(pApplyInfo->m_strName);
}

bool AuthenFriendDlg::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->scrollArea && event->type() == QEvent::Enter)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(false);
    }
    else if (obj == ui->scrollArea && event->type() == QEvent::Leave)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(true);
    }
    return QObject::eventFilter(obj, event);
}

void AuthenFriendDlg::slot_apply_ok()
{
    QJsonObject jsonObj;
    auto uid = UserMgr::GetInstance()->GetUId();
    jsonObj["fromuid"] = uid;
    jsonObj["touid"] = m_pApplyInfo->m_nUID;
    QString strBackName = "";
    if (ui->backEdit->text().isEmpty())
        strBackName = ui->backEdit->placeholderText();
    else
        strBackName = ui->backEdit->text();

    jsonObj["back"] = strBackName;

    QJsonDocument doc(jsonObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    emit TcpMgr::GetInstance()->sig_send_data(RequstID::AUTH_FRIEND_REQ, jsonData);

    this->hide();
    deleteLater();
}

void AuthenFriendDlg::slot_apply_cancel()
{
    this->hide();
    deleteLater();
}
