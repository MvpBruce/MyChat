#ifndef APPLYFRIENDPAGE_H
#define APPLYFRIENDPAGE_H

#include <QWidget>

namespace Ui {
class ApplyFriendPage;
}

class AddFriendInfo;
class AuthRsp;
class ApplyFriendItem;
class ApplyFriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyFriendPage(QWidget *parent = nullptr);
    ~ApplyFriendPage();

    //Add new apply from search
    void AddNewApplication(std::shared_ptr<AddFriendInfo> pInfo);

public slots:
    void slot_auth_rsp(std::shared_ptr<AuthRsp> pInfo);

private:
    void LoadApplicationList();

private:
    Ui::ApplyFriendPage *ui;
    QMap<int, ApplyFriendItem*> m_mapUnAuthItems;
};

#endif // APPLYFRIENDPAGE_H
