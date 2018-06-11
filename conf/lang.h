#ifndef LANG_H
#define LANG_H

#include <QString>

typedef const QString LangType;

namespace Lang {
    LangType online  = "在线";
    LangType lost    = "丢失";
    LangType hide    = "";

    LangType connected = "已连接";
    LangType notconnected = "未连接";
}

#endif // LANG_H
