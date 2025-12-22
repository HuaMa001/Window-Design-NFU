#include "mainwindow.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("影像處理"));
    central= new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300,200);
    initPixmap->fill(QColor(255,255,255));
    imgWin->resize(300,200);
    imgWin->setScaledContents(true);
    imgWin->setPixmap(*initPixmap);
    mainLayout->addWidget(imgWin);
    setCentralWidget(central);
    createActions();
    createMenu();
    createToolBars();
}

MainWindow::~MainWindow() {}

void MainWindow:: createMenu()
{
    fileMenu= menuBar()->addMenu(tr("檔案 F"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(exitAction);
}
void MainWindow:: createActions()
{
    openFileAction=new QAction(tr("開啟檔案 O"),this);
    openFileAction->setShortcut(tr("Crtl+O"));
    openFileAction->setStatusTip(tr("開啟影像檔案"));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(showOpenFile()));

    exitAction = new QAction(tr("結束 Q"),this);
    exitAction->setShortcut(tr("Crtl+Q"));
    exitAction->setStatusTip(tr("退出程式"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
}
void MainWindow:: createToolBars()
{
    fileTool=addToolBar("file");
    fileTool->addAction(openFileAction);
}
void MainWindow:: loadFile(QString filename)
{
    qDebug()<<QString("file name:%1").arg(filename);
    QByteArray ba = filename.toLatin1();
    printf("FN:%s \n",(char *) ba.data());
    img.load(filename);
    imgWin->setPixmap(QPixmap::fromImage(img));
}
void MainWindow:: showOpenFile()
{
    filename = QFileDialog::getOpenFileName(this,tr("開啟影像"),tr("."),"bmp(*.bmp);;png(*.png)"";;Jpeg(*.jpg)");
    if(!filename.isNull())
    {
        loadFile(filename);
    }
    else
    {
       MainWindow *newIPWin = new MainWindow();
        newIPWin->show();
         newIPWin->loadFile(filename);
    }
}
