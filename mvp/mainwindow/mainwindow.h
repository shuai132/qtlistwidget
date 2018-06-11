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
    void onItemClicked(chnum_t ch);

    void onChNameChanged(chnum_t ch, QString name);

private:
    Ui::MainWindow *ui;
    std::vector<ItemInfo> itemInfos;
    ChNameChangeListener chNameChangeListener = nullptr;

    // View interface
private:
    void setChName(chnum_t ch, QString name) override;
    void setState(chnum_t ch, ChState state) override;
    void setConState(bool isConnected) override;

    void setChNameChangeListener(ChNameChangeListener listener) override;
};

#endif // MAINWINDOW_H
