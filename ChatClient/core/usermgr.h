#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include "core/Singleton.h"

class UserMgr : public QObject, public Singleton<UserMgr>,
                public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT
public:
    friend class Singleton<UserMgr>;
    ~UserMgr();
    void setName(QString name);
    void setUId(int uid);
    void setToken(QString token);

private:
    UserMgr();
private:
    QString m_Name;
    QString m_Token;
    int m_UId;
};

#endif // USERMGR_H
