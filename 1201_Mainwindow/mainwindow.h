#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QScreen>
#include <QPixmap>
#include <QMessageBox>
#include "ui_mainwindow.h"
class MainWindow : public QMainWindow,public Ui_MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionScreenshot_triggered();
};
#endif // MAINWINDOW_H
