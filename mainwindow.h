#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "item.h"
#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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

private:
    Ui::MainWindow *ui;
    std::vector<ItemInfo> itemInfos;
};

#endif // MAINWINDOW_H
