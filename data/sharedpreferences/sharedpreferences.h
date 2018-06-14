#ifndef SHAREDPREFERENCES_H
#define SHAREDPREFERENCES_H

#include <QSettings>

class SharedPreferences
{
public:
    static SharedPreferences* getInstance();

    void setValue(const QString &key, const QVariant &value);
    QVariant getvalue(const QString &key, const QVariant &defaultValue = QVariant()) const;
    bool contains(const QString &key) const;

private:
    SharedPreferences();

private:
    QSettings* settings = nullptr;
};

#endif // SHAREDPREFERENCES_H
