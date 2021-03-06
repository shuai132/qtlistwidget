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
        connect(uiItem, SIGNAL(chNameChanged(chnum_t,QString)), this, SLOT(onChNameChanged(chnum_t,QString)));
    }

    this->resize(800, 500);

    setPresenter(new MainPresenter(this));
}

MainWindow::~MainWindow()
{
    delete presenter;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug()<<event;

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

void MainWindow::onItemClicked(chnum_t ch)
{
    qDebug()<<"onItemClicked:"<<ch;
}

void MainWindow::onChNameChanged(chnum_t ch, QString name)
{
    qDebug()<<__func__<<name;
    presenter->restoreChName(ch, name);
}

void MainWindow::setChName(chnum_t ch, QString name)
{
    assert(ChNumMin<=ch && ch<=AllChNum);
    itemInfos.at(ch - 1).item->ui->leName->setText(name);
}

void MainWindow::setState(chnum_t ch, ChState state)
{
    assert(ChNumMin<=ch && ch<=AllChNum);

    QString image;
    QString title;
    switch (state) {
    case ChState::HIDE:
        image = ":/state/hide.png";
        title = Lang::hide;
        break;
    case ChState::YES:
        image = ":/state/yes.png";
        title = Lang::online;
        break;
    case ChState::NO:
        image = ":/state/no.png";
        title = Lang::lost;
        break;
    default:
        break;
    }
    auto pb = itemInfos.at(ch - 1).item->ui->pbState;
    pb->setIcon(QIcon(image));
    pb->setText(title);
}

void MainWindow::setConState(bool isConnected)
{
    ui->lbConState->setText(isConnected ? Lang::connected : Lang::notconnected);
}

