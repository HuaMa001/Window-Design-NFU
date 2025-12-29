#include "ImageProcess.h"
#include <QPixmap>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QLabel>

ImageProcess::ImageProcess(QWidget *parent)
    : QWidget(parent), isDrawing(false), brushColor(Qt::red), brushSize(3)
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
    
    // Add brush controls
    brushGroup = new QGroupBox(QStringLiteral("畫筆工具"), this);
    QVBoxLayout *brushLayout = new QVBoxLayout(brushGroup);
    
    brushColorButton = new QPushButton(QStringLiteral("選擇顏色"), brushGroup);
    brushColorButton->setStyleSheet(QString("background-color: %1").arg(brushColor.name()));
    
    QLabel *sizeLabel = new QLabel(QStringLiteral("筆刷大小:"), brushGroup);
    brushSizeSpinBox = new QSpinBox(brushGroup);
    brushSizeSpinBox->setMinimum(1);
    brushSizeSpinBox->setMaximum(50);
    brushSizeSpinBox->setValue(brushSize);
    
    brushLayout->addWidget(brushColorButton);
    brushLayout->addWidget(sizeLabel);
    brushLayout->addWidget(brushSizeSpinBox);
    leftLayout->addWidget(brushGroup);
    
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
    connect(brushColorButton, SIGNAL(clicked()), this, SLOT(chooseBrushColor()));
    connect(brushSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        brushSize = value;
    });
    
    // Enable mouse tracking for the image label
    inWin->setMouseTracking(true);
    inWin->installEventFilter(this);

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


    QImage imgToSave = dstImg.isNull() ? srcImg : dstImg;

    if (!imgToSave.save(fileName))
    {
        QMessageBox::warning(this, tr("錯誤"), tr("儲存失敗"));
    }
}

void ImageProcess::chooseBrushColor()
{
    QColor color = QColorDialog::getColor(brushColor, this, tr("選擇畫筆顏色"));
    if (color.isValid())
    {
        brushColor = color;
        brushColorButton->setStyleSheet(QString("background-color: %1").arg(brushColor.name()));
    }
}

void ImageProcess::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && inWin->underMouse())
    {
        // Map to inWin coordinates
        QPoint pos = inWin->mapFrom(this, event->pos());
        if (inWin->rect().contains(pos) && !srcImg.isNull())
        {
            isDrawing = true;
            lastDrawPoint = pos;
        }
    }
}

void ImageProcess::mouseMoveEvent(QMouseEvent *event)
{
    if (isDrawing && (event->buttons() & Qt::LeftButton))
    {
        QPoint pos = inWin->mapFrom(this, event->pos());
        if (!srcImg.isNull())
        {
            drawLineTo(pos);
            lastDrawPoint = pos;
        }
    }
}

void ImageProcess::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDrawing)
    {
        isDrawing = false;
    }
}

void ImageProcess::drawLineTo(const QPoint &endPoint)
{
    // Get the current image to draw on
    QImage *drawImage = &srcImg;
    if (drawImage->isNull())
        return;
    
    // Map widget coordinates to image coordinates
    double scaleX = (double)drawImage->width() / inWin->width();
    double scaleY = (double)drawImage->height() / inWin->height();
    
    QPoint imgStart(lastDrawPoint.x() * scaleX, lastDrawPoint.y() * scaleY);
    QPoint imgEnd(endPoint.x() * scaleX, endPoint.y() * scaleY);
    
    // Draw on the image
    QPainter painter(drawImage);
    painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(imgStart, imgEnd);
    painter.end();
    
    // Update display
    inWin->setPixmap(QPixmap::fromImage(*drawImage));
    dstImg = *drawImage; // Update dstImg for saving
}

bool ImageProcess::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == inWin)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton && !srcImg.isNull())
            {
                isDrawing = true;
                lastDrawPoint = mouseEvent->pos();
                return true;
            }
        }
        else if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (isDrawing && (mouseEvent->buttons() & Qt::LeftButton))
            {
                drawLineTo(mouseEvent->pos());
                lastDrawPoint = mouseEvent->pos();
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                isDrawing = false;
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
