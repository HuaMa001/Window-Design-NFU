#include "widget.h"
#include <QPushButton>
#include <QHBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    helloButton = new QPushButton(QStringLiteral("&Hello 您好"),this);
    QHBoxLayout *layout= new QHBoxLayout;
    layout->addWidget(helloButton);
    setLayout(layout);

}

Widget::~Widget() {}
