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

struct UserInfo
{
    UserInfo(int uid, QString name, QString nick, QString icon, int gender, QString msg = "")
        : m_nUID(uid), m_strName(name), m_strNick(nick), m_strIcon(icon), m_strLastMsg(msg), m_nGender(gender)
    {}
    int m_nUID;
    QString m_strName;
    QString m_strNick;
    QString m_strIcon;
    QString m_strLastMsg;
    int m_nGender;
};

#endif // USERDATA_H
