#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <QStyle>
#include <functional>
#include <QRegularExpression>

extern std::function<void(QWidget*)> resetStyle;
extern QString strGateServerURL;

enum class RequstID {
    GET_VERIFY_CODE = 1000,
    REGISTER_USER,
    RESET_PWD,
    LOGIN_USER,
    CHAT_LOGIN
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

#endif // GLOBAL_H
