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
void MainWindow::OpenTxtFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text File"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot open file for reading: %1").arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());
    file.close();
}
void MainWindow::SaveTxtFile()
{
    // 打開保存檔案對話框
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Text File"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot open file for writing: %1").arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out << textEdit->toPlainText();  // 將 textEdit 文字寫入檔案
    file.close();
}
