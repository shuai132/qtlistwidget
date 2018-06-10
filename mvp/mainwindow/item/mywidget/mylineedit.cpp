#include "mylineedit.h"
#include <QDebug>

MyLineEdit::MyLineEdit(QWidget* parent)
    :QLineEdit(parent)
{

}

void MyLineEdit::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<event;
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QLineEdit::mousePressEvent(event);
}
