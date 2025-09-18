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
