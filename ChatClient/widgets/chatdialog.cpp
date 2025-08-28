#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "loadingdlg.h"

#include <QRandomGenerator>
#include "userchatitem.h"

//test
std::vector<QString>  strs ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                             "You have to love yourself",
                             "My love is written in the wind ever since the whole world is you"};

std::vector<QString> heads = {
    ":/assets/image/head_1.jpg",
    ":/assets/image/head_2.jpg",
    ":/assets/image/head_3.jpg",
    ":/assets/image/head_4.jpg",
    ":/assets/image/head_5.jpg"
};

std::vector<QString> names = {
    "llfc",
    "zack",
    "golang",
    "cpp",
    "java",
    "nodejs",
    "python",
    "rust"
};
//end

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog), m_bLoading(false)
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
    });

    ui->search_edit->setMaxLength(15);

    ui->add_btn->ChangeState("normal","hover","press");

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
    AddChatUserList();
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
    //set curmode, todo
    //show search, todo
}

void ChatDialog::slot_clicked_contact()
{
    ClearSideBarState(ui->contacts_lable);
}
