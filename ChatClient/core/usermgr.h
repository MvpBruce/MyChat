#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include "core/Singleton.h"
#include "core/userdata.h"
#include <QMap>

class UserMgr : public QObject, public Singleton<UserMgr>,
                public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT
public:
    friend class Singleton<UserMgr>;
    ~UserMgr();
    void SetUserInfo(std::shared_ptr<UserInfo> pUserInfo);
    std::shared_ptr<UserInfo> GetUserInfo();
    void SetToken(QString token);
    QString& GetName();
    int GetUId();
    QString& GetIcon();
    void AppendApplyList(QJsonArray array);
    void AppendFriendList(QJsonArray array);
    void AppendFriendChatMsg(int nUid, const std::vector<std::shared_ptr<ChatTextData>>& vChatTextData);
    std::shared_ptr<FriendInfo> GetFriendInfoById(int nUid);

private:
    UserMgr();
private:
    std::shared_ptr<UserInfo> m_pUserInfo;
    QString m_Token;
    std::vector<std::shared_ptr<ApplyInfo>> m_vApplyList;
    std::vector<std::shared_ptr<FriendInfo>> m_vFriendList;
    QMap<int, std::shared_ptr<FriendInfo>> m_mapFriend;
};

#endif // USERMGR_H
