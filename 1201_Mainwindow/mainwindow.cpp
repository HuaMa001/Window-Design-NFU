#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
}

MainWindow::~MainWindow() {}

void MainWindow::on_actionScreenshot_triggered()
{
    // Capture the current window
    QPixmap screenshot = this->grab();
    
    // Open file dialog to save the screenshot
    QString fileName = QFileDialog::getSaveFileName(this, 
        tr("保存截圖"), 
        "", 
        tr("PNG 圖片 (*.png);;JPEG 圖片 (*.jpg *.jpeg);;所有文件 (*)"));
    
    if (!fileName.isEmpty()) {
        // Determine format from file extension using QFileInfo
        QFileInfo fileInfo(fileName);
        QString suffix = fileInfo.suffix().toLower();
        
        // If no extension is provided, add .png
        if (suffix.isEmpty()) {
            fileName += ".png";
            suffix = "png";
        }
        
        // Determine the format for saving
        const char* format = nullptr;
        if (suffix == "png") {
            format = "PNG";
        } else if (suffix == "jpg" || suffix == "jpeg") {
            format = "JPG";
        }
        
        if (screenshot.save(fileName, format)) {
            QMessageBox::information(this, tr("成功"), tr("截圖已保存到: ") + fileName);
        } else {
            QMessageBox::warning(this, tr("錯誤"), tr("無法保存截圖"));
        }
    }
}
