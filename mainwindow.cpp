#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug()<<"initing...";
    ui->setupUi(this);

    ui->listWidget->resize(ui->centralWidget->size());
    for (chnum_t i = 0; i < AllChNum; i++) {
        QListWidgetItem* qListWidgetItem = new QListWidgetItem(ui->listWidget);
        ui->listWidget->addItem(qListWidgetItem);

        auto uiItem = new Item(i + 1);
        ui->listWidget->setItemWidget(qListWidgetItem, uiItem->ui->widget);
        itemInfos.push_back(ItemInfo(qListWidgetItem, uiItem));

        connect(uiItem, SIGNAL(itemClicked(chnum_t)), this, SLOT(onItemClicked(chnum_t)));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event __unused)
{
    auto centralSize = ui->centralWidget->size();
    qDebug()<<centralSize;

    ui->listWidget->resize(ui->centralWidget->size());
    for (auto itemInfo : itemInfos) {
        itemInfo.qListWidgetItem->setSizeHint(itemInfo.item->ui->widget->size());
        itemInfo.item->ui->widget->resize(QSize(centralSize.width(), itemInfo.item->ui->widget->size().height()));
    }
}

void MainWindow::onItemClicked(chnum_t id)
{
    qDebug()<<"onItemClicked:"<<id;
}
