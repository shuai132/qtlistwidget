#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "item/item.h"
#include "conf/config.h"
#include "maincontract.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public MainContract::View
{
    Q_OBJECT

public:
    struct ItemInfo {
        ItemInfo(QListWidgetItem* qListWidgetItem, Item* item)
            :qListWidgetItem(qListWidgetItem), item(item)
        {}
        QListWidgetItem* qListWidgetItem;
        Item* item;
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void onItemClicked(chnum_t id);

    void on_pbId_clicked();

    void on_pbState_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<ItemInfo> itemInfos;

    // View interface
private:
    void setChName(chnum_t ch, const char *name) override;
    void setState(chnum_t ch, CHState state) override;
};

#endif // MAINWINDOW_H
