#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <QStyle>
#include <functional>
#include <QRegularExpression>

extern std::function<void(QWidget*)> resetStyle;
extern QString strGateServerURL;

enum class RequstID {
    GET_VERIFY_CODE = 1001,
    REGISTER_USER = 1002,
    RESET_PWD = 1003,
    LOGIN_USER = 1004,//user login
    CHAT_LOGIN = 1005,//login to chat server
    CHAT_LOGIN_RSP = 1006//resopnse from chat server
};

enum class ErrorCodes
{
    SUCCESS = 0,
    JSON,
    NEWTORK
};

enum class Modules
{
    REGISTER = 0,
    RESET,
    LOGIN
};

struct ServerInfo
{
    QString host;
    QString port;
    QString token;
    int uId;
};

enum ListItemType {
    CHAT_USER_ITEM
};

#endif // GLOBAL_H
