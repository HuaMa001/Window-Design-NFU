#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include<QPushButton>
#include<QDial>
#include<QSpacerItem>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QImage>
#include<QMouseEvent>
#include<QColorDialog>
#include<QSpinBox>

class ImageProcess : public QWidget
{
    Q_OBJECT

public:
   ImageProcess(QWidget *parent = nullptr);
    ~ImageProcess();
    QLabel *inWin;
    QGroupBox *mirrorGroup;
    QCheckBox *vCheckBox;
    QCheckBox *hCheckBox;
    QPushButton *mirrorButton;
    QPushButton *saveButton;
    QDial *rotateDial;
    QSpacerItem *vSpacer;
    QHBoxLayout *mainLayout;
    QVBoxLayout *groupLayout;
    QVBoxLayout *leftLayout;
    QImage srcImg;
    QImage dstImg;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void mirrorImage();
    void rotatedImage();
    void saveImage();
    void chooseBrushColor();
    void onBrushSizeChanged(int value);
    
private:
    // Brush functionality
    bool isDrawing;
    QPoint lastDrawPoint;
    QColor brushColor;
    int brushSize;
    
    QPushButton *brushColorButton;
    QSpinBox *brushSizeSpinBox;
    QGroupBox *brushGroup;
    
    void drawLineTo(const QPoint &endPoint);
};
#endif // IMAGEPROCESS_H
