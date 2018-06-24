#-------------------------------------------------
#
# Project created by QtCreator 2018-06-04T20:58:41
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = listwidget
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    serialport/serialport.cpp \
    mvp/mainwindow/mainwindow.cpp \
    mvp/mainwindow/item/item.cpp \
    mvp/mainwindow/mainpresenter.cpp \
    mvp/mainwindow/mainmodel.cpp \
    data/sharedpreferences/sharedpreferences.cpp \
    mvp/mainwindow/item/mywidget/mylineedit.cpp \
    mvp/mainwindow/chstatemanager.cpp \
    serialport/lib/RT_CAN.cpp \
    serialport/lib/RT_COM.cpp \
    serialport/processor/candataprocesser.cpp

HEADERS += \
    conf/config.h \
    conf/lang.h \
    serialport/serialport.h \
    mvp/base/mvp.h \
    mvp/mainwindow/mainwindow.h \
    mvp/mainwindow/item/item.h \
    mvp/mainwindow/mainpresenter.h \
    mvp/mainwindow/maincontract.h \
    mvp/mainwindow/mainmodel.h \
    data/sharedpreferences/sharedpreferences.h \
    mvp/mainwindow/item/mywidget/mylineedit.h \
    mvp/mainwindow/maintypedef.h \
    mvp/mainwindow/chstatemanager.h \
    serialport/lib/RT_CAN.h \
    serialport/lib/RT_COM.h \
    utils/mylog.h \
    serialport/processor/candataprocesser.h

FORMS += \
    mvp/mainwindow/mainwindow.ui \
    mvp/mainwindow/item/item.ui

RESOURCES += \
    resource/state/state.qrc
