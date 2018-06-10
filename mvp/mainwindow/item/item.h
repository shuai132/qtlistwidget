#ifndef ITEM_H
#define ITEM_H

#include "ui_item.h"
#include "conf/config.h"

class Item : public QWidget
{
    Q_OBJECT

public:
    explicit Item(chnum_t ch);
    ~Item();

public:
    chnum_t ch;
    Ui::Item* ui;

signals:
    void itemClicked(chnum_t ch);
    void chNameChanged(chnum_t ch, const char *name);

private slots:
    void on_leName_editingFinished();
    void on_pbState_clicked();
};

#endif // ITEM_H
