#include "usermgr.h"
#include <QJsonArray>
#include <QJsonValue>
#include "global/global.h"

UserMgr::UserMgr()
    : m_pUserInfo(nullptr), m_nChatLoaded(0), m_nContactLoaded(0)
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

void UserMgr::AppendFriendChatMsg(int nUid, const std::vector<std::shared_ptr<ChatTextData>>& vChatTextData)
{
    auto it = m_mapFriend.find(nUid);
    if (it == m_mapFriend.end())
        return;

    it.value()->AppendChatMsgs(vChatTextData);
}

std::shared_ptr<FriendInfo> UserMgr::GetFriendInfoById(int nUid)
{
    auto it = m_mapFriend.find(nUid);
    if (it != m_mapFriend.end())
        return it.value();

    return nullptr;
}

std::vector<std::shared_ptr<FriendInfo>> UserMgr::GetSomeChatList()
{
    std::vector<std::shared_ptr<FriendInfo>> vFriendList;
    int nStart = m_nChatLoaded;
    int nEnd = nStart + CHAT_NUMBER_PER_PAGE;

    if (nStart >= m_vFriendList.size())
        return vFriendList;

    if (nEnd > m_vFriendList.size())
    {
        vFriendList = std::vector<std::shared_ptr<FriendInfo>>(m_vFriendList.begin() + nStart, m_vFriendList.end());
        return vFriendList;
    }

    vFriendList = std::vector<std::shared_ptr<FriendInfo>>(m_vFriendList.begin() + nStart, m_vFriendList.end() + nEnd);
    return vFriendList;
}

void UserMgr::UpdateChatOffset()
{
    int nBegin = m_nChatLoaded;
    int nEnd = nBegin + CHAT_NUMBER_PER_PAGE;

    if (nBegin >= m_vFriendList.size()) {
        return ;
    }

    if (nEnd > m_vFriendList.size()) {
        m_nChatLoaded = m_vFriendList.size();
        return ;
    }

    m_nChatLoaded = nEnd;
}

void UserMgr::UpdateContactOffset()
{
    int nBegin = m_nContactLoaded;
    int nEnd = nBegin + CHAT_NUMBER_PER_PAGE;

    if (nBegin >= m_vFriendList.size()) {
        return ;
    }

    if (nEnd > m_vFriendList.size()) {
        m_nContactLoaded = m_vFriendList.size();
        return ;
    }

    m_nContactLoaded = nEnd;
}

std::vector<std::shared_ptr<FriendInfo>> UserMgr::GetSomeContactList()
{
    std::vector<std::shared_ptr<FriendInfo>> vContactList;
    int nStart = m_nContactLoaded;
    int nEnd = nStart + CHAT_NUMBER_PER_PAGE;

    if (nStart >= m_vFriendList.size())
        return vContactList;

    if (nEnd > m_vFriendList.size())
    {
        vContactList = std::vector<std::shared_ptr<FriendInfo>>(m_vFriendList.begin() + nStart, m_vFriendList.end());
        return vContactList;
    }

    vContactList = std::vector<std::shared_ptr<FriendInfo>>(m_vFriendList.begin() + nStart, m_vFriendList.end() + nEnd);
    return vContactList;
}
