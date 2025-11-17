#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QTextEdit* displayTextEdit;
    QPushButton* colorPushButton;
    QPushButton* errorPushButton;
    QPushButton* filePushButton;
    QPushButton* fontPushButton;
    QPushButton* inputPushButton;
    QPushButton* pagePushButton;
    QPushButton* progressPushButton;
    QPushButton* printPushButton;
    QPushButton* displayPushButton;
    QPushButton* settingPushButton;

private slots:
    void doPushButton();
    void doTextColor();

};
#endif // DIALOG_H
