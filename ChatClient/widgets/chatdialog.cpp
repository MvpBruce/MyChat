#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "loadingdlg.h"
#include <QRandomGenerator>
#include "userchatitem.h"
#include <QMouseEvent>
#include "core/TcpMgr.h"
#include "core/usermgr.h"

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog), m_bLoading(false), m_Mode(SideBarMode::Chat), m_State(SideBarMode::Chat), m_nCurChatUId(0)
{
    ui->setupUi(this);

    //Set search icon at left
    QAction* pSearchAction = new QAction(ui->search_edit);
    pSearchAction->setIcon(QIcon(":/assets/image/search.png"));
    ui->search_edit->addAction(pSearchAction, QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("Search"));

    //Set invisible clear icon at right
    QAction* pClear = new QAction(ui->search_edit);
    pClear->setIcon(QIcon(":/assets/image/search_transparent.png"));
    ui->search_edit->addAction(pClear, QLineEdit::TrailingPosition);

    //Set close icon when text changed
    connect(ui->search_edit, &QLineEdit::textChanged, this, [pClear](const QString &text) {
        if (!text.isEmpty())
            pClear->setIcon(QIcon(":/assets/image/search_close.png"));
        else
            pClear->setIcon(QIcon(":/assets/image/search_transparent.png"));
    });

    //Set clear action
    connect(pClear, &QAction::triggered, this, [this, pClear]() {
        ui->search_edit->clear();
        pClear->setIcon(QIcon(":/assets/image/search_transparent.png"));
        ui->search_edit->clearFocus();
        this->SwitchMode(false);
    });

    ui->search_edit->setMaxLength(15);

    ui->add_btn->SetState("normal","hover","press");

    ui->search_chat_list->hide();
    connect(ui->user_chat_list, &UserChatList::sig_load_more_users, this, &ChatDialog::slot_load_more_users);

    //Add user head icon
    QString strIconHead = UserMgr::GetInstance()->GetIcon();;
    QPixmap pixMap(strIconHead);
    QPixmap scaledPix = pixMap.scaled(ui->head_lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lable->setPixmap(scaledPix);
    ui->head_lable->setScaledContents(true);

    //Set contacts icon
    ui->chat_lable->setProperty("state", "normal");
    ui->chat_lable->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");
    ui->contact_lable->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");

    m_listWidget.push_back(ui->chat_lable);
    m_listWidget.push_back(ui->contact_lable);

    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    ui->chat_lable->SetSelected(true);

    connect(ui->chat_lable, &StateWidget::clicked, this, &ChatDialog::slot_clicked_chat);
    connect(ui->contact_lable, &StateWidget::clicked, this, &ChatDialog::slot_clicked_contact);

    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_changed);
    connect(ui->user_con_list, &ContactList::sig_switch_friend_info_page, this, &ChatDialog::slot_friend_info_page);
    connect(ui->user_con_list, &ContactList::sig_switch_apply_friend_page, this, &ChatDialog::slot_switch_apply_friend_page);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_friend_apply, this, &ChatDialog::slot_apply_friend);

    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_add_auth_friend, this, &ChatDialog::slot_add_auth_friend);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ChatDialog::slot_auth_rsp);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_chat_text_msg, this, &ChatDialog::slot_chat_text_msg);

    ui->search_chat_list->SetSearchEdit(ui->search_edit);
    AddChatUserList();

    SwitchMode(false);
    ui->chat_lable->SetSelected(true);
    this->installEventFilter(this);

    connect(ui->chat_page, &ChatPage::sig_append_chat_msg, this, &ChatDialog::slot_append_chat_msg);
    connect(ui->user_chat_list, &QListWidget::itemClicked, this, &ChatDialog::slot_item_clicked);
    connect(ui->user_chat_list, &QListWidget::currentItemChanged, this, &ChatDialog::slot_current_Item_Changed);
    SetCurChatItem(0);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::AddChatUserList()
{
    auto vFriendList = UserMgr::GetInstance()->GetSomeChatList();
    if (!vFriendList.empty())
    {
        for (auto& friendInfo : vFriendList)
        {
            auto it = m_mapUidToItem.find(friendInfo->m_nUID);
            if (it != m_mapUidToItem.end())
                continue;

            auto pChatItem = new UserChatItem();
            auto pInfo = std::make_shared<UserInfo>(friendInfo);
            pChatItem->SetInfo(pInfo);
            QListWidgetItem* pItem = new QListWidgetItem();
            pItem->setSizeHint(pChatItem->sizeHint());
            ui->user_chat_list->addItem(pItem);
            ui->user_chat_list->setItemWidget(pItem, pChatItem);
            m_mapUidToItem.insert(friendInfo->m_nUID, pItem);
        }

        UserMgr::GetInstance()->UpdateChatOffset();
    }
}

void ChatDialog::ClearSideBarState(StateWidget *pWidget)
{
    for (auto& pItem : m_listWidget)
    {
        if (pItem == pWidget)
            continue;

        pItem->ClearState();
    }
}

void ChatDialog::SetCurChatItem(int nUid)
{
    if (ui->user_chat_list->count() <= 0)
        return;

    if (nUid == 0)
    {
        ui->user_chat_list->setCurrentRow(0);
        QListWidgetItem* pItem = ui->user_chat_list->item(0);
        if (!pItem)
            return;

        QWidget* pWidget = ui->user_chat_list->itemWidget(pItem);
        if (!pWidget)
            return;

        auto pUserItem = qobject_cast<UserChatItem*>(pWidget);
        if (!pUserItem)
            return;

        m_nCurChatUId = pUserItem->GetUserInfo()->m_nUID;
        return;
    }

    auto it = m_mapUidToItem.find(nUid);
    if (it == m_mapUidToItem.end())
    {
        ui->user_chat_list->setCurrentRow(0);
        return;
    }

    ui->user_chat_list->setCurrentItem(it.value());
    m_nCurChatUId = nUid;
}

void ChatDialog::slot_load_more_users()
{
    if (m_bLoading)
        return;

    m_bLoading = true;
    LoadingDlg* dlg = new LoadingDlg(this);
    dlg->setModal(true);
    dlg->show();
    AddChatUserList();
    dlg->deleteLater();
    m_bLoading = false;
}

void ChatDialog::slot_clicked_chat()
{
    ClearSideBarState(ui->chat_lable);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    m_State = SideBarMode::Chat;
    SwitchMode(false);
}

void ChatDialog::slot_clicked_contact()
{
    ClearSideBarState(ui->contact_lable);
    m_State = SideBarMode::Contact;
    SwitchMode(false);
}

void ChatDialog::slot_text_changed(const QString &text)
{
    if (!text.isEmpty())
        SwitchMode(true);
}

void ChatDialog::slot_friend_info_page(std::shared_ptr<UserInfo> pUserInfo)
{
    //todo set friend info page
    //ui->stackedWidget->setCurrentWidget();
    //ui->->setInfo(pUserInfo);
    qDebug() << "ChatDialog::slot_friend_info_page";
}

void ChatDialog::slot_switch_apply_friend_page()
{
    ui->stackedWidget->setCurrentWidget(ui->applyFriendPage);
}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendInfo> pInfo)
{
    bool bRet = UserMgr::GetInstance()->HasApplied(pInfo->m_nFromUId);
    if (bRet)
        return;

    UserMgr::GetInstance()->AddApplyList(std::make_shared<ApplyInfo>(pInfo));
    ui->contact_lable->ShowRedPoint(true);
    ui->user_con_list->ShowRedPoint(true);
    ui->applyFriendPage->AddNewApplication(pInfo);
}

void ChatDialog::slot_add_auth_friend(std::shared_ptr<AuthInfo> pInfo)
{
    auto pFriendInfo = UserMgr::GetInstance()->GetFriendInfoById(pInfo->m_nUID);
    if (pFriendInfo)
        return;

    UserMgr::GetInstance()->AddFriend(pInfo);
    auto pChatItem = new UserChatItem();
    auto pUserInfo = std::make_shared<UserInfo>(pInfo);
    pChatItem->SetInfo(pUserInfo);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(pChatItem->sizeHint());
    ui->user_chat_list->insertItem(0, pItem);
    ui->user_chat_list->setItemWidget(pItem, pChatItem);
    m_mapUidToItem.insert(pUserInfo->m_nUID, pItem);
}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> pInfo)
{
    auto pFriendInfo = UserMgr::GetInstance()->GetFriendInfoById(pInfo->m_nUID);
    if (pFriendInfo)
        return;

    auto pChatItem = new UserChatItem();
    auto pUserInfo = std::make_shared<UserInfo>(pInfo);
    pChatItem->SetInfo(pUserInfo);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(pChatItem->sizeHint());
    ui->user_chat_list->insertItem(0, pItem);
    ui->user_chat_list->setItemWidget(pItem, pChatItem);
    m_mapUidToItem.insert(pUserInfo->m_nUID, pItem);
}

void ChatDialog::slot_append_chat_msg(std::shared_ptr<ChatTextData> pData)
{
    if (m_nCurChatUId == 0)
        return;

    auto it = m_mapUidToItem.find(m_nCurChatUId);
    if (it == m_mapUidToItem.end())
        return;

    QWidget* pWidget = ui->user_chat_list->itemWidget(it.value());
    if (!pWidget)
        return;

    ListItemBase* pBaseItem = qobject_cast<ListItemBase*>(pWidget);
    if (!pBaseItem)
        return;

    auto itemType = pBaseItem->GetItemType();
    if (itemType == ListItemType::CHAT_USER_ITEM)
    {
        UserChatItem* pUserItem = qobject_cast<UserChatItem*>(pBaseItem);
        if (!pUserItem)
            return;

        auto pUserInfo = pUserItem->GetUserInfo();
        pUserInfo->m_vChatTextData.push_back(pData);
        std::vector<std::shared_ptr<ChatTextData>> vMsg;
        vMsg.push_back(pData);
        UserMgr::GetInstance()->AppendFriendChatMsg(m_nCurChatUId, vMsg);
    }
}

void ChatDialog::slot_chat_text_msg(std::shared_ptr<ChatTextMsg> pTextMsg)
{
    auto it = m_mapUidToItem.find(pTextMsg->m_nFromUid);
    if (it != m_mapUidToItem.end())
    {
        QWidget* pWidget = ui->user_chat_list->itemWidget(it.value());
        UserChatItem* pChatItem = qobject_cast<UserChatItem*>(pWidget);
        if (!pChatItem)
            return;

        //update current chat item
        pChatItem->UpdateLasgMsg(pTextMsg->m_vChatMsg);
        UpdateChatMsg(pTextMsg->m_vChatMsg);
        UserMgr::GetInstance()->AppendFriendChatMsg(pTextMsg->m_nFromUid, pTextMsg->m_vChatMsg);
        return;
    }

    //create new item if not existed
    UserChatItem* pChatItem = new UserChatItem();
    //get friend's info
    auto pFriendInfo = UserMgr::GetInstance()->GetFriendInfoById(pTextMsg->m_nFromUid);
    pChatItem->SetInfo(pFriendInfo);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(pChatItem->sizeHint());
    pChatItem->UpdateLasgMsg(pTextMsg->m_vChatMsg);
    UserMgr::GetInstance()->AppendFriendChatMsg(pTextMsg->m_nFromUid, pTextMsg->m_vChatMsg);
    ui->user_chat_list->insertItem(0, pItem);
    ui->user_chat_list->setItemWidget(pItem, pChatItem);
    m_mapUidToItem.insert(pTextMsg->m_nFromUid, pItem);
}

void ChatDialog::slot_item_clicked(QListWidgetItem *pItem)
{
    QWidget* pWidget = ui->user_chat_list->itemWidget(pItem);
    if (!pWidget)
        return;

    ListItemBase* pBase = qobject_cast<ListItemBase*>(pWidget);
    if (!pBase)
        return;

    auto nType = pBase->GetItemType();
    if (nType == ListItemType::INVALID_ITEM || nType == ListItemType::GROUP_ITEM)
        return;

    if (nType == ListItemType::CHAT_USER_ITEM)
    {
        auto pChatItem = qobject_cast<UserChatItem*>(pBase);
        auto pUserInfo = pChatItem->GetUserInfo();
        ui->chat_page->SetUserInfo(pUserInfo);
        m_nCurChatUId = pUserInfo->m_nUID;
    }
}

void ChatDialog::slot_current_Item_Changed(QListWidgetItem *current, QListWidgetItem *previous)
{
    this->slot_item_clicked(current);
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(event);
        HandleMousePress(pMouseEvent);
    }

    return QDialog::eventFilter(watched, event);
}

void ChatDialog::UpdateChatMsg(const std::vector<std::shared_ptr<ChatTextData>>& vChatText)
{
    for (auto& chatText: vChatText)
    {
        if (chatText->m_nFromUid != m_nCurChatUId)
            break;

        ui->chat_page->AppendChatMsg(chatText);
    }
}

void ChatDialog::SwitchMode(bool bSearch)
{
    if (bSearch)
    {
        ui->user_chat_list->hide();
        ui->search_chat_list->show();
        ui->user_con_list->hide();
        m_Mode = SideBarMode::Search;
    }
    else if (m_State == SideBarMode::Chat)
    {
        ui->search_chat_list->hide();
        ui->user_chat_list->show();
        ui->user_con_list->hide();
        ui->search_edit->clear();
        ui->search_edit->clearFocus();
        m_Mode = SideBarMode::Chat;
    }
    else if (m_State == SideBarMode::Contact)
    {
        ui->user_chat_list->hide();
        ui->search_chat_list->hide();
        ui->user_con_list->show();
        ui->search_edit->clear();
        ui->search_edit->clearFocus();
        m_Mode = SideBarMode::Contact;
    }
}

void ChatDialog::HandleMousePress(QMouseEvent *event)
{
    if (m_Mode != SideBarMode::Search)
        return;

    QPoint posClick = ui->search_chat_list->mapToGlobal(event->globalPosition()).toPoint();
    if (!ui->search_chat_list->rect().contains(posClick))
    {
        ui->search_edit->clear();
        SwitchMode(false);
    }
}
