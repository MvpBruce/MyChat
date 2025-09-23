#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <QStyle>
#include <functional>
#include <QRegularExpression>

extern std::function<void(QWidget*)> resetStyle;
extern QString strGateServerURL;

enum RequstID {
    GET_VERIFY_CODE = 1001,
    REGISTER_USER = 1002,
    RESET_PWD = 1003,
    LOGIN_USER = 1004,//user login
    CHAT_LOGIN = 1005,//login to chat server
    CHAT_LOGIN_RSP = 1006,//resopnse from chat server
    SEARCH_USER_REQ = 1007, //request searching user
    SEARCH_USER_RSP = 1008,// response from chat server
    ADD_FRIEND_REQ = 1009,//request adding friend
    ADD_FRIEND_RSP = 1010,
    ADD_FRIEND_NOTIFY = 1011,
    AUTH_FRIEND_REQ = 1012,
    AUTH_FRIEND_RSP = 1013,
    AUTH_FRIEND_NOTIFY = 1014,
    TEXT_CHAT_MSG_REQ = 1015,
    TEXT_CHAT_MSG_RSP = 1016,
    TEXT_CHAT_MSG_NOTIFY = 1017
};

enum ErrorCodes
{
    SUCCESS = 0,
    JSON,
    NEWTORK
};

enum Modules
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
    CHAT_USER_ITEM,
    ADD_USER_TIP_ITEM,
    INVALID_ITEM,
    GROUP_ITEM,
    CONTACT_USER_ITEM,
    APPLY_FRIEND_ITEM
};

enum Role
{
    Self,
    Other
};

enum ClickState
{
    Normal = 0,
    Selected = 1
};

enum SideBarMode
{
    Search,
    Chat,
    Contact
};

struct MsgInfo
{
    QString msgType;
    QString content;
    QPixmap pixMap;
};

const int CHAT_NUMBER_PER_PAGE = 12;

#endif // GLOBAL_H
