#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <QStyle>
#include <functional>

extern std::function<void(QWidget*)> resetStyle;
extern QString strGateServerURL;

enum class RequstID {
    GET_VERIFY_CODE = 1000,
    REGISTER_USER
};

enum class ErrorCodes
{
    SUCCESS = 0,
    JSON,
    NEWTORK
};

enum class Modules
{
    REGISTER = 0
};

#endif // GLOBAL_H
