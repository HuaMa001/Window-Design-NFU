#include "mainwindow.h"
#include <QPalette>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    // 設定暗色調
    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(42,42,42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66,66,66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);

    darkPalette.setColor(QPalette::Highlight, QColor(42,130,218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);
}

MainWindow::~MainWindow() {}

void MainWindow::on_actionSave_triggered()
{
    QString fileName;
    
    // If we have a current file, save to it directly
    if (!currentFilePath.isEmpty()) {
        fileName = currentFilePath;
    } else {
        // Otherwise, ask user where to save
        fileName = QFileDialog::getSaveFileName(this, 
            tr("儲存檔案"), 
            "", 
            tr("文字檔案 (*.txt);;所有文件 (*)"));
        
        if (fileName.isEmpty()) {
            return; // User cancelled
        }
        
        // Save the file path for future saves
        currentFilePath = fileName;
    }
    
    // Open file for writing
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("錯誤"), tr("無法開啟檔案進行寫入"));
        return;
    }
    
    // Write content to file
    QTextStream out(&file);
    out << textEdit->toPlainText();
    file.close();
    
    QMessageBox::information(this, tr("成功"), tr("檔案已儲存"));
}

void MainWindow::on_actionOpen_triggered()
{
    // Open file dialog to select a file
    QString fileName = QFileDialog::getOpenFileName(this, 
        tr("開啟檔案"), 
        "", 
        tr("文字檔案 (*.txt);;所有文件 (*)"));
    
    if (fileName.isEmpty()) {
        return; // User cancelled
    }
    
    // Open file for reading
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("錯誤"), tr("無法開啟檔案進行讀取"));
        return;
    }
    
    // Read content from file
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    // Set the content to the text edit
    textEdit->setPlainText(content);
    
    // Save the file path for future saves
    currentFilePath = fileName;
    
    QMessageBox::information(this, tr("成功"), tr("檔案已開啟"));
}

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
