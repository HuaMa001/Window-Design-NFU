#include "ImageProcess.h"
#include <QPixmap>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

ImageProcess::ImageProcess(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    leftLayout = new QVBoxLayout(this);
    mirrorGroup = new QGroupBox(QStringLiteral("鏡射"),this);
    groupLayout = new QVBoxLayout(mirrorGroup);

    hCheckBox = new QCheckBox(QStringLiteral("水平"),mirrorGroup);
    vCheckBox = new QCheckBox(QStringLiteral("垂直"),mirrorGroup);
    mirrorButton = new QPushButton(QStringLiteral("執行"),mirrorGroup);
    saveButton = new QPushButton(tr("儲存"));

    hCheckBox->setGeometry(QRect(13,28,87,19));
    hCheckBox->setGeometry(QRect(13,54,87,19));
    mirrorButton->setGeometry(QRect(13,80,93,28));
    groupLayout->addWidget(hCheckBox);
    groupLayout->addWidget(vCheckBox);
    groupLayout->addWidget(mirrorButton);
    leftLayout->addWidget(mirrorGroup);
    rotateDial = new QDial(this);
    rotateDial->setNotchTarget(true);
    vSpacer= new QSpacerItem(20,58,QSizePolicy::Minimum,QSizePolicy::Expanding);

    leftLayout->addWidget(rotateDial);
    leftLayout->addWidget(saveButton);
    leftLayout->addItem(vSpacer);
    mainLayout->addLayout(leftLayout);

    inWin=new QLabel(this);
    inWin->setScaledContents(true);
    QPixmap *initPixmap = new QPixmap(300,200);
    initPixmap->fill(QColor(255,255,255));

    QPainter *paint = new QPainter(initPixmap);
    paint->setPen(*(new QColor(0,0,0)));
    paint->begin(initPixmap);
    paint->drawRect(15,15,60,40);
    paint->end();

    inWin->setPixmap(*initPixmap);
    inWin->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    if(srcImg.isNull())
    {
        srcImg =initPixmap->toImage();
        /*
        QPixmap *initPixmap = new QPixmap(300,200);
        initPixmap ->fill(QColor(255,255,255));
        inWin->setPixmap(*initPixmap);*/
    }
    mainLayout->addWidget(inWin);
    connect(mirrorButton,SIGNAL(clicked()),this,SLOT(mirrorImage()));
    connect(rotateDial,SIGNAL(valueChanged(int)),this,SLOT(rotatedImage()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveImage()));

}

ImageProcess::~ImageProcess() {}
void ImageProcess::mirrorImage()
{
    bool H,V;
    if(srcImg.isNull())
        return;
    H=hCheckBox->isChecked();
    V=vCheckBox->isChecked();
    dstImg=srcImg.mirrored(H,V);
    srcImg=dstImg;
}
void ImageProcess::rotatedImage()
{
    QTransform tran;
    int angle;
    if(srcImg.isNull())
        return;
    angle=rotateDial->value();
    tran.rotate(angle);
    dstImg = srcImg.transformed(tran);
    inWin->setPixmap(QPixmap::fromImage(dstImg));
}
void ImageProcess::saveImage()
{
    if (dstImg.isNull() && srcImg.isNull())
        return;

    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("儲存影像"),
        "",
        tr("PNG (*.png);;JPG (*.jpg);;BMP (*.bmp)")
        );

    if (fileName.isEmpty())
        return;

<<<<<<< HEAD:1222_1229_ImageProcess/ImageProcess.cpp

=======
    
>>>>>>> c4dc1e7297dacd1f38c4e416ea5a2f5d13c899cd:1222_ImageProcess/widget.cpp
    QImage imgToSave = dstImg.isNull() ? srcImg : dstImg;

    if (!imgToSave.save(fileName))
    {
        QMessageBox::warning(this, tr("錯誤"), tr("儲存失敗"));
    }
}
