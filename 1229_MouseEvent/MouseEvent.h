#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "../1222_1229_ImageProcess/ImageProcess.h"
#include <QMainWindow>
#include <QMouseEvent>
#include <QStatusBar>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QImage>

class MouseEvent : public QMainWindow
{
    Q_OBJECT

public:
    MouseEvent(QWidget *parent = nullptr);
    ~MouseEvent();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void createActions();
    void createToolBars();
    void loadFile(const QString &filename);

private slots:
    void showOpenFile();
    void showGeometryTransform();

private:
    ImageProcess *gWin;

    QLabel *statusLabel;
    QLabel *MousePosLabel;
    QLabel *imgWin;

    QToolBar *fileTool;
    QAction *openFileAction;
    QAction *geometryAction;
    QAction *exitAction;

    QImage img;
    QString filename;
};

#endif // MOUSEEVENT_H
