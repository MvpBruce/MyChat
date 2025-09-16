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
    AUTH_FRIEND_NOTIFY = 1014
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

const std::vector<QString>  strs ={"hello world !",
                                   "nice to meet u",
                                   "New year，new life",
                                   "You have to love yourself",
                                   "My love is written in the wind ever since the whole world is you"};

const std::vector<QString> names = {
    "HanMeiMei",
    "Lily",
    "Ben",
    "Androw",
    "Max",
    "Summer",
    "Candy",
    "Hunter"
};

const std::vector<QString> heads = {
    ":/assets/image/head_1.jpg",
    ":/assets/image/head_2.jpg",
    ":/assets/image/head_3.jpg",
    ":/assets/image/head_4.jpg",
    ":/assets/image/head_5.jpg"
};

#endif // GLOBAL_H
