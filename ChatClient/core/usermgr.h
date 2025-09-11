#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include "core/Singleton.h"

class UserInfo;
class UserMgr : public QObject, public Singleton<UserMgr>,
                public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT
public:
    friend class Singleton<UserMgr>;
    ~UserMgr();
    void SetUserInfo(std::shared_ptr<UserInfo> pUserInfo);
    void SetToken(QString token);
    QString& GetName();
    int GetUId();
    QString& GetIcon();

private:
    UserMgr();
private:
    std::shared_ptr<UserInfo> m_pUserInfo;
    QString m_Token;
};

#endif // USERMGR_H
