#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>



void writeString(QString Filename,QString str)
{
    QFile mFile(Filename);
    if(! mFile.open(QFile::WriteOnly | QFile::Text))//如果開檔失敗
    {
        qDebug()<<"Error";
        return;
    }
    QTextStream out(&mFile);//out: 文字串流物件-->mFile:檔案物件
    out<<str; //文字輸出
    out.flush(); //清空緩衝區

}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList colTitle;
    ui->tableWidget->setColumnCount(4);
    colTitle<<"學號"<<"班級"<<"姓名"<<"電話";
    ui->tableWidget->setHorizontalHeaderLabels(colTitle);

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    QTableWidgetItem *col1,*col2,*col3,*col4;
    col1=new QTableWidgetItem(QString(ui->lineEdit->text()));
    col2=new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    col3=new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    col4=new QTableWidgetItem(QString(ui->lineEdit_4->text()));

    int rc=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rc);

    ui->tableWidget->setItem(rc,0,col1);
    ui->tableWidget->setItem(rc,1,col2);
    ui->tableWidget->setItem(rc,2,col3);
    ui->tableWidget->setItem(rc,3,col4);
}


void Widget::on_pushButton_2_clicked()
{
    QString saveFile="";
    int rc,cc;
    rc=ui->tableWidget->rowCount();
    cc=ui->tableWidget->columnCount();

    QString mFilename=QFileDialog::getSaveFileName(this,"存檔",",");

    for(int i=0;i<rc;i++)
    {
        for(int j=0;j<cc;j++)
        {
            saveFile += ui->tableWidget->item(i, j)->text();
            if (j < cc - 1)
                saveFile += ",";
        }
        saveFile+='\n';
    }
    writeString(mFilename,saveFile);
}


void Widget::on_pushButton_3_clicked()
{

    // 1. 出現檔案選擇視窗，讓使用者選要讀取的檔案
    QString Filename = QFileDialog::getOpenFileName(
        this,
        "讀取資料",                     // 視窗標題
        ""                            // 預設路徑
        );

    // 2. 如果使用者按「取消」，Filename 就是空字串，直接結束
    if (Filename.isEmpty())
        return;

    // 3. 用使用者選的檔名建立 QFile 物件
    QFile file(Filename);

    // 4. 嘗試開啟檔案（唯讀模式、純文字）
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Error";
        return;
    }

    // 5. 清空 tableWidget，準備載入新資料
    ui->tableWidget->setRowCount(0);

    // 6. 用 QTextStream 讀取文字
    QTextStream in(&file);
    int row = 0;

    // 7. 一行一行讀取，直到檔案結束
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();  // 讀一行並去掉前後空白
        if(line.isEmpty())
            continue;  // 空行跳過

        // 8. 用逗號分割 CSV 欄位
        QStringList parts = line.split(",");

        // 9. 新增一列到 tableWidget
        ui->tableWidget->insertRow(row);

        // 10. 把每一個欄位的資料放到該列
        for(int col = 0; col < parts.size(); col++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(parts[col]);
            ui->tableWidget->setItem(row, col, item);
        }

        // 11. 要寫下一列了
        row++;
    }

    // 12. 關閉檔案
    file.close();
    }




void Widget::on_pushButton_4_clicked()
{
    Widget::on_pushButton_clicked();
    close();
}

