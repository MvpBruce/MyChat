#include "userdata.h"

SearchInfo::SearchInfo(int nUID, QString strName, QString strNick, QString strDesc, int nGender, QString strIcon)
    : m_nUID(nUID), m_strName(strName), m_strNick(strNick), m_strDesc(strDesc), m_nGender(nGender), m_strIcon(strIcon)
{

}

void FriendInfo::AppendChatMsgs(const std::vector<std::shared_ptr<ChatTextData> > &vChatTextData)
{
    for (const auto& textData : vChatTextData)
        m_vChatTextData.push_back(textData);
}


ChatTextMsg::ChatTextMsg(int nFromUid, int nToUid, const QJsonArray& jsonArray)
    : m_nFromUid(nFromUid), m_nToUid(nToUid)
{
    for (auto& msgData : jsonArray)
    {
        auto msgObj = msgData.toObject();
        auto strContent = msgObj["content"].toString();
        auto strMsgID = msgObj["msgid"].toString();
        auto pChatTextData = std::make_shared<ChatTextData>(strMsgID, strContent, m_nFromUid, m_nToUid);
        m_vChatMsg.push_back(pChatTextData);
    }
}
