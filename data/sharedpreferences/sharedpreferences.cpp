#include "sharedpreferences.h"

SharedPreferences::SharedPreferences()
{
    settings = new QSettings("bk_name.ini", QSettings::IniFormat);
}

SharedPreferences *SharedPreferences::getInstance()
{
    static SharedPreferences* instance = nullptr;
    if (instance == nullptr) {
        instance = new SharedPreferences();
    }
    return instance;
}

void SharedPreferences::setValue(const QString &key, const QVariant &value)
{
    settings->setValue(key, value);
}

QVariant SharedPreferences::getvalue(const QString &key, const QVariant &defaultValue) const
{
    return settings->value(key, defaultValue);
}

bool SharedPreferences::contains(const QString &key) const
{
    return settings->contains(key);
}
