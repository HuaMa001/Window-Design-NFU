#include "widget.h"
#include <QGridLayout>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    storedValue(0.0)
{
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setText("0");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(display, 0, 0, 1, 4);

    // top row operators: <- / * -
    QString topOps[4] = { "<-", "/", "*", "-" };
    for(int i = 0; i < 4; i++)
    {
        QPushButton *btn = new QPushButton(topOps[i], this);
        if(topOps[i] == "<-")
            connect(btn, &QPushButton::clicked, this, &Widget::clearClicked);
        else
            connect(btn, &QPushButton::clicked, this, &Widget::opClicked);
        layout->addWidget(btn, 1, i);
    }

    // numeric buttons 1~9
    int numbers[9] = {1,2,3,4,5,6,7,8,9};
    for(int i = 0; i < 9; i++)
    {
        int n = numbers[i];
        QPushButton *btn = new QPushButton(QString::number(n), this);
        connect(btn, &QPushButton::clicked, this, &Widget::numberClicked);

        int row = 2 + i/3;    // row 2,3,4
        int col = i%3;        // col 0,1,2
        layout->addWidget(btn, row, col);
    }

    // plus button (+) 跨2行，放在數字區右邊
    QPushButton *plusBtn = new QPushButton("+", this);
    connect(plusBtn, &QPushButton::clicked, this, &Widget::opClicked);
    layout->addWidget(plusBtn, 2, 3, 2, 1); // row=2, col=3, rowSpan=2

    // zero button (0) 跨2列
    QPushButton *zeroBtn = new QPushButton("0", this);
    connect(zeroBtn, &QPushButton::clicked, this, &Widget::numberClicked);
    layout->addWidget(zeroBtn, 5, 0, 1, 2); // row=5, col=0, colSpan=2

    // dot button
    QPushButton *dotBtn = new QPushButton(".", this);
    connect(dotBtn, &QPushButton::clicked, this, &Widget::numberClicked);
    layout->addWidget(dotBtn, 5, 2);

    // equal button (=) 跨2行，對齊 plus 按鈕右側
    QPushButton *eqBtn = new QPushButton("=", this);
    connect(eqBtn, &QPushButton::clicked, this, &Widget::equalClicked);
    layout->addWidget(eqBtn, 4, 3, 2, 1); // row=4, col=3, rowSpan=2
}

void Widget::numberClicked()
{
    QString val = ((QPushButton*)sender())->text();
    if(display->text() == "0")
        display->setText(val);
    else
        display->setText(display->text() + val);
}

void Widget::opClicked()
{
    storedValue = display->text().toDouble();
    pendingOp = ((QPushButton*)sender())->text();
    display->setText("0");
}

void Widget::equalClicked()
{
    double v = display->text().toDouble();
    double r = 0;

    if(pendingOp == "+") r = storedValue + v;
    if(pendingOp == "-") r = storedValue - v;
    if(pendingOp == "*") r = storedValue * v;
    if(pendingOp == "/") r = storedValue / v;

    display->setText(QString::number(r));
}

void Widget::clearClicked()
{
    storedValue = 0;
    pendingOp.clear();
    display->setText("0");
}
