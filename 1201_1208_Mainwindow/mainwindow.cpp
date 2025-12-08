#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    connect(actionSave, &QAction::triggered, this, &MainWindow::SaveTxtFile);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::OpenTxtFile);

}

MainWindow::~MainWindow() {}
// 打開文字檔案並顯示在 textEdit
void MainWindow::OpenTxtFile()
{
    // 打開檔案選擇對話框，讓使用者選擇要打開的文字檔
    QString fileName = QFileDialog::getOpenFileName(
        this,                            // 對話框父視窗
        tr("Open Text File"),             // 對話框標題
        "",                               // 初始路徑（空字串表示當前目錄）
        tr("Text Files (*.txt);;All Files (*)")  // 檔案類型篩選
        );

    // 如果使用者按取消，fileName 為空，直接返回
    if (fileName.isEmpty())
        return;

    // 用 QFile 操作選擇的檔案
    QFile file(fileName);

    // 嘗試以唯讀文字模式打開檔案
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // 打開失敗，彈出警告視窗提示錯誤
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot open file for reading: %1").arg(file.errorString())
                             );
        return;
    }

    // 用 QTextStream 讀取文字檔內容
    QTextStream in(&file);
    // 將讀取到的文字設定到 QTextEdit 控制項中
    textEdit->setPlainText(in.readAll());

    // 關閉檔案
    file.close();
}

// 將 textEdit 中的文字存成文字檔
void MainWindow::SaveTxtFile()
{
    // 打開保存檔案對話框，讓使用者選擇檔案名稱與路徑
    QString fileName = QFileDialog::getSaveFileName(
        this,                            // 對話框父視窗
        tr("Save Text File"),             // 對話框標題
        "",                               // 初始路徑（空字串表示當前目錄）
        tr("Text Files (*.txt);;All Files (*)")  // 檔案類型篩選
        );

    // 如果使用者按取消，fileName 為空，直接返回
    if (fileName.isEmpty())
        return;

    // 用 QFile 操作選擇的檔案
    QFile file(fileName);

    // 嘗試以寫入文字模式打開檔案
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // 打開失敗，彈出警告視窗提示錯誤
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot open file for writing: %1").arg(file.errorString())
                             );
        return;
    }

    // 用 QTextStream 將文字寫入檔案
    QTextStream out(&file);
    out << textEdit->toPlainText();  // 取得 QTextEdit 中的全部文字並寫入檔案

    // 關閉檔案
    file.close();
}

