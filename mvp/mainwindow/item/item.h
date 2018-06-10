#ifndef ITEM_H
#define ITEM_H

#include "ui_item.h"
#include "conf/config.h"

class Item : public QWidget
{
    Q_OBJECT

public:
    explicit Item(chnum_t id);
    ~Item();

public:
    chnum_t id;
    Ui::Item* ui;

signals:
    void itemClicked(chnum_t id);

private slots:
    void on_pushButton_clicked();
};

#endif // ITEM_H
