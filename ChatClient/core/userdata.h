#ifndef USERDATA_H
#define USERDATA_H
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

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

class ChatTextData;
class FriendInfo
{
public:
    FriendInfo(int nUID, QString strName, QString strNick,
               QString strIcon, int nGender, QString strDesc,
               QString strBack, QString strLastMsg = "")
        : m_nUID(nUID), m_strName(std::move(strName)),
        m_strNick(std::move(strNick)), m_strIcon(std::move(strIcon)),
        m_nGender(nGender), m_strDesc(std::move(strDesc)),
        m_strBack(std::move(strBack)), m_strLastMsg(std::move(strLastMsg)) {
    }

    void AppendChatMsgs(const std::vector<std::shared_ptr<ChatTextData>>& vChatTextData);

    int m_nUID;
    QString m_strName;
    QString m_strNick;
    QString m_strIcon;
    int m_nGender;
    QString m_strDesc;
    QString m_strBack;
    QString m_strLastMsg;
    std::vector<std::shared_ptr<ChatTextData>> m_vChatTextData;
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

    UserInfo(std::shared_ptr<FriendInfo> pInfo)
        : m_nUID(pInfo->m_nUID), m_strName(pInfo->m_strName), m_strNick(pInfo->m_strNick), m_strIcon(pInfo->m_strIcon), m_strLastMsg(""), m_nGender(pInfo->m_nGender)
    {
        m_vChatTextData = pInfo->m_vChatTextData;
    }

    int m_nUID;
    QString m_strName;
    QString m_strNick;
    QString m_strIcon;
    QString m_strLastMsg;
    int m_nGender;
    std::vector<std::shared_ptr<ChatTextData>> m_vChatTextData;
};

class ChatTextData
{
public:
    ChatTextData(QString strMsgID, QString strContent, int nFromUid,
                 int nToUid)
        : m_strMsgID(std::move(strMsgID)),
        m_strContent(std::move(strContent)), m_nFromUid(nFromUid),
        m_nToUid(nToUid) {}

    QString m_strMsgID;
    QString m_strContent;
    int m_nFromUid;
    int m_nToUid;
};

class ChatTextMsg
{
public:
    ChatTextMsg(int nFromUid, int nToUid, QJsonArray jsonArray)
        : m_nFromUid(nFromUid), m_nToUid(nToUid)
    {
        for (auto msgData : jsonArray)
        {
            auto msgObj = msgData.toObject();
            auto strContent = msgObj["content"].toString();
            auto strMsgID = msgObj["msgid"].toString();
            auto pChatTextData = std::make_shared<ChatTextData>(strMsgID, strContent, m_nFromUid, m_nToUid);
            m_vChatMsg.push_back(pChatTextData);
        }
    }

public:
    int m_nFromUid;
    int m_nToUid;
    std::vector<std::shared_ptr<ChatTextData>> m_vChatMsg;
};

#endif // USERDATA_H
