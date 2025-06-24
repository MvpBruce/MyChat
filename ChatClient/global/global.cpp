#ifndef GLOBAL_CPP
#define GLOBAL_CPP
#include "global.h"

std::function<void(QWidget*)> resetStyle = [](QWidget* w){
    w->style()->unpolish(w);
    w->style()->polish(w);
};

QString strGateServerURL = "";

#endif // GLOBAL_CPP
