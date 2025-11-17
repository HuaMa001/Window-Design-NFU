#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

private slots:
    void numberClicked();
    void opClicked();
    void equalClicked();
    void clearClicked();

private:
    QLineEdit *display;

    double storedValue;
    QString pendingOp;
};

#endif
