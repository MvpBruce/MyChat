#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "loadingdlg.h"
#include <QRandomGenerator>
#include "userchatitem.h"
#include <QMouseEvent>
#include "core/TcpMgr.h"

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog), m_bLoading(false), m_Mode(SideBarMode::Chat), m_State(SideBarMode::Chat)
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
    QString strIconHead = ":/assets/image/head_1.jpg";
    QPixmap pixMat(strIconHead);
    QPixmap scaledPix = pixMat.scaled(ui->head_lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lable->setPixmap(scaledPix);
    ui->head_lable->setScaledContents(true);

    //Set contacts icon
    ui->chat_lable->setProperty("state", "normal");
    ui->chat_lable->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");
    ui->contacts_lable->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");

    m_listWidget.push_back(ui->chat_lable);
    m_listWidget.push_back(ui->contacts_lable);
    connect(ui->chat_lable, &StateWidget::clicked, this, &ChatDialog::slot_clicked_chat);
    connect(ui->contacts_lable, &StateWidget::clicked, this, &ChatDialog::slot_clicked_contact);

    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_changed);
    connect(ui->user_con_list, &ContactList::sig_switch_friend_info_page, this, &ChatDialog::slot_friend_info_page);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_friend_apply, this, &ChatDialog::slot_apply_friend);


    ui->search_chat_list->SetSearchEdit(ui->search_edit);
    AddChatUserList();

    SwitchMode(false);
    ui->chat_lable->SetSelected(true);
    this->installEventFilter(this);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::AddChatUserList()
{
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100);
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto *chat_user_wid = new UserChatItem();
        chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->user_chat_list->addItem(item);
        ui->user_chat_list->setItemWidget(item, chat_user_wid);
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
    ClearSideBarState(ui->contacts_lable);
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
    //ui->stackedWidget->setCurrentWidget(ui->friend)
    //ui->friendInfoPage->setInfo(pUserInfo);
    qDebug() << "ChatDialog::slot_friend_info_page";
}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendInfo> pInfo)
{
    //todo, need to check friend request already sent

    //toto, add to apply list
    ui->contacts_lable->ShowRedPoint(true);
    ui->user_con_list->ShowRedPoint(true);
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
