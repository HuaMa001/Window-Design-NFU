#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

void writeString(QString Filename,QString str)
{
    QFile mFile(Filename);
    if(! mFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"Error opening file for writing";
        return;
    }
    QTextStream out(&mFile);
    out<<str;
    out.flush();
    mFile.close();
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    
    // Setup table widget
    QStringList colTitle;
    ui->tableWidget->setColumnCount(4);
    colTitle<<"學號"<<"班級"<<"姓名"<<"電話";
    ui->tableWidget->setHorizontalHeaderLabels(colTitle);
    
    // Set column widths
    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 150);
    ui->tableWidget->setColumnWidth(3, 200);
    
    // Enable alternating row colors for better readability
    ui->tableWidget->setAlternatingRowColors(true);
    
    // Connect search functionality
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &Widget::on_lineEditSearch_textChanged);
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::validateInputs()
{
    if(ui->lineEdit->text().trimmed().isEmpty())
    {
        showErrorMessage("請輸入學號！");
        ui->lineEdit->setFocus();
        return false;
    }
    
    if(ui->lineEdit_2->text().trimmed().isEmpty())
    {
        showErrorMessage("請輸入班級！");
        ui->lineEdit_2->setFocus();
        return false;
    }
    
    if(ui->lineEdit_3->text().trimmed().isEmpty())
    {
        showErrorMessage("請輸入姓名！");
        ui->lineEdit_3->setFocus();
        return false;
    }
    
    if(ui->lineEdit_4->text().trimmed().isEmpty())
    {
        showErrorMessage("請輸入電話號碼！");
        ui->lineEdit_4->setFocus();
        return false;
    }
    
    return true;
}

void Widget::clearInputs()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit->setFocus();
}

void Widget::showErrorMessage(const QString &message)
{
    QMessageBox::warning(this, "錯誤", message);
}

void Widget::showInfoMessage(const QString &message)
{
    QMessageBox::information(this, "訊息", message);
}

void Widget::loadRowToInputs(int row)
{
    if(row < 0 || row >= ui->tableWidget->rowCount())
        return;
    
    QTableWidgetItem *item;
    
    item = ui->tableWidget->item(row, 0);
    ui->lineEdit->setText(item ? item->text() : "");
    
    item = ui->tableWidget->item(row, 1);
    ui->lineEdit_2->setText(item ? item->text() : "");
    
    item = ui->tableWidget->item(row, 2);
    ui->lineEdit_3->setText(item ? item->text() : "");
    
    item = ui->tableWidget->item(row, 3);
    ui->lineEdit_4->setText(item ? item->text() : "");
}

void Widget::on_btnAdd_clicked()
{
    if(!validateInputs())
        return;
    
    // Check for duplicate student ID
    QString studentId = ui->lineEdit->text().trimmed();
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        if(item && item->text() == studentId)
        {
            showErrorMessage("該學號已存在！");
            return;
        }
    }
    
    QTableWidgetItem *col1, *col2, *col3, *col4;
    col1 = new QTableWidgetItem(ui->lineEdit->text().trimmed());
    col2 = new QTableWidgetItem(ui->lineEdit_2->text().trimmed());
    col3 = new QTableWidgetItem(ui->lineEdit_3->text().trimmed());
    col4 = new QTableWidgetItem(ui->lineEdit_4->text().trimmed());

    int rc = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rc);

    ui->tableWidget->setItem(rc, 0, col1);
    ui->tableWidget->setItem(rc, 1, col2);
    ui->tableWidget->setItem(rc, 2, col3);
    ui->tableWidget->setItem(rc, 3, col4);
    
    clearInputs();
    showInfoMessage("新增成功！");
}

void Widget::on_btnExport_clicked()
{
    if(ui->tableWidget->rowCount() == 0)
    {
        showErrorMessage("沒有資料可以匯出！");
        return;
    }
    
    QString saveFile = "";
    int rc = ui->tableWidget->rowCount();
    int cc = ui->tableWidget->columnCount();

    QString mFilename = QFileDialog::getSaveFileName(this, "儲存檔案", "", "CSV檔案 (*.csv)");
    
    if(mFilename.isEmpty())
        return;

    for(int i = 0; i < rc; i++)
    {
        for(int j = 0; j < cc; j++)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            saveFile += item ? item->text() : "";
            if (j < cc - 1)
                saveFile += ",";
        }
        saveFile += '\n';
    }
    
    writeString(mFilename, saveFile);
    showInfoMessage("匯出成功！");
}

void Widget::on_btnImport_clicked()
{
    QString Filename = QFileDialog::getOpenFileName(
        this,
        "讀取資料",
        "",
        "CSV檔案 (*.csv);;所有檔案 (*.*)"
    );

    if (Filename.isEmpty())
        return;

    QFile file(Filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        showErrorMessage("無法開啟檔案！");
        return;
    }

    // Confirm before clearing existing data
    if(ui->tableWidget->rowCount() > 0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "確認", "匯入將清空現有資料，是否繼續？",
                                      QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            file.close();
            return;
        }
    }

    ui->tableWidget->setRowCount(0);

    QTextStream in(&file);
    int row = 0;
    int importedCount = 0;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if(line.isEmpty())
            continue;

        QStringList parts = line.split(",");
        
        if(parts.size() < 4)
            continue;

        ui->tableWidget->insertRow(row);

        for(int col = 0; col < 4 && col < parts.size(); col++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(parts[col].trimmed());
            ui->tableWidget->setItem(row, col, item);
        }

        row++;
        importedCount++;
    }

    file.close();
    showInfoMessage(QString("成功匯入 %1 筆資料！").arg(importedCount));
}

void Widget::on_btnExit_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "確認", "確定要結束程式嗎？",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        close();
    }
}

void Widget::on_btnDelete_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    
    if(currentRow < 0)
    {
        showErrorMessage("請先選擇要刪除的項目！");
        return;
    }
    
    QTableWidgetItem *nameItem = ui->tableWidget->item(currentRow, 2);
    QString name = nameItem ? nameItem->text() : "此項目";
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "確認刪除", 
                                  QString("確定要刪除「%1」嗎？").arg(name),
                                  QMessageBox::Yes | QMessageBox::No);
    
    if(reply == QMessageBox::Yes)
    {
        ui->tableWidget->removeRow(currentRow);
        showInfoMessage("刪除成功！");
    }
}

void Widget::on_btnEdit_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    
    if(currentRow < 0)
    {
        showErrorMessage("請先選擇要修改的項目！");
        return;
    }
    
    if(!validateInputs())
        return;
    
    // Check for duplicate student ID (excluding current row)
    QString studentId = ui->lineEdit->text().trimmed();
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if(i == currentRow)
            continue;
            
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        if(item && item->text() == studentId)
        {
            showErrorMessage("該學號已存在！");
            return;
        }
    }
    
    ui->tableWidget->item(currentRow, 0)->setText(ui->lineEdit->text().trimmed());
    ui->tableWidget->item(currentRow, 1)->setText(ui->lineEdit_2->text().trimmed());
    ui->tableWidget->item(currentRow, 2)->setText(ui->lineEdit_3->text().trimmed());
    ui->tableWidget->item(currentRow, 3)->setText(ui->lineEdit_4->text().trimmed());
    
    clearInputs();
    showInfoMessage("修改成功！");
}

void Widget::on_btnClear_clicked()
{
    clearInputs();
}

void Widget::on_lineEditSearch_textChanged(const QString &text)
{
    QString searchText = text.trimmed().toLower();
    
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        bool match = false;
        
        if(searchText.isEmpty())
        {
            match = true;
        }
        else
        {
            // Search in student ID (column 0) and name (column 2)
            QTableWidgetItem *idItem = ui->tableWidget->item(i, 0);
            QTableWidgetItem *nameItem = ui->tableWidget->item(i, 2);
            
            if((idItem && idItem->text().toLower().contains(searchText)) ||
               (nameItem && nameItem->text().toLower().contains(searchText)))
            {
                match = true;
            }
        }
        
        ui->tableWidget->setRowHidden(i, !match);
    }
}

