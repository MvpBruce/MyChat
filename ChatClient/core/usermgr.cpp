#include "usermgr.h"
#include <QJsonArray>
#include <QJsonValue>

UserMgr::UserMgr()
    : m_pUserInfo(nullptr)
{

}

UserMgr::~UserMgr()
{

}

void UserMgr::SetUserInfo(std::shared_ptr<UserInfo> pUserInfo)
{
    m_pUserInfo = pUserInfo;
}

std::shared_ptr<UserInfo> UserMgr::GetUserInfo()
{
    return m_pUserInfo;
}

void UserMgr::SetToken(QString token)
{
    m_Token = token;
}

QString& UserMgr::GetName()
{
    return m_pUserInfo->m_strName;
}

int UserMgr::GetUId()
{
    return m_pUserInfo->m_nUID;
}

QString &UserMgr::GetIcon()
{
    return m_pUserInfo->m_strIcon;
}

void UserMgr::AppendApplyList(QJsonArray array)
{
    for (const QJsonValue& jValue : array)
    {
        auto uid = jValue["uid"].toInt();
        auto name = jValue["name"].toString();
        auto desc = jValue["desc"].toString();
        auto icon = jValue["icon"].toString();
        auto nick = jValue["nick"].toString();
        auto gender = jValue["gender"].toInt();
        auto status = jValue["status"].toInt();
        auto pInfo = std::make_shared<ApplyInfo>(uid, name, desc, icon, nick, gender, status);
        m_vApplyList.push_back(pInfo);
    }
}

void UserMgr::AppendFriendList(QJsonArray array)
{
    for (const QJsonValue& jValue : array)
    {
        auto uid = jValue["uid"].toInt();
        auto name = jValue["name"].toString();
        auto desc = jValue["desc"].toString();
        auto icon = jValue["icon"].toString();
        auto nick = jValue["nick"].toString();
        auto gender = jValue["gender"].toInt();
        auto back = jValue["back"].toString();
        auto pInfo = std::make_shared<FriendInfo>(uid, name, nick, icon, gender, desc, back);
        m_vFriendList.push_back(pInfo);
        m_mapFriend.insert(uid, pInfo);
    }
}
