#include "usermgr.h"

UserMgr::UserMgr()
{

}

UserMgr::~UserMgr()
{

}

void UserMgr::SetName(QString name)
{
    m_Name = name;
}

void UserMgr::SetUId(int uid)
{
    m_UId = uid;
}

void UserMgr::SetToken(QString token)
{
    m_Token = token;
}

QString& UserMgr::GetName()
{
    return m_Name;
}

int UserMgr::GetUId()
{
    return m_UId;
}
