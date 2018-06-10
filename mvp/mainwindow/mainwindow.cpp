#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "conf/config.h"
#include "conf/lang.h"
#include "mainpresenter.h"
#include <QDebug>
#include <cassert>

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

    this->resize(800, 500);

    setPresenter(new MainPresenter(this));
}

MainWindow::~MainWindow()
{
    delete presenter;
    presenter = nullptr;

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto centralSize = ui->centralWidget->size();
    qDebug()<<centralSize;

    ui->listWidget->resize(QSize(ui->centralWidget->size().width(), ui->centralWidget->size().height() - ui->titleWidget->size().height()));
    for (auto itemInfo : itemInfos) {
        const int marginWidth = 20;
        itemInfo.item->ui->widget->resize(QSize(centralSize.width() - marginWidth, itemInfo.item->ui->widget->size().height()));
        itemInfo.qListWidgetItem->setSizeHint(itemInfo.item->ui->widget->size());
    }

    ui->titleWidget->resize(QSize(centralSize.width(), ui->titleWidget->size().height()));
}

void MainWindow::onItemClicked(chnum_t id)
{
    qDebug()<<"onItemClicked:"<<id;

    // todo:
    setChName(id, "abc");
    setState(id, CHState::NO);
}

void MainWindow::on_pbId_clicked()
{

}

void MainWindow::on_pbState_clicked()
{

}

void MainWindow::setChName(chnum_t ch, const char *name)
{
    assert(0<=ch && ch<=AllChNum-1);
    itemInfos.at(ch - 1).item->ui->label->setText(name);
}

void MainWindow::setState(chnum_t ch, CHState state)
{
    assert(0<=ch && ch<=AllChNum-1);

    qDebug()<<"state:"<<state;
    const char* image = "";
    switch (state) {
    case CHState::HIDE:
        image = ":/state/hide.png";
        break;
    case YES:
        image = ":/state/yes.png";
        break;
    case NO:
        image = ":/state/no.png";
        break;
    default:
        break;
    }
    itemInfos.at(ch - 1).item->ui->pushButton->setIcon(QIcon(image));
}
