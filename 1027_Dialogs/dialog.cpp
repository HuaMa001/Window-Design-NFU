#include "dialog.h"
#include "ui_dialog.h"
#include <QGridLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QColorDialog>
#include <QPalette>
#include <QFileDialog>
#include <QProgressDialog>
#include <QErrorMessage>
#include <QFontDialog>
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qpagesetupdialog.h>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QInputDialog>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    QGridLayout *gridLayout = new QGridLayout(this);

    displayTextEdit = new QTextEdit(QStringLiteral("TEXT"),this);

    colorPushButton = new QPushButton(QStringLiteral("顏色對話框"), this);
    errorPushButton = new QPushButton(QStringLiteral("錯誤對話框"), this);
    filePushButton = new QPushButton(QStringLiteral("檔案對話框"), this);
    fontPushButton = new QPushButton(QStringLiteral("字型對話框"), this);
    inputPushButton = new QPushButton(QStringLiteral("輸入對話框"), this);
    pagePushButton = new QPushButton(QStringLiteral("頁面設定"), this);
    progressPushButton = new QPushButton(QStringLiteral("進度條"), this);
    printPushButton = new QPushButton(QStringLiteral("列印設定"), this);
    settingPushButton = new QPushButton(QStringLiteral("設定前景顏色"), this);

    gridLayout->addWidget(colorPushButton, 0, 0, 1, 1);
    gridLayout->addWidget(errorPushButton, 0, 1, 1, 1);
    gridLayout->addWidget(filePushButton, 0, 2, 1, 1);
    gridLayout->addWidget(fontPushButton, 1, 0, 1, 1);
    gridLayout->addWidget(inputPushButton, 1, 1, 1, 1);
    gridLayout->addWidget(pagePushButton, 1, 2, 1, 1);
    gridLayout->addWidget(progressPushButton, 2, 0, 1, 1);
    gridLayout->addWidget(printPushButton, 2, 1, 1, 1);
    gridLayout->addWidget(settingPushButton, 2, 2, 1, 1);
    gridLayout->addWidget(displayTextEdit, 4, 0, 1, 3);

    setLayout(gridLayout);
    setWindowTitle(tr("內建對話盒展示"));
    resize(400, 300);

    connect(colorPushButton, SIGNAL(clicked()), this, SLOT(doPushButton()));
    connect(errorPushButton, SIGNAL(clicked()), this, SLOT(doPushButton()));
    connect(filePushButton, SIGNAL(clicked()), this, SLOT(doPushButton()));
    connect(fontPushButton, SIGNAL(clicked()), this, SLOT(doPushButton()));
    connect(inputPushButton, SIGNAL(clicked()), this, SLOT(doPushButton()));
    connect(pagePushButton, SIGNAL(clicked()), this, SLOT(doPushButton()));
    connect(progressPushButton, SIGNAL(clicked()), this, SLOT(doPushButton()));
    connect(printPushButton, SIGNAL(clicked()), this, SLOT(doPushButton()));
    connect(settingPushButton, SIGNAL(clicked()), this, SLOT(doTextColor()));

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::doPushButton()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn == colorPushButton)
    {
        QPalette palette = displayTextEdit->palette();
        const QColor color = QColorDialog::getColor(palette.color(QPalette::Base), this, tr("設定顏色背景"));
        if (color.isValid()) {
            palette.setColor(QPalette::Base, color);
            displayTextEdit->setPalette(palette);
        }
    }
    if(btn== errorPushButton)
    {
        QErrorMessage box(this);
        box.setWindowTitle(QStringLiteral("錯誤訊息盒"));
        box.showMessage(QStringLiteral("錯誤訊息盒實例xx:"));
        box.showMessage(QStringLiteral("錯誤訊息盒實例yy:"));
        box.showMessage(QStringLiteral("錯誤訊息盒實例zz:"));
        box.exec();

    }
    if(btn == filePushButton)
    {
        QString fileName= QFileDialog::getOpenFileName(this,tr("開啟檔案"),tr("."),
                                                        tr("任何檔案(*.*)"";;文字檔(*.txt)"";;XML(*.xml)"));
        displayTextEdit->setText(fileName);
    }
    if(btn== fontPushButton)
    {
        bool ok;
        const QFont &font=QFontDialog::getFont(&ok,displayTextEdit->font(),this,QStringLiteral("字體對話盒"));
        if(ok) displayTextEdit->setFont(font);
    }
    if(btn== inputPushButton)
    {
        bool ok;
        QString text = QInputDialog::getText(this,
                                             QStringLiteral("輸入對話盒"),
                                             QStringLiteral("輸入文字"),
                                             QLineEdit::Normal,
                                             QDir::home().dirName(),
                                             &ok);
        if(ok && !text.isEmpty()) displayTextEdit->setText(text);
    }
    if(btn== pagePushButton)
    {
        QPrinter printer(QPrinter::HighResolution);
        QPageSetupDialog* dig= new QPageSetupDialog(&printer,this);
        dig->setWindowTitle(QStringLiteral("頁面設定方塊"));
        if(dig->exec() == QDialog::Accepted)
        {

        }
    }
    if(btn== printPushButton)
    {
        QPrinter printer(QPrinter::HighResolution);
        QPrintDialog dialog(&printer,this);
        if(dialog.exec() != QDialog::Accepted)
            return;
    }
    if(btn== progressPushButton)
    {
        QProgressDialog progress (tr("正在複製檔案..."),tr("取消"),0,10000,this);
        progress.setWindowTitle(tr("進度條"));
        progress.show();
        for(int i=0;i<10000;i++)
        {
            progress.setValue(i);
            qApp->processEvents();
            qDebug()<<i;
        }

    }

}
void Dialog::doTextColor()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(btn == settingPushButton)
    {
        QPalette palette = displayTextEdit->palette();
        const QColor color = QColorDialog::getColor(palette.color(QPalette::Text), this, tr("設定顏色背景"));
        if (color.isValid()) {
            palette.setColor(QPalette::Text, color);
            displayTextEdit->setPalette(palette);
        }
    }
}

