#include "groupitem.h"
#include "ui_groupitem.h"

GroupItem::GroupItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::GroupItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::GROUP_ITEM);
}

GroupItem::~GroupItem()
{
    delete ui;
}

QSize GroupItem::sizeHint() const
{
    return QSize(250, 25);
}

void GroupItem::SetGroupTip(QString str)
{
    ui->label->setText(str);
}
