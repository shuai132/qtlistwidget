#include "sharedpreferences.h"

SharedPreferences *SharedPreferences::getInstance()
{
    static SharedPreferences* instance = nullptr;
    if (instance == nullptr) {
        instance = new SharedPreferences();
    }
    return instance;
}

SharedPreferences::SharedPreferences()
{
    settings = new QSettings("bk_name.ini", QSettings::IniFormat);
}
