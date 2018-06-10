#include "item.h"
#include <QDebug>

Item::Item(chnum_t ch)
    :ch(ch), ui(new Ui::Item)
{
    ui->setupUi(this);
    ui->lbId->setText(QString::number(ch));
}

Item::~Item()
{
    delete ui;
}

void Item::on_leName_editingFinished()
{
    qDebug()<<"on_lineEdit_editingFinished";

    auto leName = this->ui->leName;
    emit chNameChanged(ch, leName->text().toStdString().c_str());

    // NOTE: for lost focus
    leName->setEnabled(false);
    leName->setEnabled(true);
}

void Item::on_pbState_clicked()
{
    qDebug()<<"clicke ch:"<<this->ch;
    emit itemClicked(this->ch);
}
