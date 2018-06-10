#ifndef SHAREDPREFERENCES_H
#define SHAREDPREFERENCES_H

#include <QSettings>

class SharedPreferences
{
public:
    static SharedPreferences* getInstance();

private:
    SharedPreferences();

public:
    QSettings* settings = nullptr;
};

#endif // SHAREDPREFERENCES_H
