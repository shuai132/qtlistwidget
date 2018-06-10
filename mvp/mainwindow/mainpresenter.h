#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include "maincontract.h"
#include "serialport/serialport.h"
#include <QObject>

class MainPresenter : public QObject, public MVP<MainContract::Model, MainContract::View>::Presenter
{
    Q_OBJECT

public:
    explicit MainPresenter(MainContract::View* view);
    ~MainPresenter();

private slots:
    void onConStateChanged(bool isConnected);

private:
    SerialPort* serialPort = nullptr;
};

#endif // MAINPRESENTER_H
