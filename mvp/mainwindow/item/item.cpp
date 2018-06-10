#include "item.h"
#include <QDebug>

Item::Item(chnum_t id)
    :id(id), ui(new Ui::Item)
{
    ui->setupUi(this);

    ui->label->setText(QString("label%1").arg(id));
}

Item::~Item()
{
    delete ui;
}

void Item::on_pushButton_clicked()
{
    qDebug()<<"clicke id:"<<this->id;
    emit itemClicked(this->id);
}
