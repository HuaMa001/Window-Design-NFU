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

private slots:
    void mirrorImage();
    void rotatedImage();
    void saveImage();
};
#endif // IMAGEPROCESS_H
