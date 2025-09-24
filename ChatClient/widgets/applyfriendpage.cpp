#include "applyfriendpage.h"
#include "ui_applyfriendpage.h"
#include <QRandomGenerator>
#include "global/global.h"
#include "applyfrienditem.h"

#include <QJsonObject>
#include "core/usermgr.h"
#include "core/TcpMgr.h"

ApplyFriendPage::ApplyFriendPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplyFriendPage)
{
    ui->setupUi(this);
    LoadApplicationList();
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ApplyFriendPage::slot_auth_rsp);
}

ApplyFriendPage::~ApplyFriendPage()
{
    delete ui;
}

void ApplyFriendPage::AddNewApplication(std::shared_ptr<AddFriendInfo> pInfo)
{
    auto applyItem = new ApplyFriendItem();
    auto pApplyInfo = std::make_shared<ApplyInfo>(pInfo);
    applyItem->SetInfo(pApplyInfo);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setFlags(pItem->flags() &~ Qt::ItemIsEnabled &~ Qt::ItemIsSelectable);
    pItem->setSizeHint(applyItem->sizeHint());
    ui->applyFriendList->insertItem(0, pItem);
    ui->applyFriendList->setItemWidget(pItem, applyItem);
    applyItem->ShowAddBtn(true);
    connect(applyItem, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> pInfo) {
        //todo, create authenticate friend page
        //set moddal
        //set applyinfo
        //show
        qDebug() << "Show authenticate friend page";
        qDebug() << "Slot Apply Sure ";
        QJsonObject jsonObj;
        auto uid = UserMgr::GetInstance()->GetUId();
        jsonObj["fromuid"] = uid;
        jsonObj["touid"] = pInfo->m_nUID;
        jsonObj["back"] = "backName";

        QJsonDocument doc(jsonObj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
        emit TcpMgr::GetInstance()->sig_send_data(RequstID::AUTH_FRIEND_REQ, jsonData);

        //this->hide();
        //deleteLater();
    });

}

void ApplyFriendPage::slot_auth_rsp(std::shared_ptr<AuthRsp> pInfo)
{
    auto it = m_mapUnAuthItems.find(pInfo->m_nUID);
    if (it == m_mapUnAuthItems.end())
        return;

    it.value()->ShowAddBtn(false);
}

void ApplyFriendPage::LoadApplicationList()
{
    auto applyList = UserMgr::GetInstance()->GetApplyList();
    for (const auto& apply : applyList)
    {
        auto* pApplyItem = new ApplyFriendItem();
        pApplyItem->SetInfo(apply);
        QListWidgetItem* pItem = new QListWidgetItem();
        pItem->setSizeHint(pApplyItem->sizeHint());
        pItem->setFlags(pItem->flags() &~ Qt::ItemIsEnabled &~ Qt::ItemIsSelectable);
        ui->applyFriendList->insertItem(0, pItem);
        ui->applyFriendList->setItemWidget(pItem, pApplyItem);
        if (apply->m_nStatus)
        {
            pApplyItem->ShowAddBtn(false);
        }
        else
        {
            pApplyItem->ShowAddBtn(true);
            m_mapUnAuthItems[apply->m_nUID] = pApplyItem;
        }

        //receive authenticatd signal
        connect(pApplyItem, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> pInfo){
            //todo,
            //new authe friend dialog
            //set modal
            //set apply info to dialg
            //show
        });
    }
}
