#include "chatdialog.h"
#include "ui_chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog)
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
}

ChatDialog::~ChatDialog()
{
    delete ui;
}
