#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QScreen>
#include <QPixmap>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "ui_mainwindow.h"
class MainWindow : public QMainWindow,public Ui_MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionScreenshot_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();

private:
    QString currentFilePath;
};
#endif // MAINWINDOW_H
