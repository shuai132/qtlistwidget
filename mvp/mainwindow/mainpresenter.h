#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include "maincontract.h"
#include "mainwindow.h"
#include "mainmodel.h"
#include "chstatemanager.h"
#include "serialport/serialport.h"
#include "serialport/processor/candataprocesser.h"
#include <QObject>

class MainPresenter : public QObject, public MainContract::Presenter
{
    Q_OBJECT

public:
    explicit MainPresenter(MainContract::View* view);
    ~MainPresenter();

private:
    SerialPort* serialPort = nullptr;
    CanDataProcesser* canDataProcesser = nullptr;

    // Presenter interface
public:
    void restoreChName(chnum_t ch, QString name);
};

#endif // MAINPRESENTER_H
