#include "usermgr.h"

UserMgr::UserMgr()
{

}

UserMgr::~UserMgr()
{

}

void UserMgr::setName(QString name)
{
    m_Name = name;
}

void UserMgr::setUId(int uid)
{
    m_UId = uid;
}

void UserMgr::setToken(QString token)
{
    m_Token = token;
}
