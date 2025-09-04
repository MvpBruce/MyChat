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
    int m_nUID;
    QString m_strName;
    QString m_strNick;
    QString m_strIcon;
    QString m_strLastMsg;
    int m_nGender;
};

#endif // USERDATA_H
