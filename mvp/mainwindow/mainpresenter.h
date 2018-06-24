#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include "maincontract.h"
#include "serialport/serialport.h"
#include "mainwindow.h"
#include "mainmodel.h"
#include "chstatemanager.h"
#include <QObject>

class MainPresenter : public QObject, public MainContract::Presenter
{
    Q_OBJECT

public:
    explicit MainPresenter(MainContract::View* view);
    ~MainPresenter();

private slots:
    void onConStateChanged(bool isConnected);

private:
    SerialPort* serialPort = nullptr;

    // Presenter interface
public:
    void restoreChName(chnum_t ch, QString name);
};

#endif // MAINPRESENTER_H
