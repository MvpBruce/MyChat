#include "usermgr.h"
#include "core/userdata.h"

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
