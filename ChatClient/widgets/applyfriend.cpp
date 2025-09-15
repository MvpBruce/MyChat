#include "applyfriend.h"
#include "ui_applyfriend.h"
#include "core/userdata.h"
#include "statelabel.h"
#include <QFontMetrics>
#include <QScrollBar>
#include "core/usermgr.h"
#include <QJsonObject>
#include "core/TcpMgr.h"

const int g_tipHOffset = 5;
const int g_tipVOffset = 15;

ApplyFriend::ApplyFriend(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApplyFriend), m_labelPoint(2, 6), m_pSearchInfo(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setObjectName("applyFriend");
    this->setModal(true);
    ui->nameEdit->setPlaceholderText("test user");
    ui->labelEdit->setPlaceholderText("search and add lables");
    ui->nickNameEdit->setPlaceholderText("user's nick name");

    ui->labelEdit->setMaxLength(21);
    ui->labelEdit->move(2, 2);
    ui->labelEdit->setFixedHeight(20);
    ui->inputTipWid->hide();

    m_curTipPoint = QPoint(5, 5);

    m_tipData = {"Classmate", "Family", "Junior tutorial", "c++ Primer", "Rust design", "Pytho", "Node js", "go"};
    connect(ui->moreLb, &TipLabel::clicked, this, &ApplyFriend::slot_show_more);
    InitTipLabels();

    connect(ui->okBtn, &QPushButton::clicked, this, &ApplyFriend::slot_apply_ok);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &ApplyFriend::slot_apply_cancel);
}

ApplyFriend::~ApplyFriend()
{
    delete ui;
}

void ApplyFriend::InitTipLabels()
{
    int nLines = 1;
    for (int i = 0; i < m_tipData.size(); i++)
    {
        auto* pLb = new StateLabel(ui->lableListWid);
        pLb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");
        pLb->setObjectName("tipslb");
        pLb->setText(m_tipData[i]);
        connect(pLb, &StateLabel::clicked, this, &ApplyFriend::slot_change_tip);

        QFontMetrics fontMetrics(pLb->font());
        int nWidth = fontMetrics.boundingRect(pLb->text()).width();
        int nHeight = fontMetrics.height();

        if (m_curTipPoint.x() + nWidth + g_tipHOffset > ui->lableListWid->width())
        {
            nLines++;
            if (nLines > 2)
            {
                delete pLb;
                return;
            }

            m_curTipPoint.setX(g_tipHOffset);
            m_curTipPoint.setY(m_curTipPoint.y() + nHeight + g_tipVOffset);
        }

        auto nextPoint = m_curTipPoint;
        AddTipLabels(pLb, m_curTipPoint, nextPoint, nWidth, nHeight);
        m_curTipPoint = nextPoint;
    }
}

void ApplyFriend::AddTipLabels(StateLabel *pLable, QPoint curPoint, QPoint &nextPoint, int nWidth, int nHeight)
{
    pLable->move(curPoint);
    pLable->show();
    m_mapTips.insert(pLable->text(), pLable);
    nextPoint.setX(pLable->pos().x() + nWidth + g_tipHOffset);
    nextPoint.setY(pLable->pos().y());
}

bool ApplyFriend::eventFilter(QObject *obj, QEvent *event)
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

void ApplyFriend::SetSearchInfo(std::shared_ptr<SearchInfo> pSearchInfo)
{
    m_pSearchInfo = pSearchInfo;
    auto applyName = UserMgr::GetInstance()->GetName();
    auto nickName = m_pSearchInfo->m_strName;
    ui->nameEdit->setText(applyName);
    ui->nickNameEdit->setText(nickName);
}

void ApplyFriend::slot_show_more()
{
    ui->moreLbWid->hide();
    //todo
}

void ApplyFriend::slot_change_tip(QString text, ClickState state)
{

}

void ApplyFriend::slot_apply_ok()
{
    QJsonObject jObj;
    auto uid = UserMgr::GetInstance()->GetUId();
    jObj["uid"] = uid;
    auto name = ui->nameEdit->text();
    if (name.isEmpty())
        name = ui->nameEdit->placeholderText();

    jObj["applyname"] = name;
    auto nickName = ui->nickNameEdit->text();
    if (nickName.isEmpty())
        nickName = ui->nickNameEdit->placeholderText();

    jObj["backname"] = nickName;
    jObj["touid"] = m_pSearchInfo->m_nUID;

    QJsonDocument jDoc(jObj);
    QByteArray jsonData = jDoc.toJson(QJsonDocument::Compact);
    //send request to chat server
    emit TcpMgr::GetInstance()->sig_send_data(RequstID::ADD_FRIEND_REQ, jsonData);
    this->hide();
    deleteLater();
}

void ApplyFriend::slot_apply_cancel()
{
    this->hide();
    deleteLater();
}
