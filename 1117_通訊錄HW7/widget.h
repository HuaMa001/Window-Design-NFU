#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnAdd_clicked();
    void on_btnExport_clicked();
    void on_btnImport_clicked();
    void on_btnExit_clicked();
    void on_btnDelete_clicked();
    void on_btnEdit_clicked();
    void on_btnClear_clicked();
    void on_lineEditSearch_textChanged(const QString &text);

private:
    Ui::Widget *ui;
    
    // Helper functions
    bool validateInputs();
    void clearInputs();
    void showErrorMessage(const QString &message);
    void showInfoMessage(const QString &message);
    void loadRowToInputs(int row);
};
#endif // WIDGET_H
