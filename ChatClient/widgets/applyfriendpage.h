#ifndef APPLYFRIENDPAGE_H
#define APPLYFRIENDPAGE_H

#include <QWidget>

namespace Ui {
class ApplyFriendPage;
}

class AddFriendInfo;
class ApplyFriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyFriendPage(QWidget *parent = nullptr);
    ~ApplyFriendPage();

    void AddNewApplication(std::shared_ptr<AddFriendInfo> pInfo);

private:
    void LoadApplicationList();

private:
    Ui::ApplyFriendPage *ui;
};

#endif // APPLYFRIENDPAGE_H
