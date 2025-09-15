#include "applyfriendpage.h"
#include "ui_applyfriendpage.h"
#include <QRandomGenerator>
#include "global/global.h"

ApplyFriendPage::ApplyFriendPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplyFriendPage)
{
    ui->setupUi(this);
}

ApplyFriendPage::~ApplyFriendPage()
{
    delete ui;
}

void ApplyFriendPage::AddNewApplication(std::shared_ptr<AddFriendInfo> pInfo)
{
    int nRandom = QRandomGenerator::global()->bounded(100);
    int nHeadIndex = nRandom % heads.size();
    //auto pApplyItem = new app
}

void ApplyFriendPage::LoadApplicationList()
{

}
