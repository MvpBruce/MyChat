#ifndef USERDATA_H
#define USERDATA_H
#include <QString>

class SearchInfo {
public:
    SearchInfo(int nUID, QString strName, QString strNick, QString strDesc, int nGender, QString strIcon);

    int m_nUID;
    QString m_strName;
    QString m_strNick;
    QString m_strDesc;
    int m_nGender;
    QString m_strIcon;
};

class AddFriendInfo {
public:
    AddFriendInfo(int from_uid, QString name, QString desc, QString icon, QString nick, int gender)
        : m_nFromUId(from_uid), m_strName(name), m_strDesc(desc), m_strIcon(icon), m_strNick(nick), m_nGender(gender)
    {}

public:
    int m_nFromUId;
    QString m_strName;
    QString m_strDesc;
    QString m_strIcon;
    QString m_strNick;
    int m_nGender;
};

class ApplyInfo
{
public:
    ApplyInfo(int nUID, QString strName, QString strDesc,
              QString strIcon, QString strNick, int nGender,
              int nStatus)
        : m_nUID(nUID), m_strName(std::move(strName)),
        m_strDesc(std::move(strDesc)), m_strIcon(std::move(strIcon)),
        m_strNick(std::move(strNick)), m_nGender(nGender),
        m_nStatus(nStatus) {}

    ApplyInfo(std::shared_ptr<AddFriendInfo> pInfo)
    : m_nUID(pInfo->m_nFromUId), m_strName(std::move(pInfo->m_strName)),
        m_strDesc(std::move(pInfo->m_strDesc)), m_strIcon(std::move(pInfo->m_strIcon)),
        m_strNick(std::move(pInfo->m_strIcon)), m_nGender(pInfo->m_nGender),
        m_nStatus(0) {}

    void SetInfo(QString strHead) { m_strIcon = strHead;}
public:
    int m_nUID;
    QString m_strName;
    QString m_strDesc;
    QString m_strIcon;
    QString m_strNick;
    int m_nGender;
    int m_nStatus;
};

class AuthInfo
{
public:
    AuthInfo(int nUID, QString strName, QString strNick, QString strIcon,
             int nGender)
        : m_nUID(nUID), m_strName(std::move(strName)),
        m_strNick(std::move(strNick)), m_strIcon(std::move(strIcon)),
        m_nGender(nGender) {}

public:
    int m_nUID;
    QString m_strName;
    QString m_strNick;
    QString m_strIcon;
    int m_nGender;
};

class AuthRsp
{
public:
    AuthRsp(int nUID, QString strName, QString strNick, QString strIcon,
            int nGender)
        : m_nUID(nUID), m_strName(std::move(strName)),
        m_strNick(std::move(strNick)), m_strIcon(std::move(strIcon)),
        m_nGender(nGender) {}

public:
    int m_nUID;
    QString m_strName;
    QString m_strNick;
    QString m_strIcon;
    int m_nGender;
};

class UserInfo
{
public:
    UserInfo(int uid, QString name, QString nick, QString icon, int gender, QString msg = "")
        : m_nUID(uid), m_strName(name), m_strNick(nick), m_strIcon(icon), m_strLastMsg(msg), m_nGender(gender)
    {}

    UserInfo(std::shared_ptr<AuthInfo> pInfo)
        : m_nUID(pInfo->m_nUID), m_strName(pInfo->m_strName), m_strNick(pInfo->m_strNick), m_strIcon(pInfo->m_strIcon), m_strLastMsg(""), m_nGender(pInfo->m_nGender)
    {}

    UserInfo(std::shared_ptr<AuthRsp> pInfo)
        : m_nUID(pInfo->m_nUID), m_strName(pInfo->m_strName), m_strNick(pInfo->m_strNick), m_strIcon(pInfo->m_strIcon), m_strLastMsg(""), m_nGender(pInfo->m_nGender)
    {}
    int m_nUID;
    QString m_strName;
    QString m_strNick;
    QString m_strIcon;
    QString m_strLastMsg;
    int m_nGender;
};
#endif // USERDATA_H
