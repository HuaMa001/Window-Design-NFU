#include "MouseEvent.h"
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QPixmap>
#include <QPainter>
#include <QPen>

MouseEvent::MouseEvent(QWidget *parent)
    : QMainWindow(parent), isSelecting(false)
{
    setWindowTitle(tr("測試視窗"));

    gWin = new ImageProcess();

    // 初始化影像顯示區
    imgWin = new QLabel(this);
    QPixmap initPixmap(300, 200);
    initPixmap.fill(Qt::white);
    imgWin->setPixmap(initPixmap);
    imgWin->setScaledContents(true);
    imgWin->resize(300, 200);
    setCentralWidget(imgWin);

    // 狀態列
    statusLabel = new QLabel(tr("指標位址"), this);
    statusLabel->setFixedWidth(100);
    MousePosLabel = new QLabel(tr(""), this);
    MousePosLabel->setFixedWidth(100);
    statusBar()->addPermanentWidget(statusLabel);
    statusBar()->addPermanentWidget(MousePosLabel);

    setMouseTracking(true);
    resize(400, 300);

    createActions();
    createToolBars();
}

MouseEvent::~MouseEvent() {}

void MouseEvent::createActions()
{
    // 開啟檔案
    openFileAction = new QAction(tr("開啟檔案 O"), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("開啟影像檔案"));
    connect(openFileAction, &QAction::triggered, this, &MouseEvent::showOpenFile);

    // 幾何轉換
    geometryAction = new QAction(tr("幾何轉換"), this);
    geometryAction->setShortcut(tr("Ctrl+G"));
    geometryAction->setStatusTip(tr("影像幾何轉換"));
    connect(geometryAction, &QAction::triggered, this, &MouseEvent::showGeometryTransform);

    // 退出
    exitAction = new QAction(tr("退出"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("關閉程式"));
    connect(exitAction, &QAction::triggered, gWin, &QWidget::close);
}

void MouseEvent::createToolBars()
{
    fileTool = addToolBar(tr("File"));
    fileTool->addAction(openFileAction);
    fileTool->addAction(geometryAction);
    fileTool->addAction(exitAction);
}

void MouseEvent::showOpenFile()
{
    filename = QFileDialog::getOpenFileName(
        this,
        tr("開啟影像"),
        tr("."),
        "bmp (*.bmp);;png (*.png);;jpg (*.jpg)"
        );

    if (!filename.isNull())
    {
        loadFile(filename);
    }
    else
    {
        statusBar()->showMessage(tr("沒有選擇檔案"));
    }
}

void MouseEvent::showGeometryTransform()
{
    if (!img.isNull())
    {
        gWin->srcImg = img;
        gWin->inWin->setPixmap(QPixmap::fromImage(gWin->srcImg));
        gWin->show();
    }
    else
    {
        statusBar()->showMessage(tr("影像尚未載入"));
    }
}

void MouseEvent::loadFile(const QString &filename)
{
    if (!QFile::exists(filename))
    {
        statusBar()->showMessage(tr("檔案不存在"));
        return;
    }

    if (!img.load(filename))
    {
        statusBar()->showMessage(tr("讀取影像失敗"));
        return;
    }

    imgWin->setPixmap(QPixmap::fromImage(img));
    statusBar()->showMessage(tr("已載入: ") + filename);
}

// 滑鼠事件
void MouseEvent::mouseMoveEvent(QMouseEvent *event)
{
    QString str = QString("(%1,%2)").arg(event->x()).arg(event->y());
    MousePosLabel->setText(str);
    
    // Update selection rectangle during drag
    if (isSelecting && !img.isNull())
    {
        selectionEnd = event->pos();
        update(); // Trigger repaint to show selection rectangle
    }
}

void MouseEvent::mousePressEvent(QMouseEvent *event)
{
    QString str = QString("(%1,%2)").arg(event->x()).arg(event->y());
    if (event->button() == Qt::LeftButton)
    {
        statusBar()->showMessage(tr("左鍵") + str);
        
        // Start selection if image is loaded
        if (!img.isNull())
        {
            isSelecting = true;
            selectionStart = event->pos();
            selectionEnd = event->pos();
        }
    }
    else if (event->button() == Qt::RightButton)
        statusBar()->showMessage(tr("右鍵") + str);
    else if (event->button() == Qt::MiddleButton)
        statusBar()->showMessage(tr("中鍵") + str);
}

void MouseEvent::mouseReleaseEvent(QMouseEvent *event)
{
    QString str = QString("(%1,%2)").arg(event->x()).arg(event->y());
    statusBar()->showMessage(tr("釋放") + str);
    
    // Complete selection and zoom
    if (isSelecting && event->button() == Qt::LeftButton && !img.isNull())
    {
        isSelecting = false;
        selectionEnd = event->pos();
        
        // Calculate selection rectangle
        int x1 = qMin(selectionStart.x(), selectionEnd.x());
        int y1 = qMin(selectionStart.y(), selectionEnd.y());
        int x2 = qMax(selectionStart.x(), selectionEnd.x());
        int y2 = qMax(selectionStart.y(), selectionEnd.y());
        
        selectionRect = QRect(x1, y1, x2 - x1, y2 - y1);
        
        // Only zoom if selection is large enough
        if (selectionRect.width() > 10 && selectionRect.height() > 10)
        {
            // Map selection from widget coordinates to image coordinates
            QRect imgRect = imgWin->rect();
            
            // Calculate the actual image display area
            double scaleX = (double)img.width() / imgRect.width();
            double scaleY = (double)img.height() / imgRect.height();
            
            // Adjust selection to account for toolbar and status bar
            int toolbarHeight = fileTool->height();
            int statusBarHeight = statusBar()->height();
            
            int adjY1 = y1 - toolbarHeight;
            int adjY2 = y2 - toolbarHeight;
            
            // Only process if selection is within image area
            if (adjY1 >= 0 && adjY2 > 0)
            {
                int imgX = qMax(0, (int)(x1 * scaleX));
                int imgY = qMax(0, (int)(adjY1 * scaleY));
                int imgW = qMin(img.width() - imgX, (int)((x2 - x1) * scaleX));
                int imgH = qMin(img.height() - imgY, (int)((adjY2 - adjY1) * scaleY));
                
                if (imgW > 0 && imgH > 0)
                {
                    // Extract and zoom the selected region
                    QImage selectedRegion = img.copy(imgX, imgY, imgW, imgH);
                    
                    // Open geometry transform window with zoomed image
                    gWin->srcImg = selectedRegion;
                    gWin->inWin->setPixmap(QPixmap::fromImage(gWin->srcImg));
                    gWin->show();
                    
                    statusBar()->showMessage(tr("已放大選取區域"));
                }
            }
        }
        
        update(); // Clear selection rectangle
    }
}

void MouseEvent::mouseDoubleClickEvent(QMouseEvent *event)
{
    QString str = QString("(%1,%2)").arg(event->x()).arg(event->y());
    statusBar()->showMessage(tr("雙擊") + str);
}

void MouseEvent::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    
    // Draw selection rectangle during drag
    if (isSelecting && !img.isNull())
    {
        QPainter painter(this);
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        
        int x1 = qMin(selectionStart.x(), selectionEnd.x());
        int y1 = qMin(selectionStart.y(), selectionEnd.y());
        int x2 = qMax(selectionStart.x(), selectionEnd.x());
        int y2 = qMax(selectionStart.y(), selectionEnd.y());
        
        painter.drawRect(x1, y1, x2 - x1, y2 - y1);
    }
}
