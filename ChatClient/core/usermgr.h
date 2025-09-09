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
    void SetName(QString name);
    void SetUId(int uid);
    void SetToken(QString token);
    QString& GetName();

private:
    UserMgr();
private:
    QString m_Name;
    QString m_Token;
    int m_UId;
};

#endif // USERMGR_H
