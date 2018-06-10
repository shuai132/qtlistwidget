#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QMouseEvent>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit MyLineEdit(QWidget* parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();
};

#endif // MYLINEEDIT_H
